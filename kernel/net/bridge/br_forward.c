/*
 *	Forwarding decision
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: br_forward.c,v 1.9 2008/04/10 06:09:16 pluswang Exp $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/skbuff.h>
#include <linux/if_bridge.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"

static inline int should_deliver(struct net_bridge_port *p, struct sk_buff *skb)
{
	if (skb->dev == p->dev ||
	    p->state != BR_STATE_FORWARDING)
		return 0;

#ifdef MULTICAST_BWCTRL
	if (p->bandwidth != 0)
	{
		if ((p->accumulation + skb->len) > p->bandwidth)
			return 0;
		p->accumulation += skb->len;
	}
#endif

#ifdef MULTICAST_FILTER
	if ((p->br->fltr_portlist_num) &&
		!memcmp(skb->mac.ethernet->h_dest, "\x01\x00\x5e", 3))
	{
		int i, filter = 0;
		unsigned short frag_offset = *((unsigned short *)&(skb->data[6]));
		unsigned short port = *((unsigned short *)&(skb->data[22]));
		unsigned long x;

		if ((frag_offset & 0x1fff) == 0) {	// check fragment offset
			for (i=0; i<p->br->fltr_portlist_num; i++) {
				if (port == p->br->fltr_portlist[i]) {
					filter = 1;
					break;
				}
			}
		}

		x = skb->mac.ethernet->h_dest[3] ^ skb->mac.ethernet->h_dest[4] ^ skb->mac.ethernet->h_dest[5];
		x = x & (MLCST_MAC_ENTRY - 1);

		if (!strcmp(p->dev->name, "wlan0") ||
			!strcmp(p->dev->name, "wlan1") ||
			!strcmp(p->dev->name, "wlan2"))
		{
			if (filter) {
				if (p->br->fltr_maclist[x][3] == 0) {
					memcpy(&(p->br->fltr_maclist[x][0]), &(skb->mac.ethernet->h_dest[3]), 3);
					p->br->fltr_maclist[x][3] = 1;
				}
				return 0;
			}
			else {
				if ((p->br->fltr_maclist[x][3] != 0) &&
					!memcmp(&(p->br->fltr_maclist[x][0]), &(skb->mac.ethernet->h_dest[3]), 3))
					return 0;
				else
					return 1;
			}
		}
		else
			return 1;
	}
	else
		return 1;
#else
	return 1;
#endif
}

static int __dev_queue_push_xmit(struct sk_buff *skb)
{
	skb_push(skb, ETH_HLEN);
	dev_queue_xmit(skb);

	return 0;
}

static int __br_forward_finish(struct sk_buff *skb)
{
#if 0
	if (!(list_empty(&nf_hooks[(PF_BRIDGE)][(NF_BR_POST_ROUTING)])))
		printk("netfilter not empty in (PF_BRIDGE)][(NF_BR_POST_ROUTING\n");	
#endif

	NF_HOOK(PF_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
			__dev_queue_push_xmit);

	return 0;
}

static void __br_deliver(struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	indev = skb->dev;
	skb->dev = to->dev;
#if 0	
	if (!(list_empty(&nf_hooks[(PF_BRIDGE)][(NF_BR_LOCAL_OUT)])))
		                  printk("netfilter not empty in (PF_BRIDGE)][(NF_BR_POST_ROUTING\n");
#endif

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, skb, indev, skb->dev,
			__br_forward_finish);
}

static void __br_forward(struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	indev = skb->dev;
	skb->dev = to->dev;
#if 0	
	if (!(list_empty(&nf_hooks[(PF_BRIDGE)][(NF_BR_FORWARD)])))
	printk("netfilter not empty in (PF_BRIDGE)][(NF_BR_POST_ROUTING\n");
#endif

	NF_HOOK(PF_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
			__br_forward_finish);
}

/* called under bridge lock */
void br_deliver(struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_deliver(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called under bridge lock */
void br_forward(struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_forward(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called under bridge lock */
static void br_flood(struct net_bridge *br, struct sk_buff *skb, int clone,
	void (*__packet_hook)(struct net_bridge_port *p, struct sk_buff *skb))
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;

	if (clone) {
		struct sk_buff *skb2;
		if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
			br->statistics.tx_dropped++;
			return;
		}

		skb = skb2;
	}

	prev = NULL;

	p = br->port_list;
	while (p != NULL) {
		if (should_deliver(p, skb)) {
			if (prev != NULL) {
				struct sk_buff *skb2;
				if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
					br->statistics.tx_dropped++;
					kfree_skb(skb);
					return;
				}

				__packet_hook(prev, skb2);
			}

			prev = p;
		}

		p = p->next;
	}

	if (prev != NULL) {
		__packet_hook(prev, skb);
		return;
	}

	kfree_skb(skb);
}

/* called under bridge lock */
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb, int clone)
{
	br_flood(br, skb, clone, __br_deliver);
}

/* called under bridge lock */
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb, int clone)
{
	br_flood(br, skb, clone, __br_forward);
}
#ifdef IGMP_SNOOPING
static void br_multicast(struct net_bridge *br, struct net_bridge_fdb_entry *dst, struct sk_buff *skb, int clone, 
		  void (*__packet_hook)(struct net_bridge_port *p, struct sk_buff *skb))
{
	//	char i;
	struct net_bridge_port *prev;
	struct net_bridge_port *p;
	unsigned short port_bitmask=0;
        if (clone) {
                struct sk_buff *skb2;
                                                                                                                                              
                if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
                        br->statistics.tx_dropped++;
                        return;
                }
                                                                                                                                              
                skb = skb2;
        }
                                                                                                                                              
	prev = NULL;
	p = br->port_list;
	while (p != NULL) {
        port_bitmask = 0;
        port_bitmask = (1 << p->port_no);
                if ((port_bitmask & dst->group_src) && should_deliver(p, skb)) {
                        if (prev != NULL) {
                                struct sk_buff *skb2;
                                                                                                                                 
                                if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
                                        br->statistics.tx_dropped++;
                                        kfree_skb(skb);
                                        return;
                                }
                                        
                                __packet_hook(prev, skb2);
                        }
                                                                                                                       
                        prev = p;
                }
                p = p->next;
	}
        if (prev != NULL) {
                __packet_hook(prev, skb);
                return;
        }

	kfree_skb(skb);
}

void br_multicast_deliver(struct net_bridge *br, struct net_bridge_fdb_entry *dst, struct sk_buff *skb, int clone)
{
	br_multicast(br, dst, skb, clone, __br_deliver);
}
void br_multicast_forward(struct net_bridge *br, struct net_bridge_fdb_entry *dst, struct sk_buff *skb, int clone)
{
	br_multicast(br, dst, skb, clone, __br_forward);
}

#endif
