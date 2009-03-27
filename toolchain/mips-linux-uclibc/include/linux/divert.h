/*
 * Frame Diversion, Benoit Locher <Benoit.Locher@skf.com>
 *
 * Changes:
 * 		06/09/2000	BL:	initial version
 * 		15/10/2000	BL:	changed to a rule based structure
 */
 
#ifndef _LINUX_DIVERT_H
#define _LINUX_DIVERT_H

#include <linux/version.h>
#include <linux/if_ether.h>

//#ifdef __KERNEL__
//#include <asm/spinlock.h>
//#endif	/* __KERNEL__ */


struct sk_buff;
struct divert_rule;
typedef int (*process_rule_func)(struct sk_buff *, struct divert_rule *);


enum DIVERT_PROTO
{
	DIVERT_PROTO_IP=10,
	DIVERT_PROTO_TCP=11,
	DIVERT_PROTO_UDP=12,
	DIVERT_PROTO_ICMP=13,
	DIVERT_PROTO_ARP=20,
	DIVERT_PROTO_IPX=30,
};


typedef struct _divert_cidr
{
	unsigned long	ipnum,
					ipmask;
} divert_cidr;


typedef struct _divert_icmp_blk
{
	unsigned long	scidr_negate;
	divert_cidr 	scidr;
	unsigned long	dcidr_negate;
	divert_cidr 	dcidr;
	unsigned short	type_negate,
					type;
} divert_icmp_blk;


typedef struct _divert_tcp_blk
{
	unsigned long   scidr_negate;
	divert_cidr 	scidr;
	unsigned long   dcidr_negate;
	divert_cidr 	dcidr;
	unsigned short	sport_negate,
					sport_start,
					sport_end,
					dport_negate,
					dport_start,
					dport_end;
} divert_tcp_blk;


typedef struct _divert_udp_blk
{
	unsigned long	scidr_negate;
	divert_cidr 	scidr;
	unsigned long	dcidr_negate;
	divert_cidr 	dcidr;
	unsigned short	sport_negate,
					sport_start,
					sport_end,
					dport_negate,
					dport_start,
	 				dport_end;
} divert_udp_blk;


typedef struct _divert_ip_blk
{
	unsigned long	scidr_negate;
	divert_cidr		scidr;
	unsigned long 	dcidr_negate;
	divert_cidr		dcidr;
} divert_ip_blk;


typedef struct _divert_arp_blk
{

} divert_arp_blk;


typedef struct _divert_ipx_blk
{

} divert_ipx_blk;


typedef struct _divert_spx_blk
{

} divert_spx_blk;


typedef union _divert_proto
{
	divert_ip_blk	ip;
	divert_tcp_blk	tcp;
	divert_udp_blk	udp;
	divert_icmp_blk	icmp;
	divert_arp_blk	arp;
	divert_ipx_blk	ipx;
	divert_spx_blk	spx;
} divert_proto;


#ifdef __KERNEL__

struct divert_rule
{
	struct divert_rule	*prev,
						*next;
	unsigned long		proto_num;
	divert_proto		proto_blk;
	int					divert_locally;
	unsigned char		dest_mac[ETH_ALEN];
	process_rule_func	process_rule;
};


struct divert_blk
{
	int			divert;			/* diverter status on|off */
	rwlock_t	rwlock;			/* for rule list interprocess locking */
	struct divert_rule	*rule;	/* the rule doubly-linked list */
};

#endif	/* __KERNEL__ */

/*
 * Diversion control block, for configuration with the userspace tool
 * divert
 */

struct divert_cf
{
	int	cmd;							/* Command */
	unsigned int    rule_num;			/* Used to retrieve a specific rule */
	unsigned int    proto_num;			/* Protocol this rule works on */
	divert_proto    proto_blk;			/* Rule protocol description */
	int				status;				/* Used to get the status of the diverter */
	int				divert_locally;		/* acts locally to the specified interface */
	unsigned char	dest_mac[ETH_ALEN];	/* If not divert_locally, mac address it should
										   be forwarded to */
	char			dev_name[32];		/* Device name (eth0...) */
};


/* Diversion commands */
#define	DIVERT_SET_STATE		1	/* Switch diversion on or off */
#define	DIVERT_APPEND_RULE		2	/* Add a rule */
#define	DIVERT_INSERT_RULE		3	/* Insert a rule */
#define	DIVERT_REPLACE_RULE		4	/* Replace a rule */
#define	DIVERT_DELETE_RULE		5	/* Delete a rule */
#define	DIVERT_FLUSH_RULES		6	/* Flush all the rules */
#define	DIVERT_GET_RULE			7	/* Retrieve a rule */
#define	DIVERT_GET_STATUS		8	/* retrieve the version + diverter status */


#ifdef __KERNEL__

// Linux kernel 2.4.0+
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
#	define NETDEVICE   struct net_device
// Linux kernel 2.3.0+ not supported
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,0)
#	error "Linux kernel 2.3.x not supported !"
// Linux kernel 2.2.18+
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,18)
#	define NETDEVICE   struct device
#	define dev_put(a)
#else
#	error "This Linux kernel is not supported !"
#endif

extern int (*divert_ioctl_hook)(unsigned int, struct divert_cf *);
extern void (*divert_frame_hook)(struct sk_buff *skb); 
extern int (*alloc_divert_blk_hook)(NETDEVICE *);
extern void (*free_divert_blk_hook)(NETDEVICE *);
extern int (*dv_init_hook)(void);
extern char sysctl_divert_version_hook[32];


/* diverter functions */
#include <linux/skbuff.h>
int    dv_init(void);
void   dv_cleanup(void);
int    alloc_divert_blk(NETDEVICE *);
void   free_divert_blk(NETDEVICE *);
int    divert_ioctl(unsigned int, struct divert_cf *);
void   divert_frame(struct sk_buff *);

/* Rule processing routines */
int    process_ip_rule(struct sk_buff *, struct divert_rule *);
int    process_ip_tcp_rule(struct sk_buff *, struct divert_rule *);
int    process_ip_udp_rule(struct sk_buff *, struct divert_rule *);
int    process_ip_icmp_rule(struct sk_buff *, struct divert_rule *);

/* Rule manipulation routines */
struct divert_rule *divert_alloc_rule(void);
void   divert_free_rule(struct divert_rule *);
struct divert_rule *divert_get_rule(NETDEVICE *, int);
int    divert_append_rule(NETDEVICE *, struct divert_rule *);
int    divert_insert_rule(NETDEVICE *, struct divert_rule *, int);
int    divert_delete_rule(NETDEVICE *, int);
int    divert_replace_rule(NETDEVICE *, struct divert_rule *, int);
int    divert_flush_rules(NETDEVICE *);

#endif	/*  __KERNEL__ */

#endif	/* _LINUX_DIVERT_H */
