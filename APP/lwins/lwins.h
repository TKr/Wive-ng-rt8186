/*
    LWINS - Lightweight WINS Server http://www.lwins.org/

    Copyright (c) 2008 by Thomas Bluemel <thomasb@lwins.org>

    This file is part of LWINS.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#define ARRAY_CNT(arr) (sizeof (arr) / sizeof (arr[0]))
#define ASSERT(cnd) \
    if (!(cnd)) { \
        lwins_assert (#cnd, __FILE__, __LINE__); \
    }
#define LOG(level,fmt,...) \
    lwins_log (level, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGERR(level,func) \
    lwins_log (level, __FILE__, __LINE__, func " failed: %d (%s)\n", errno, strerror (errno))
#define LOGPACKET(packet,len,client,dumpfunc) \
    if (lwins_check_loglevel (LOG_PACKET)) \
        dumpfunc (packet, len, client)

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#define LWINS_ERR \
    LOG (LOG_ALL, "LWINS_ERR in %s at %s:%d\n", __FUNCTION__, __FILE__, __LINE__), 0

/* config.c */

struct lwins_config_option
{
    char *name;
    char **params;
    unsigned int params_cnt;
};

struct lwins_config
{
    struct lwins_config_option *options;
    unsigned int options_cnt;
};

int lwins_convert_to_integer (const char *param, unsigned int *ret);
int lwins_convert_to_bool (const char *param, int *ret);
int lwins_convert_to_ip (const char *param, struct in_addr *ret);
void lwins_free_config (struct lwins_config *cfg);
int lwins_read_config_args (int argc, const char *argv[], struct lwins_config *cfg);
int lwins_get_config_param (struct lwins_config *cfg, const char *option, unsigned int idx, const char **value);
int lwins_get_config_param_long (struct lwins_config *cfg, const char *option, unsigned int idx, unsigned long *value);
int lwins_config_get_option_param_cnt (struct lwins_config *cfg, const char *option);
int lwins_config_option_has_param (struct lwins_config *cfg, const char *option, const char *param);

/* discover.c */

#define DISCOVER_OP_UP 0
#define DISCOVER_OP_DOWN 1
struct nbns_discover_packet
{
    u32 op_code;
    struct in_addr *addresses;
    u16 addresses_cnt;
};

size_t calculate_nbns_discover_packet_size (struct nbns_discover_packet *packet);
void free_nbns_discover_packet (struct nbns_discover_packet *packet);
int parse_raw_nbns_discover_packet (unsigned char *raw_packet, size_t len, struct nbns_discover_packet *packet);
int build_raw_nbns_discover_packet (unsigned char *raw_packet, size_t len, struct nbns_discover_packet *packet);

/* netbios.c */
#define NETBT_MAX_LABELS 64

#define NETBT_COMPRESSED_MAX 255
#define NETBT_NAME_LENGTH 16

#define NETBT_HEADER_SIZE 12

#define NETBT_OP_QUERY 0
#define NETBT_OP_REGISTRATION 5
#define NETBT_OP_RELEASE 6
#define NETBT_OP_WACK 7
#define NETBT_OP_REFRESH 8

struct netbt_header
{
    u16 tran_id;
    union
    {
        u16 u;
        struct
        {
            u8 is_response : 1;
            u8 op_code : 4;
            u8 authoritive_answer : 1;
            u8 truncation : 1;
            u8 recursion_desired : 1;
            u8 recursion_available : 1;
            u8 broadcast : 1;
        };
    };
    u16 qscnt;
    u16 ancnt;
    u16 nscnt;
    u16 arcnt;
    u8 response_code;
};

struct netbt_question
{
    char *qname;
    u16 qtype;
    u16 qclass;
};

struct netbt_resource
{
    char *rname;
    unsigned char *rdata;
    u32 rttl;
    u16 rtype;
    u16 rclass;
    u16 rdlen;
};

struct netbt_packet
{
    struct sockaddr_in addr;
    struct netbt_header header;
    struct netbt_question *questions;
    struct netbt_resource *answers;
    struct netbt_resource *authority;
    struct netbt_resource *additional;
};

int is_name_reserved (const char *name);
int is_valid_netbios_name (const char *name);
int is_valid_domain_name (const char *name);
int encode_netbios_name (const char *name, const char *scope, char *encoded, size_t encoded_size);
int decode_netbios_name (const char *encoded_name, char *decoded_name, size_t decoded_size);
void netbt_free_packet (struct netbt_packet *packet);
int netbt_parse_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *addr, struct netbt_packet *packet);
int netbt_build_raw_packet (struct netbt_packet *packet, unsigned char *raw_packet, size_t len);
const char* netbt_packet_question (struct netbt_packet *packet, unsigned int idx);

/* replicate.c */

#define REPL_MSG_ASSOC_START_REQ 0x0
#define REPL_MSG_ASSOC_START_RESP 0x1
#define REPL_MSG_ASSOC_STOP_REQ 0x2
#define REPL_MSG_PUSHPULL 0x3
#define REPL_MSG_MAX REPL_MSG_PUSHPULL
struct replication_packet_header
{
    u32 assoc_handle;
    u32 msg_type;
};

struct replication_assoc_start
{
    u32 sdr_assoc_handle;
    u16 nbns_version_major;
    u16 nbns_version_minor;
};

#define REPL_ASSOC_STOP_NORMAL 0x0
#define REPL_ASSOC_STOP_ERROR 0x4
struct replication_assoc_stop
{
    u32 reason;
};

struct replication_owner_record
{
    struct in_addr addr;
    u32 max_version_high;
    u32 max_version_low;
    u32 min_version_high;
    u32 min_version_low;
};

struct replication_pushpull_ovmap
{
    u32 owners_cnt;
    struct replication_owner_record *owners;
};

struct replication_pushpull_updatenotify
{
    u32 owners_cnt;
    struct replication_owner_record *owners;
    struct in_addr initiator_addr;
};

struct replication_name_record_addr
{
    struct in_addr owner_addr;
    struct in_addr member_addr;
};

#define NREC_NODE_B 0x0
#define NREC_NODE_P 0x1
#define NREC_NODE_M 0x2

#define NREC_STATE_ACTIVE 0x0
#define NREC_STATE_TOMBSTONED 0x2

#define NREC_TYPE_UNIQUE 0x0
#define NREC_TYPE_GROUP 0x1
#define NREC_TYPE_GROUP_SPECIAL 0x2
#define NREC_TYPE_MULTIHOMED 0x3

struct replication_name_record
{
    char *name;
    u8 is_static;
    u8 node_type;
    u8 is_replica;
    u8 state;
    u8 type;
    u32 version_high;
    u32 version_low;
    u8 addrs_cnt;
    struct replication_name_record_addr *addrs;
};

struct replication_pushpull_namerec_req
{
    struct replication_owner_record rec;
};

struct replication_pushpull_namerec_resp
{
    u32 namerecs_cnt;
    struct replication_name_record **namerecs;
};

#define REPL_PUSHPULL_OP_OVMAP_REQ 0x0
#define REPL_PUSHPULL_OP_OVMAP_RESP 0x1
#define REPL_PUSHPULL_OP_NREC_REQ 0x2
#define REPL_PUSHPULL_OP_NREC_RESP 0x3
#define REPL_PUSHPOLL_OP_UN_NO_PROPAGATE 0x4
#define REPL_PUSHPOLL_OP_UN_PROPAGATE 0x5
#define REPL_PUSHPOLL_OP_UN_PERSIST_NO_PROPAGATE 0x8
#define REPL_PUSHPOLL_OP_UN_PERSIST_PROPAGATE 0x9
struct replication_pushpull
{
    u8 op_code;
    
    union
    {
        struct replication_pushpull_ovmap ovmap_resp;
        struct replication_pushpull_updatenotify updatenotify;
        struct replication_pushpull_namerec_req namerec_req;
        struct replication_pushpull_namerec_resp namerec_resp;
    };
};

struct replication_packet
{
    struct replication_packet_header header;
    union
    {
        struct replication_assoc_start assoc_start;
        struct replication_assoc_stop assoc_stop;
        struct replication_pushpull pushpull;
    };
};

struct association
{
    u32 handle;
    int sd;
    struct sockaddr_in client;
    
    unsigned char *buffer;
    size_t buffer_size;
    size_t buffer_length;
    
    u32 sender_handle;
    u8 recv_start_req;
    u8 close_conn;
};

struct replication_service
{
    /* association handles */
    struct association **associations;
    unsigned short associations_size;
    unsigned short associations_cnt;
    
    unsigned int max_packet_size;
};

int replication_build_raw_packet (unsigned char *raw_packet, size_t packet_size, struct replication_packet *packet);
int replication_parse_raw_packet (unsigned char *raw_packet, size_t packet_size, struct replication_packet *packet);
void replication_free_packet (struct replication_packet *packet);
struct association *alloc_association (struct replication_service *service, int sd, struct sockaddr_in *client);
void free_association (struct replication_service *service, struct association *assoc);
struct association *find_association (struct replication_service *service, int sd);
struct replication_service *init_replication_service (struct lwins_config *cfg);
void free_replication_service (struct replication_service *service);
int association_buffer_receive(struct replication_service *service, struct association *assoc, unsigned char *buffer, size_t buffer_size);

/* server.c */
struct netbios_name
{
    char *name;
};

struct netbios_scope
{
    char *name;
    struct netbios_name *names;
    unsigned int names_cnt;
};

struct netbios_server
{
    volatile unsigned int *terminate;
    
    int nbns_udp_sd;
    int replication_sd;
    int discovery_sd;
    
    struct lwins_config *config;
    struct replication_service *replication;
    
    struct netbios_scope global_scope;
    struct netbios_scope *scopes;
    unsigned int scopes_cnt;
};

int lwins_make_socket_nonblocking (int sd);
int lwins_set_socket_broadcast (int sd, int enable);
int lwins_broadcast_discover_msg (struct netbios_server *server, u32 op_code);
struct netbios_server* lwins_setup_server (struct lwins_config *cfg, volatile unsigned int *terminate);
void lwins_run_server (struct netbios_server *server);
void lwins_free_server (struct netbios_server *server);

void *lwins_alloc (size_t size);
void lwins_free (void *ptr);

/* debug.c */
void lwins_assert (const char *msg, const char *file, int line);

#define LOG_ALL 0
#define LOG_PACKET 31
void lwins_log (int level, const char *file, int line, const char *fmt, ...);
int lwins_check_loglevel (int level);

void lwins_dump_nbns_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *client);
void lwins_dump_discovery_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *client);
void lwins_dump_replication_raw_packet (unsigned char *raw_packet, size_t len, struct sockaddr_in *client);

