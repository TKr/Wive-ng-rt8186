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
#include <lwins.h>

static int lwins_get_interface_addresses (int sd, struct lwins_config *cfg, const char *option, struct in_addr **paddrs);

void *lwins_alloc (size_t size)
{
    void *ptr = malloc (size);
    if (ptr)
        memset (ptr, 0, size);
    return ptr;
}

void lwins_free (void *ptr)
{
    free (ptr);
}

int lwins_make_socket_nonblocking (int sd)
{
    long flags = fcntl (sd, F_GETFL);
    if (fcntl (sd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        LOGERR (LOG_ALL, "fcntl");
        return 0;
    }

    return 1;
}

int lwins_set_socket_broadcast (int sd, int enable)
{
    int on;
    
    on = enable != 0;
    if (setsockopt (sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof (on)) == -1)
    {
        LOGERR (LOG_ALL, "setsocketopt");
        return 0;
    }
    
    return 1;
}

static int lwins_set_multicast_if (int sd, struct in_addr *ifc)
{
    struct in_addr ifcaddr = *ifc;
    
    if (setsockopt (sd, IPPROTO_IP, IP_MULTICAST_IF, &ifcaddr, sizeof (ifcaddr)) == -1)
    {
        LOGERR (LOG_ALL, "setsockopt");
        return LWINS_ERR;
    }
    return 1;
}

static int lwins_multicast_group (int sd, struct in_addr *addr, struct in_addr *ifcs, unsigned int ifcs_cnt, int add)
{
    struct ip_mreq mr;
    unsigned int i, cnt = 0;
    
    if (ifcs_cnt == 0 || ifcs_cnt > IP_MAX_MEMBERSHIPS)
        return LWINS_ERR;
    
    for (i = 0; i < ifcs_cnt; i++)
    {
        mr.imr_multiaddr = *addr;
        mr.imr_interface = ifcs[i];
        
        if (setsockopt (sd, IPPROTO_IP, add ? IP_ADD_MEMBERSHIP : IP_DROP_MEMBERSHIP, &mr, sizeof (mr)) == 0)
            cnt++;
        else
            LOGERR (LOG_ALL, "setsockopt");
    }
    
    return cnt;
}

int lwins_broadcast_discover_msg (struct netbios_server *server, u32 op_code)
{
    struct nbns_discover_packet packet;
    unsigned char *raw_packet;
    size_t raw_packet_size;
    ssize_t s;
    struct sockaddr_in addr_multicast;
    int i, addr_cnt, cnt = 0;
    
    (void)server;
    
    ASSERT (op_code == DISCOVER_OP_UP || op_code == DISCOVER_OP_DOWN);
    
    addr_multicast.sin_family = AF_INET;
    addr_multicast.sin_port = htons (42);
    if (!inet_aton ("224.0.1.24", &addr_multicast.sin_addr))
        return LWINS_ERR;
    
    addr_cnt = lwins_get_interface_addresses (server->replication_sd, server->config, "replication-interfaces", &packet.addresses);
    if (addr_cnt >= 0xFFFF)
    {
        if (addr_cnt > 0)
            lwins_free (packet.addresses);
        return LWINS_ERR;
    }
    else if (addr_cnt == 0)
        return 0;
    
    packet.op_code = op_code;
    packet.addresses_cnt = addr_cnt & 0xFFFF;
    
    raw_packet_size = calculate_nbns_discover_packet_size (&packet);
    raw_packet = lwins_alloc (raw_packet_size);
    if (!raw_packet)
    {
        if (addr_cnt > 0)
            lwins_free (packet.addresses);
        return LWINS_ERR;
    }
    
    if (!build_raw_nbns_discover_packet (raw_packet, raw_packet_size, &packet))
    {
        if (addr_cnt > 0)
            lwins_free (packet.addresses);
        lwins_free (raw_packet);
        return LWINS_ERR;
    }
    
    for (i = 0; i < addr_cnt; i++)
    {
        if (!lwins_set_multicast_if (server->discovery_sd, &packet.addresses[i]))
        {
            LOG (LOG_ALL, "Could not broadcast discovery packet\n");
            continue;
        }
        
        s = sendto (server->discovery_sd, raw_packet, (size_t)raw_packet_size, 0,
            (const struct sockaddr *)&addr_multicast, sizeof (struct sockaddr));
        if ((size_t)s == raw_packet_size)
            cnt++;
        else if (s < 0)
            LOGERR (LOG_ALL, "sendto");
    }
    
    if (addr_cnt > 0)
        lwins_free (packet.addresses);
    lwins_free (raw_packet);
    return cnt;
}

static int lwins_get_interface_addresses (int sd, struct lwins_config *cfg, const char *option, struct in_addr **paddrs)
{
    struct in_addr *addrs;
    struct ifconf ifc;
    struct ifreq *buf = NULL;
    struct ifreq *pifc;
    int i, j, cnt, addr_cnt, icfg, filter = 0;
    size_t nbuf = 1;
    
    icfg = lwins_config_get_option_param_cnt (cfg, option);
    if (icfg == 0)
        return 0;
    
    for (;;)
    {
        if (buf)
            lwins_free (buf);
        buf = lwins_alloc (nbuf * sizeof (*buf));
        if (!buf)
            return LWINS_ERR;
        
        ifc.ifc_len = nbuf * sizeof (*buf);
        ifc.ifc_buf = (char *)buf;
        
        if (ioctl (sd, SIOCGIFCONF, &ifc) < 0)
        {
            LOGERR (LOG_ALL, "ioctl");
            return LWINS_ERR;
        }
        
        if (ifc.ifc_len == (int)(nbuf * sizeof (*buf)))
            nbuf += 1;
        else
            break;
    }
    
    if (icfg >= 0 &&
        !lwins_config_option_has_param (cfg, option, "all") &&
        !lwins_config_option_has_param (cfg, option, "*"))
    {
        filter = 1;
    }
    
    pifc = ifc.ifc_req;
    cnt = ifc.ifc_len / sizeof (struct ifreq);
    j = addr_cnt = 0;
    
    for (i = 0; i < cnt; i++)
    {
        if (ioctl (sd, SIOCGIFFLAGS, &pifc[i]) < 0)
        {
            LOGERR (LOG_ALL, "ioctl");
            return LWINS_ERR;
        }
        if (!(pifc[i].ifr_flags & IFF_UP))
            continue;
        
        if (filter && !lwins_config_option_has_param (cfg, option, pifc[i].ifr_name))
            continue;
        
        if (!(pifc[i].ifr_flags & IFF_LOOPBACK))
            addr_cnt++;
    }
    
    addrs = lwins_alloc (addr_cnt * sizeof (*addrs));
    if (!addrs)
        return LWINS_ERR;
    
    for (i = 0; i < cnt; i++)
    {
        if (!(pifc[i].ifr_flags & IFF_UP))
            continue;
        
        if (filter && !lwins_config_option_has_param (cfg, option, pifc[i].ifr_name))
            continue;
        
        if (!(pifc[i].ifr_flags & IFF_LOOPBACK))
            addrs[j++].s_addr = ((struct sockaddr_in *)&pifc[i].ifr_addr)->sin_addr.s_addr;
    }
    
    *paddrs = addrs;
    return addr_cnt;
}

static int lwins_setup_nbns_listener (struct netbios_server *server)
{
    struct sockaddr_in srv;
    
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = htonl (INADDR_ANY);
    srv.sin_port = htons (137);
    
    server->nbns_udp_sd = socket (AF_INET, SOCK_DGRAM, 0);
    if (server->nbns_udp_sd == -1)
    {
        LOGERR (LOG_ALL, "socket");
        return LWINS_ERR;
    }
    if (!lwins_make_socket_nonblocking (server->nbns_udp_sd))
        return LWINS_ERR;
    if (bind (server->nbns_udp_sd, (struct sockaddr*)&srv, sizeof (srv)) != 0)
    {
        LOGERR (LOG_ALL, "bind");
        return LWINS_ERR;
    }
    
    return 1;
}

static int lwins_setup_discovery_listener (struct netbios_server *server)
{
    struct sockaddr_in srv;
    struct in_addr *addrs;
    unsigned int addrs_cnt;
    
    srv.sin_family = AF_INET;
    srv.sin_port = htons (42);
    if (!inet_aton ("224.0.1.24", &srv.sin_addr))
        return LWINS_ERR;
    
    server->discovery_sd = socket (AF_INET, SOCK_DGRAM, 0);
    if (server->discovery_sd == -1)
    {
        LOGERR (LOG_ALL, "socket");
        return LWINS_ERR;
    }
    
    if (!lwins_make_socket_nonblocking (server->discovery_sd))
        return LWINS_ERR;
    if (!lwins_set_socket_broadcast (server->discovery_sd, 1))
        return LWINS_ERR;
    
    if (bind (server->discovery_sd, (struct sockaddr*)&srv, sizeof (srv)) != 0)
    {
        LOGERR (LOG_ALL, "bind");
        return LWINS_ERR;
    }
    
    addrs_cnt = lwins_get_interface_addresses (server->discovery_sd, server->config, "replication-interfaces", &addrs);
    if (addrs_cnt > 0)
    {
        /* Join the multicast group */
        if (!lwins_multicast_group (server->discovery_sd, &srv.sin_addr, addrs, addrs_cnt, 1))
            return LWINS_ERR;
        
        if (addrs)
            lwins_free (addrs);
    }
    
    return 1;
}

static int lwins_setup_replication_listener (struct netbios_server *server)
{
    struct sockaddr_in srv;
    struct in_addr *addrs;
    unsigned int i, addrs_cnt;
    unsigned long val;
    
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = htonl (INADDR_ANY);
    if (lwins_get_config_param_long (server->config, "replication-port", 0, &val))
    {
        if (val > 0xFFFF)
        {
            LOG (LOG_ALL, "Invalid repliaction-port %u, using default\n", val);
            srv.sin_port = htons (42);
        }
        else
            srv.sin_port = htons (val & 0xFFFF);
    }
    else
        srv.sin_port = htons (42);
    
    server->replication_sd = socket (AF_INET, SOCK_STREAM, 0);
    if (server->replication_sd == -1)
    {
        LOGERR (LOG_ALL, "socket");
        return LWINS_ERR;
    }
    
    if (bind (server->replication_sd, (struct sockaddr*)&srv, sizeof (srv)) != 0)
    {
        LOGERR (LOG_ALL, "bind");
        return LWINS_ERR;
    }
    addrs_cnt = lwins_get_interface_addresses (server->replication_sd, server->config, "replication-interfaces", &addrs);
    if (addrs_cnt > 0)
    {
        for (i = 0; i < addrs_cnt; i++)
            printf ("Replication service listening on %s\n", inet_ntoa (addrs[i]));
        if (addrs)
            lwins_free (addrs);
    }
    
    if (listen (server->replication_sd, 32) == -1)
    {
        LOGERR (LOG_ALL, "listen");
        return LWINS_ERR;
    }
    
    return 1;
}

struct netbios_server* lwins_setup_server (struct lwins_config *cfg, volatile unsigned int *terminate)
{
    struct netbios_server *server;
    const char *value;
    uid_t uid = 0;
    gid_t gid = 0;
    int found_usr = 0;
    int found_grp = 0;
    
    server = lwins_alloc (sizeof (*server));
    if (!server)
        return NULL;
    server->terminate = terminate;
    server->config = cfg;
    
    server->nbns_udp_sd = -1;
    server->replication_sd = -1;
    server->discovery_sd = -1;
    
    server->replication = init_replication_service (server->config);
    if (!server->replication ||
        !lwins_setup_nbns_listener (server) ||
        !lwins_setup_replication_listener (server) ||
        !lwins_setup_discovery_listener (server))
    {
        lwins_free_server (server);
        return NULL;
    }
    
    if (lwins_get_config_param (server->config, "user", 0, &value))
    {
        struct passwd *pwd;
        
        if (!lwins_convert_to_integer (value, &uid))
        {
            pwd = getpwnam (value);
            if (!pwd)
            {
                LOG (LOG_ALL, "User %s could not be found\n", value);
                lwins_free_server (server);
                return NULL;
            }
            
            uid = pwd->pw_uid;
            gid = pwd->pw_gid;
            found_grp = 1;
        }
        found_usr = 1;
    }
    
    if (lwins_get_config_param (server->config, "group", 0, &value))
    {
        struct group *grp;
        
        if (!lwins_convert_to_integer (value, &gid))
        {
            grp = getgrnam (value);
            if (!grp)
            {
                LOG (LOG_ALL, "Group %s could not be found\n", value);
                lwins_free_server (server);
                return NULL;
            }
            
            gid = grp->gr_gid;
        }
        found_grp = 1;
    }
    
    if (found_grp && setgid (gid) != 0)
    {
        LOGERR (LOG_ALL, "setgid");
        LOG (LOG_ALL, "Could not set the process group identity\n");
        lwins_free_server (server);
        return NULL;
    }
    
    if (found_usr && setuid (uid) != 0)
    {
        LOGERR (LOG_ALL, "setuid");
        LOG (LOG_ALL, "Could not set the process user identity\n");
        lwins_free_server (server);
        return NULL;
    }
    
    return server;
}

static void lwins_free_name (struct netbios_server *server, struct netbios_scope *scope, struct netbios_name *name)
{
    (void)server;
    (void)scope;
    
    if (name->name)
        lwins_free (name->name);
    memset (name, 0, sizeof (*name));
}

static void lwins_free_scope (struct netbios_server *server, struct netbios_scope *scope)
{
    unsigned int i;
    
    (void)server;
    
    if (scope->names)
    {
        for (i = 0; i < scope->names_cnt; i++)
            lwins_free_name (server, scope, &scope->names[i]);
        lwins_free (scope->names);
    }
    
    if (scope->name)
        lwins_free (scope->name);
    memset (scope, 0, sizeof (*scope));
}

void lwins_free_server (struct netbios_server *server)
{
    unsigned int i;
    
    lwins_free_scope (server, &server->global_scope);
    
    if (server->scopes)
    {
        for (i = 0; i < server->scopes_cnt; i++)
            lwins_free_scope (server, &server->scopes[i]);
        lwins_free (server->scopes);
    }
    
    if (server->nbns_udp_sd != -1)
        close (server->nbns_udp_sd);
    if (server->replication_sd != -1)
        close (server->replication_sd);
    if (server->discovery_sd != -1)
        close (server->discovery_sd);
    
    if (server->replication)
        free_replication_service (server->replication);
    lwins_free (server);
}

static int lwins_name_query (struct netbios_server *server, struct netbt_packet *packet)
{
    (void)server;
    
    if (!packet->header.recursion_desired)
        return 0;
    LOG (LOG_ALL, "Process name question: %s\n", netbt_packet_question (packet, 0));
    return 0;
}

static int lwins_process_request (struct netbios_server *server, struct netbt_packet *packet)
{
    int ret = 0;
    
    LOG (LOG_ALL, "Request from %s:%d tran_id: %u: op_code: %u\n", inet_ntoa (packet->addr.sin_addr),
        ntohs(packet->addr.sin_port), packet->header.tran_id, packet->header.op_code);
    
    switch (packet->header.op_code)
    {
        case NETBT_OP_QUERY:
            ret = lwins_name_query (server, packet);
            break;
        case NETBT_OP_REGISTRATION:
        case NETBT_OP_RELEASE:
        case NETBT_OP_WACK:
        case NETBT_OP_REFRESH:
            LOG (LOG_ALL, "Unimplemented packet op code: %u\n", packet->header.op_code);
            break;
        default:
            LOG (LOG_ALL, "Invalid packet op code: %u\n", packet->header.op_code);
            break;
    }
    
    return ret;
}

static int lwins_read_nbns_packet (struct netbios_server *server)
{
    struct netbt_packet packet;
    unsigned char buf[576];
    struct sockaddr_in client;
    ssize_t len;
    socklen_t slen;
    int ret;
    
    printf ("Read NETBIOS UDP 137 packet\n");
    
    memset (buf, 0, sizeof (buf));
    memset (&client, 0, sizeof (client));
    slen = sizeof (client);
    len = recvfrom (server->nbns_udp_sd, buf, sizeof (buf), 0, (struct sockaddr *)&client, &slen);
    if (len < 0)
    {
        LOGERR (LOG_ALL, "recvfrom");
        return LWINS_ERR;
    }
    
    ret = 0;
    
    if (netbt_parse_raw_packet (buf, (size_t)len, &client, &packet))
    {
        if (!packet.header.is_response) // don't handle response packets...
            ret = lwins_process_request (server, &packet);
        netbt_free_packet (&packet);
        ret = 1; //fixme
    }
    
    if (!ret)
        LOGPACKET (buf, (size_t)len, &client, lwins_dump_nbns_raw_packet);
    return 1;
}

static int read_discovery_packet (struct netbios_server *server)
{
    struct nbns_discover_packet packet;
    unsigned char buf[576];
    struct sockaddr_in client;
    ssize_t len;
    socklen_t slen;
    int ret;
    
    memset (buf, 0, sizeof (buf));
    memset (&client, 0, sizeof (client));
    memset (&packet, 0, sizeof (packet));
    
    slen = sizeof (client);
    len = recvfrom (server->discovery_sd, buf, sizeof (buf), 0, (struct sockaddr *)&client, &slen);
    if (len < 0)
    {
        LOGERR (LOG_ALL, "recvfrom");
        return LWINS_ERR;
    }
    
    ret = 0;
    
    if (parse_raw_nbns_discover_packet (buf, (size_t)len, &packet))
    {
        LOG (LOG_ALL, "Received discovery packet from %s:%d\n", inet_ntoa (client.sin_addr), ntohs(client.sin_port));
        
        ret = 0; // FIXME
    }
    
    if (!ret)
        LOGPACKET (buf, (size_t)len, &client, lwins_dump_discovery_raw_packet);
    
    free_nbns_discover_packet (&packet);
    return 1;
}

static int lwins_accept_replication_connection (struct netbios_server *server)
{
    struct association *assoc;
    struct sockaddr_in client;
    socklen_t client_len;
    int sd;
    
    client_len = sizeof (client);
    sd = accept (server->replication_sd, (struct sockaddr *)&client, &client_len);
    if (sd == -1)
    {
        LOGERR (LOG_ALL, "accept");
        return -1;
    }
    
    assoc = alloc_association (server->replication, sd, &client);
    if (!assoc)
    {
        LOG (LOG_ALL, "Failed to allocation association, closing connection!\n");
        close (sd);
        return -1;
    }
    return sd;
}

static int lwins_read_replication_connection (struct netbios_server *server, struct association *assoc)
{
    unsigned char buffer[512];
    int len;
    
    len = recv (assoc->sd, buffer, sizeof (buffer), 0);
    if (len < 0)
    {
        LOGERR (LOG_ALL, "recv");
        return LWINS_ERR;
    }
    else if (len == 0)
        return 0; /* connection closed */
    
    if (!association_buffer_receive (server->replication, assoc, buffer, (size_t)len))
    {
        if (assoc->close_conn)
            return 0;
        
        return LWINS_ERR;
    }
   
    return 1;
}

void lwins_run_server (struct netbios_server *server)
{
    struct association *assoc;
    struct timeval timeout;
    int maxfd, ret, i, sd, fds_cnt = 0;
    fd_set read_fds, master_fds;
    
    FD_ZERO (&read_fds);
    FD_ZERO (&master_fds);
    FD_SET (server->nbns_udp_sd, &master_fds);
    fds_cnt++;
    maxfd = server->nbns_udp_sd;
    FD_SET (server->replication_sd, &master_fds);
    fds_cnt++;
    if (server->replication_sd > maxfd)
        maxfd = server->replication_sd;
    FD_SET (server->discovery_sd, &master_fds);
    fds_cnt++;
    if (server->discovery_sd > maxfd)
        maxfd = server->discovery_sd;
    
    LOG (LOG_ALL, "WINS server running\n");
    
    if (!lwins_broadcast_discover_msg (server, DISCOVER_OP_UP))
        LOG (LOG_ALL, "Failed to broadcast discovery (up) message\n");
    
    do
    {
        memcpy (&read_fds, &master_fds, sizeof (read_fds));
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        ret = select (maxfd + 1, &read_fds, NULL, NULL, &timeout);
        if (ret < 0)
        {
            LOGERR (LOG_ALL, "select");
            break;
        }
        else if (ret == 0)
        {
            /* perform timed action */
            //printf ("Timeout\n");
        }
        else
        {
            for (i = 0; i <= maxfd; i++)
            {
                if (FD_ISSET (i, &read_fds))
                {
                    if (i == server->nbns_udp_sd)
                        lwins_read_nbns_packet (server);
                    else if (i == server->replication_sd)
                    {
                        sd = lwins_accept_replication_connection (server);
                        if (sd != -1)
                        {
                            if (fds_cnt < FD_SETSIZE)
                            {
                                fds_cnt++;
                                FD_SET (sd, &master_fds);
                                if (sd > maxfd)
                                    maxfd = sd;
                            }
                            else
                            {
                                LOG (LOG_ALL, "Too many simultaneous connections!\n");
                                close (sd);
                            }
                        }
                    }
                    else if (i == server->discovery_sd)
                        read_discovery_packet (server);
                    else
                    {
                        assoc = find_association (server->replication, i);
                        if (assoc)
                        {
                            /* replication connection */
                            if (!lwins_read_replication_connection (server, assoc))
                            {
                                close (i);
                                FD_CLR (i, &master_fds);
                                if (i == maxfd)
                                {
                                    while (maxfd > 0 && !FD_ISSET (maxfd, &master_fds))
                                        maxfd--;
                                }
                                free_association (server->replication, assoc);
                            }
                        }
                        else
                        {
                            /* NETBIOS TCP connection */
                            LOG (LOG_ALL, "Not yet implemented!\n");
                        }
                    }
                }
            }
        }
    } while (!(*server->terminate));
    
    if (!lwins_broadcast_discover_msg (server, DISCOVER_OP_DOWN))
        LOG (LOG_ALL, "Failed to broadcast discovery (down) message\n");
    
    LOG (LOG_ALL, "WINS server terminated\n");
}

static volatile unsigned int terminate_server = 0;

static void handle_sigint (int signum)
{
    (void)signum;
    terminate_server = 1;
    signal (SIGINT, SIG_IGN);
}

int main (int argc, const char *argv[])
{
    struct netbios_server *server;
    struct lwins_config config;
    
    memset (&config, 0, sizeof (config));
    if (!lwins_read_config_args (argc, argv, &config))
    {
        lwins_free_config (&config);
        return 1;
    }
    
    server = lwins_setup_server (&config, &terminate_server);
    if (!server)
    {
        lwins_free_config (&config);
        fprintf (stderr, "Initialization failed!\n");
        return 1;
    }
    
    (void)signal (SIGINT, handle_sigint);
    
    lwins_run_server (server);
    lwins_free_config (&config);
    exit (0);
}

