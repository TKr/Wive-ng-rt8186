/* -*- linux-c -*- */
#ifndef _LINUX_IF_MACVLAN_H
#define _LINUX_IF_MACVLAN_H

/* the ioctl commands */

/* actions */
#define MACVLAN_ENABLE         1
#define MACVLAN_DISABLE        2
#define MACVLAN_ADD            3
#define MACVLAN_DEL            4
#define MACVLAN_BIND           5
#define MACVLAN_UNBIND         6

/* informative */
#define MACVLAN_GET_NUM_PORTS  7
#define MACVLAN_GET_PORT_NAME  8
#define MACVLAN_GET_NUM_VLANS  9
#define MACVLAN_GET_VLAN_NAME  10
#define MACVLAN_GET_NUM_MACS   11
#define MACVLAN_GET_MAC_NAME   12

#define MACVLAN_SET_PORT_FLAGS 13
#define MACVLAN_GET_PORT_FLAGS 14

/* If this IOCTL succeedes, we are a MAC-VLAN interface, otherwise, we are not. */
#define MACVLAN_IS_MACVLAN     15


#ifdef __KERNEL__
#include <linux/if.h>
#include <linux/netdevice.h>
extern int (*macvlan_ioctl_hook)(unsigned long arg);

/* Returns >= 0 if it consumed the packet, otherwise let the pkt
 * be processed by the netif_rx method, as if macvlan's didn't
 * exist.
 */
extern int (*macvlan_handle_frame_hook)(struct sk_buff *skb);
#endif

struct macvlan_ioctl_reply {
        int num;
        char name[IFNAMSIZ];
};

struct macvlan_ioctl {
        int  cmd;
        int portidx;
        char *ifname;
        int ifidx; /* flags when setting port flags */
        unsigned char *macaddr;
        int macaddridx;
        struct macvlan_ioctl_reply *reply;
};

#endif
