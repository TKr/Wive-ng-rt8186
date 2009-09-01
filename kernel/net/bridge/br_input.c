/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: br_input.c,v 1.16 2008/07/31 08:11:23 pluswang Exp $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_bridge.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"

#ifdef IGMP_SNOOPING
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/igmp.h>
#include <net/checksum.h>
extern int igmpsnoopenabled;



#ifdef	MCAST_TO_UNICAST
extern int IGMPProxyOpened;

#ifdef	IPV6_MCAST_TO_UNICAST
#include <linux/ipv6.h>
#include <linux/in6.h>
#include <linux/icmpv6.h>

//#define	DBG_ICMPv6	//enable it to debug icmpv6 check;plus note
static int ICMPv6_check(struct sk_buff *skb , unsigned char *gmac);
#endif	//end of IPV6_MCAST_TO_UNICAST

#endif

static char igmp_type_check(struct sk_buff *skb, unsigned char *gmac);

static void br_update_igmp_snoop_fdb(unsigned char op, struct net_bridge *br, struct net_bridge_port *p, unsigned char *gmac 
#ifdef	MCAST_TO_UNICAST
	, struct sk_buff *skb
#endif	

);

#endif	//end of IGMP_SNOOPING


unsigned char bridge_ula[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };
static int br_pass_frame_up_finish(struct sk_buff *skb)
{
	netif_rx(skb);

	return 0;
}

static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev;
#ifdef IGMP_SNOOPING
	unsigned char *dest;
	struct net_bridge_port *p;
	unsigned char macAddr[6];
	unsigned char operation;

	
                          
                                                                                                                           
	dest = skb->mac.ethernet->h_dest;
	p = skb->dev->br_port;
	if (igmpsnoopenabled && MULTICAST_MAC(dest) 
		&& (skb->mac.ethernet->h_proto == ETH_P_IP))
	{
		struct iphdr *iph;	
		iph = skb->nh.iph;
		unsigned char proto =  iph->protocol; 
       	if (proto== IPPROTO_IGMP){
			if ((operation=igmp_type_check(skb, macAddr)) > 0) {

				#ifdef	MCAST_TO_UNICAST
               	br_update_igmp_snoop_fdb(operation, br, p, macAddr,skb);
				#else
               	br_update_igmp_snoop_fdb(operation, br, p, macAddr);
				#endif
			}	
		}
	}
	#ifdef	MCAST_TO_UNICAST	
	#ifdef	IPV6_MCAST_TO_UNICAST
	else if(igmpsnoopenabled 
		&& IPV6_MULTICAST_MAC(dest) 
		&& (skb->mac.ethernet->h_proto == ETH_P_IPV6) )
	{		
		operation = ICMPv6_check(skb , macAddr);
		if (operation > 0) {
				#ifdef	DBG_ICMPv6
				if( operation == 1)
					printk("ICMPv6 mac add (from frame_up)\n");
				else if(operation == 2)
					printk("ICMPv6 mac del (from frame_up)\n");	
				#endif
               	br_update_igmp_snoop_fdb(operation, br, p, macAddr,skb);
		}
	}
	#endif	//end of IPV6_MCAST_TO_UNICAST
	#endif

#endif

	br->statistics.rx_packets++;
	br->statistics.rx_bytes += skb->len;

	indev = skb->dev;
	skb->dev = &br->dev;
	skb->pkt_type = PACKET_HOST;
	skb_push(skb, ETH_HLEN);
	skb->protocol = eth_type_trans(skb, &br->dev);
#if 0
	if (!(list_empty(&nf_hooks[(PF_BRIDGE)][(NF_BR_LOCAL_IN)])))
		printk("netfilter no empty in PF_BRIDGE\n");
#endif	

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
			br_pass_frame_up_finish);
}

static int br_handle_frame_finish(struct sk_buff *skb)
{
	struct net_bridge *br;
	unsigned char *dest;
	struct net_bridge_fdb_entry *dst;
	struct net_bridge_port *p;
	int passedup;

	dest = skb->mac.ethernet->h_dest;

	p = skb->dev->br_port;
	if (p == NULL)
		goto err_nolock;

	br = p->br;
	read_lock(&br->lock);
	if (skb->dev->br_port == NULL)
		goto err;

	passedup = 0;
	if (br->dev.flags & IFF_PROMISC) {
		struct sk_buff *skb2;

		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (skb2 != NULL) {
			passedup = 1;
			br_pass_frame_up(br, skb2);
		}
	}

	if (dest[0] & 1) {
#ifdef IGMP_SNOOPING
		struct iphdr *iph;
		unsigned int ipaddr=0;
		unsigned char proto=0;
		unsigned char reserved=0;
		unsigned char operation=0;
		
		iph = skb->nh.iph;
		ipaddr =  iph->daddr;
		proto  =  iph->protocol; 		
		
		if (!(br->dev.flags & IFF_PROMISC) 
			&& MULTICAST_MAC(dest) 
            && (skb->mac.ethernet->h_proto == ETH_P_IP))
        {
	        
			unsigned char macAddr1[6];
			
 
	       	if (proto == IPPROTO_IGMP) {
               	if ((operation=igmp_type_check(skb, macAddr1)) > 0){

					#ifdef	MCAST_TO_UNICAST
                   	br_update_igmp_snoop_fdb(operation, br, p, macAddr1,skb);
					#else
                   	br_update_igmp_snoop_fdb(operation, br, p, macAddr1);
					#endif

               	}
            }
	    }
		#ifdef	IPV6_MCAST_TO_UNICAST
		else if(!(br->dev.flags & IFF_PROMISC) 
				&& IPV6_MULTICAST_MAC(dest)
				&& (skb->mac.ethernet->h_proto == ETH_P_IPV6))
		{
				unsigned char macAddr2[6];				
				operation = ICMPv6_check(skb , macAddr2);
				if(operation > 0){
					#ifdef	DBG_ICMPv6
					if( operation == 1)
						printk("icmpv6 add from frame finish\n");
					else if(operation == 2)
						printk("icmpv6 del from frame finish\n");	
					#endif
					br_update_igmp_snoop_fdb(operation, br, p, macAddr2,skb);
				}
				
		}
		#endif		
	    //Brad disable the check 20080619
	    /*    
	    if((ipaddr&0xFFFFFF00)==0xE0000000){
        	reserved=1;
	    }
	    */
		if( igmpsnoopenabled && MULTICAST_MAC(dest) 			
			&& proto !=IPPROTO_IGMP && ipaddr != 0xEFFFFFFA 
			&& reserved ==0)
		{			
	        if ((dst = br_fdb_get(br, dest)) != NULL){
    	        br_multicast_forward(br, dst, skb, !passedup);
                br_fdb_put(dst);
            }else{
        	   	br_flood_forward(br, skb, !passedup);
            }
        }
        else { // broadcast
			br_flood_forward(br, skb, !passedup);
        }
#else
		br_flood_forward(br, skb, !passedup);
#endif	//end of IGMP_SNOOPING

		if (!passedup)
#ifdef MULTICAST_FILTER
		{
			if ((br->fltr_portlist_num) &&
				!memcmp(dest, "\x01\x00\x5e", 3))
			{
				int i, pass_up = 1;
				unsigned short frag_offset = *((unsigned short *)&(skb->data[6]));
				unsigned short port = *((unsigned short *)&(skb->data[22]));
				unsigned long x;

				if ((frag_offset & 0x1fff) == 0) {	// check fragment offset
					for (i=0; i<br->fltr_portlist_num; i++) {
						if (port == br->fltr_portlist[i]) {
							pass_up = 0;
							break;
						}
					}
				}

				x = dest[3] ^ dest[4] ^ dest[5];
				x = x & (MLCST_MAC_ENTRY - 1);

				if (pass_up) {
					if ((br->fltr_maclist[x][3] != 0) &&
						!memcmp(&(br->fltr_maclist[x][0]), &(dest[3]), 3))
						kfree_skb(skb);
					else
						br_pass_frame_up(br, skb);
				}
				else {
					kfree_skb(skb);
					if (br->fltr_maclist[x][3] == 0) {
						memcpy(&(br->fltr_maclist[x][0]), &(dest[3]), 3);
						br->fltr_maclist[x][3] = 1;
					}
				}
			}
			else
				br_pass_frame_up(br, skb);
		}
#else
			br_pass_frame_up(br, skb);
#endif
		goto out;
	}

	dst = br_fdb_get(br, dest);
	if (dst != NULL && dst->is_local) {
		if (!passedup)
			br_pass_frame_up(br, skb);
		else
			kfree_skb(skb);
		br_fdb_put(dst);
		goto out;
	}

	if (dst != NULL) {
		br_forward(dst->dst, skb);
		br_fdb_put(dst);
		goto out;
	}

	br_flood_forward(br, skb, 0);

out:
	read_unlock(&br->lock);
	return 0;

err:
	read_unlock(&br->lock);
err_nolock:
	kfree_skb(skb);
	return 0;
}

void br_handle_frame(struct sk_buff *skb)
{
	struct net_bridge *br;
	unsigned char *dest;
	struct net_bridge_port *p;

	dest = skb->mac.ethernet->h_dest;

	p = skb->dev->br_port;
	if (p == NULL)
		goto err_nolock;

	br = p->br;
	read_lock(&br->lock);
	if (skb->dev->br_port == NULL)
		goto err;

	if (!(br->dev.flags & IFF_UP) ||
	    p->state == BR_STATE_DISABLED)
		goto err;

	if (skb->mac.ethernet->h_source[0] & 1)
		goto err;

#ifdef RTL_BRIDGE_MAC_CLONE
	if (br_mac_clone_handle_frame(br, p, skb) == -1)
		goto err;
#endif
	/* Pause frames shouldn't be passed up by driver anyway */
	if (skb->protocol == htons(ETH_P_PAUSE))
                       goto err_nolock;

	if (p->state == BR_STATE_LEARNING ||
	    p->state == BR_STATE_FORWARDING)
		br_fdb_insert(br, p, skb->mac.ethernet->h_source, 0);

	if (br->stp_enabled &&
	    !memcmp(dest, bridge_ula, 5) &&
	    !(dest[5] & 0xF0))
		goto handle_special_frame;

	if (p->state == BR_STATE_FORWARDING) {
	
	if (!compare_ether_addr(eth_hdr(skb)->h_dest,skb->dev->dev_addr))
                skb->pkt_type = PACKET_HOST;
                                        
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
                if(eth_hdr(skb)->h_proto == htons(ETH_P_8021Q)) // Block vlan packet
                    {
                        read_unlock(p->br->lock);
			return 0;                       // Forward to normal path
                    }
#endif
                                                                                                
		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		read_unlock(&br->lock);
		return;
	}

err:
	read_unlock(&br->lock);
err_nolock:
	kfree_skb(skb);
	return;

handle_special_frame:
	if (!dest[5]) {
		br_stp_handle_bpdu(skb);
		return;
	}

	kfree_skb(skb);
}



#ifdef IGMP_SNOOPING
#ifdef	IPV6_MCAST_TO_UNICAST
/*Convert  MultiCatst IPV6_Addr to MAC_Addr*/
static void CIPV6toMac
	(unsigned char* icmpv6_McastAddr, unsigned char *gmac )
{
	/*ICMPv6 valid addr 2^32 -1*/
	gmac[0] = 0x33;
	gmac[1] = 0x33;
	gmac[2] = icmpv6_McastAddr[12];
	gmac[3] = icmpv6_McastAddr[13];
	gmac[4] = icmpv6_McastAddr[14];
	gmac[5] = icmpv6_McastAddr[15];			
}



static int ICMPv6_check(struct sk_buff *skb , unsigned char *gmac)
{
	
	struct ipv6hdr *ipv6h;
	char* protoType;	
	
	/* check IPv6 header information */
	ipv6h = skb->nh.ipv6h;
	if(ipv6h->version != 6){	
		//printk("ipv6h->version != 6\n");
		return -1;
	}


	/*Next header: IPv6 hop-by-hop option (0x00)*/
	if(ipv6h->nexthdr == 0)	{
		protoType = (unsigned char*)( (unsigned char*)ipv6h + sizeof(struct ipv6hdr) );	
	}else{
		//printk("ipv6h->nexthdr != 0\n");
		return -1;
	}

	if(protoType[0] == 0x3a){
		
		//printk("recv icmpv6 packet\n");
		struct icmp6hdr* icmpv6h = (struct icmp6hdr*)(protoType + 8);
		unsigned char* icmpv6_McastAddr ;
	
		if(icmpv6h->icmp6_type == 0x83){
			
			icmpv6_McastAddr = (unsigned char*)((unsigned char*)icmpv6h + 8);
			#ifdef	DBG_ICMPv6					
			printk("Type: 0x%x (Multicast listener report) \n",icmpv6h->icmp6_type);
			#endif

		}else if(icmpv6h->icmp6_type == 0x8f){		
		
			icmpv6_McastAddr = (unsigned char*)((unsigned char*)icmpv6h + 8 + 4);
			#ifdef	DBG_ICMPv6					
			printk("Type: 0x%x (Multicast listener report v2) \n",icmpv6h->icmp6_type);
			#endif			
		}else if(icmpv6h->icmp6_type == 0x84){
		
			icmpv6_McastAddr = (unsigned char*)((unsigned char*)icmpv6h + 8 );			
			#ifdef	DBG_ICMPv6					
			printk("Type: 0x%x (Multicast listener done ) \n",icmpv6h->icmp6_type);
			#endif			
		}
		else{
			#ifdef	DBG_ICMPv6
			printk("Type: 0x%x (unknow type)\n",icmpv6h->icmp6_type);
			#endif			
			return -1;
		}				

		#ifdef	DBG_ICMPv6			
		printk("MCAST_IPV6Addr:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x \n",
			icmpv6_McastAddr[0],icmpv6_McastAddr[1],icmpv6_McastAddr[2],icmpv6_McastAddr[3],
			icmpv6_McastAddr[4],icmpv6_McastAddr[5],icmpv6_McastAddr[6],icmpv6_McastAddr[7],
			icmpv6_McastAddr[8],icmpv6_McastAddr[9],icmpv6_McastAddr[10],icmpv6_McastAddr[11],
			icmpv6_McastAddr[12],icmpv6_McastAddr[13],icmpv6_McastAddr[14],icmpv6_McastAddr[15]);
		#endif

		CIPV6toMac(icmpv6_McastAddr, gmac);
		
		#ifdef	DBG_ICMPv6					
		printk("group_mac [%02x:%02x:%02x:%02x:%02x:%02x] \n",
			gmac[0],gmac[1],gmac[2],
			gmac[3],gmac[4],gmac[5]);
		#endif
			


		if(icmpv6h->icmp6_type == 0x83){

			return 1;//icmpv6 listener report (add)
		}
		else if(icmpv6h->icmp6_type == 0x8f){
			return 1;//icmpv6 listener report v2 (add) 
		}
		else if(icmpv6h->icmp6_type == 0x84){
			return 2;//icmpv6 Multicast listener done (del)
		}
	}		
	else{
		//printk("protoType[0] != 0x3a\n");		
		return -1;//not icmpv6 type
	}
		

}

#endif	//end of IPV6_MCAST_TO_UNICAST
static void ConvertMulticatIPtoMacAddr(__u32 group, unsigned char *gmac)
{
	//printk("ConvertMulticatIPtoMacAddr:group:%04X\n",group);//plus for trace
	__u32 u32tmp, tmp;
	int i;

	u32tmp = group & 0x007FFFFF;
	gmac[0]=0x01; gmac[1]=0x00; gmac[2]=0x5e;
	for (i=5; i>=3; i--) {
		tmp=u32tmp&0xFF;
		gmac[i]=tmp;
		u32tmp >>= 8;
	}
}
static char igmp_type_check(struct sk_buff *skb, unsigned char *gmac)
{
	
	struct iphdr *iph;
	__u8 hdrlen;
	struct igmphdr *igmph;
	
#ifdef	MCAST_TO_UNICAST	
	unsigned int IGMP_Group;// add  for fit igmp v3
#endif
	
	/* check IP header information */
	iph = skb->nh.iph;
	hdrlen = iph->ihl << 2;
	if ((iph->version != 4) &&  (hdrlen < 20)){		
		return -1;
	}
	
	if (ip_fast_csum((u8 *)iph, iph->ihl) != 0){
		return -1;
	}
	
	{ /* check the length */
	__u32 len = ntohs(iph->tot_len);
	if (skb->len < len || len < hdrlen){
		return -1; 
		}
	}
	/* parsing the igmp packet */
	igmph = (struct igmphdr *)((u8*)iph+hdrlen);

#ifdef	MCAST_TO_UNICAST
	//IGMP-V3 type Report
	if(igmph->type == IGMPV3_HOST_MEMBERSHIP_REPORT)
	{
		if(IGMPProxyOpened==0){
			IGMP_Group = *(unsigned int*)((unsigned int*)igmph + 3);
			//printk("v3_group:%02X:%02X:%02X:%02X\n",
			//IGMP_Group>>24,(IGMP_Group<<8)>>24,(IGMP_Group<<16)>>24,(IGMP_Group<<24)>>24);
		}else{
			return -1;//don't care v3 report
		}
		
	}else{	//4 V2 or V1
		//printk("igmph->group:%04X\n",igmph->group);	
		IGMP_Group = igmph->group;
	}
#endif
#ifdef	MCAST_TO_UNICAST

	/*check if it's protocol reserved group */
	if(!IN_MULTICAST(IGMP_Group))
	{			
			return -1;
	}
	//Brad disable the check 20080619
	/*
	if((IGMP_Group&0xFFFFFF00)==0xE0000000){			
			return -1;
	}
	*/
	ConvertMulticatIPtoMacAddr(IGMP_Group, gmac);
	
#else
	/*check if it's protocol reserved group */
	if(!IN_MULTICAST(igmph->group))
	{			
			return -1;
	}
	//Brad disable the check 20080619
	/*
	if((igmph->group&0xFFFFFF00)==0xE0000000){			
			return -1;
	}
	*/
	ConvertMulticatIPtoMacAddr(igmph->group, gmac);
#endif



	
	if (
#ifdef	MCAST_TO_UNICAST		
		(igmph->type==IGMPV3_HOST_MEMBERSHIP_REPORT)||	//for support igmp v3
#endif		
		(igmph->type==IGMP_HOST_MEMBERSHIP_REPORT) ||
	    (igmph->type==IGMP_HOST_NEW_MEMBERSHIP_REPORT)) 
	{

		return 1; /* report and add it */
	}else if (igmph->type==IGMP_HOST_LEAVE_MESSAGE){
	
		return 2; /* leave and delete it */

	}	
	
	//printk("exit igmp: neither Report nor Leave\n");//plus test
	
	return -1;
}


static void br_update_igmp_snoop_fdb(unsigned char op, struct net_bridge *br, struct net_bridge_port *p, unsigned char *dest 
#ifdef	MCAST_TO_UNICAST
, struct sk_buff *skb
#endif
)
{

#ifdef	MCAST_TO_UNICAST
	if(!dest)	return;
	
	if( !MULTICAST_MAC(dest) 
#ifdef	IPV6_MCAST_TO_UNICAST
		&& !IPV6_MULTICAST_MAC(dest)
#endif		
	   ){ return; }
#endif

	struct net_bridge_fdb_entry *dst;
//	unsigned char i=0;
	unsigned char exist=0;
	unsigned short del_group_src=0;
	/* check whether entry exist */
	dst = br_fdb_get(br, dest);
	if (dst != NULL) {
		if((dst->group_src & (1<< p->port_no))== (1 << p->port_no)){
				exist = 1;
				//printk("fdb rdy exist!\n");//pluswang for trace 0317-2008				
		}		
	
	}
	if (op == 1) /* add */
	{
#ifdef	MCAST_TO_UNICAST
			//TODO maybe add more wlan interface,now just try,plus

			if (!strcmp(p->dev->name, "wlan0"))
			{				
				unsigned char StaMacAndGroup[12];
				memcpy(StaMacAndGroup , dest , 6);
				memcpy(StaMacAndGroup+6 , skb->mac.ethernet->h_source,6);				
				if(p->dev->do_ioctl != NULL){
					p->dev->do_ioctl(p->dev, (struct ifreq*)StaMacAndGroup, 0x8B80);//call AddMCASTGroup2STA()				
				}
			}
#endif	
		if (dst) {
			if (exist == 0)	{
       	        dst->group_src = dst->group_src | (1 << p->port_no);
			}
			
			// update the timer to prevent from timeout
			dst->ageing_timer = jiffies;
		}
		else {
			/* insert one fdb entry */
			br_fdb_insert(br, p, dest, 0);			
			dst = br_fdb_get(br, dest);
			
			
			if(dst !=NULL)
				dst->group_src = dst->group_src | (1 << p->port_no);
		}
	}
	else if (op == 2) /* delete */
	{
#ifdef	MCAST_TO_UNICAST
			//TODO maybe add more wlan interface,now just try,plus
			if (!strcmp(p->dev->name, "wlan0"))
			{
				unsigned char StaMacAndGroup[12];
				memcpy(StaMacAndGroup , dest , 6);
				memcpy(StaMacAndGroup+6 , skb->mac.ethernet->h_source,6);				
				if(p->dev->do_ioctl != NULL){
					p->dev->do_ioctl(p->dev, (struct ifreq*)StaMacAndGroup, 0x8B81);//call DelMCASTGroup2STA()
				}
			}
#endif		
		if (dst && exist){
			del_group_src = ~(1 << p->port_no);
			dst->group_src = dst->group_src & del_group_src;
			//printk("exist and delete it\n");//plus 0313 test			
		}
	}
}

#endif // IGMP_SNOOPING
