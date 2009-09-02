/* -*- linux-c -*-
#######################################################################
#
# (C) Copyright 2001-2003
# Alex Zeffertt, Cambridge Broadband Ltd, ajz@cambridgebroadband.com
# Re-worked by Ben Greear <greearb@candelatech.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#######################################################################
# Notes:
# 
# This file implements the macvlan.o MAC address based VLAN support 
# module.
#
# This provides an IOCTL interface which allows you to
# It uses an IOCTL interface which allows you to
#
# 1. enable/disable MAC address based VLANS over an ether type net_device
# 2. add/remove a MAC address based VLAN - which is an ether type net_device
#    layered over the original MACVLAN enabled ether type net_device.
# 3. bind/unbind MAC addresses to/from particular MAC address based VLANs
# 4. discover the state of MAC address based VLANs on the system.
# 5. set/get port flags, including whether to bind to destination MAC
#    or source mac.
# 6. Traffic to/from eth0 will not be affected.

# Example: (Assuming you are using source binding)
#
# If you enable MAC address based VLANS over eth0
#
# You may then create further VLANs, e.g. eth0#1 eth0#2 ....
# These will not receive any frames until you bind MAC addresses to them.
# If you bind 11:22:33:44:55:66 to eth0#1, then any frames received by
# eth0 with source MAC 11:22:33:44:55:66 will be routed up through eth0#1
# instead of eth0.
#
# Example: (Assuming you are using destination (local) binding)
#
# If you enable MAC address based VLANS over eth0
#
# You may then create further VLANs, e.g. eth0#1 eth0#2 ....
# These will not receive any frames until you bind MAC addresses to them.
# If you bind 11:22:33:44:55:66 to eth0#1, then any broadcast/multicast
# frames, or frames with a destination MAC 11:22:33:44:55:66
# will be routed up through eth0#1 instead of eth0
#
# For broadcasts, the packet will be duplicated for every VLAN
# with at least one MAC attached.  Attaching more than one MAC
# when destination binding makes no sense...don't do it!
#
# 
#######################################################################
*/
#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h> 
#include <linux/fs.h>	  
#include <linux/errno.h>  
#include <linux/delay.h>  
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/tqueue.h>
#include <linux/poll.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/if_macvlan.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
#include <net/arp.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#define MVL_PROC_DIR "macvlan"
#define MVL_PROC_CFG "config"
#define PORT_CFG_FILE_NAME "config"
static struct proc_dir_entry *mvl_proc_dir;
static struct proc_dir_entry *mvl_proc_cfg;
#endif

#include "macvlan.h"


/*********************************************************/
/*			defines				 */
/*********************************************************/

#if 0
#define DEBUG(format,args...) printk(KERN_ERR format, ##args);
#else
#define DEBUG(format,args...)
#endif


#undef MVL_USE_RW_LOCKS
#ifdef MVL_USE_RW_LOCKS
/*  Must hold this lock to make any changes to the macvlan structures.
 */
static rwlock_t mvl_cfg_lock = RW_LOCK_UNLOCKED;

#define MVL_READ_LOCK /* printk("%i: read-lock port list\n", __LINE__); */ \
                            BUG_ON(in_interrupt()); \
			    read_lock(&mvl_cfg_lock);
#define MVL_READ_UNLOCK /* printk("%i: read-unlock port list\n", __LINE__); */ \
                            BUG_ON(in_interrupt()); \
			    read_unlock(&mvl_cfg_lock);

#define MVL_WRITE_LOCK /* printk("%i: write-lock port list\n", __LINE__); */ \
                            BUG_ON(in_interrupt()); \
			    write_lock(&mvl_cfg_lock);
#define MVL_WRITE_UNLOCK /* printk("%i: write-unlock port list\n", __LINE__); */ \
                            BUG_ON(in_interrupt()); \
			    write_unlock(&mvl_cfg_lock);


#define MVL_IRQ_RLOCK(a) /* printk("%i: read-unlock port list\n", __LINE__); */ { \
                            __u64 now = getCurUs(); \
	                    __u64 later; \
			    read_lock_irqsave(&mvl_cfg_lock, a); \
                            later = getCurUs(); \
                            if ((later - now) > 100) { \
	                       printk("took: %lluus to acquire read lock, line: %i\n", \
				      later - now, __LINE__); \
                            }}

#define MVL_IRQ_RUNLOCK(a) /* printk("%i: read-unlock port list\n", __LINE__); */ \
			    read_unlock_irqrestore(&mvl_cfg_lock, a);
#else
/*  Must hold this lock to make any changes to the macvlan structures.
 */
static spinlock_t mvl_cfg_lock = SPIN_LOCK_UNLOCKED;

#define MVL_READ_LOCK(a) MVL_WRITE_LOCK(a)
#define MVL_READ_UNLOCK(a) MVL_WRITE_UNLOCK(a)

#define MVL_WRITE_LOCK(a) /* printk("%i: write-lock port list\n", __LINE__); */ \
		           spin_lock_irqsave(&mvl_cfg_lock, a);
#define MVL_WRITE_UNLOCK(a) /* printk("%i: write-unlock port list\n", __LINE__); */ \
                           spin_unlock_irqrestore(&mvl_cfg_lock, a); \


#define MVL_IRQ_RLOCK(a) /* printk("%i: read-unlock port list\n", __LINE__); */ \
			    spin_lock_irqsave(&mvl_cfg_lock, a); \

#define MVL_IRQ_RUNLOCK(a) /* printk("%i: read-unlock port list\n", __LINE__); */ \
			    spin_unlock_irqrestore(&mvl_cfg_lock, a);
#endif


/*********************************************************/
/*		       file scope variables		 */
/*********************************************************/

static struct macvlan_port *port_list = NULL;

static atomic_t macvlan_nports;
static atomic_t mvl_vlan_counter;

static int debug_lvl = 0;


/*********************************************************/
/*		   forward declarations			 */
/*********************************************************/
static int macvlan_hash_rem(const char* vlan_ifname,
			    const unsigned char* mac);

/*********************************************************/
/*		   function definitions			 */
/*********************************************************/

/** Convert to micro-seconds */
static inline __u64 tv_to_us(const struct timeval* tv) {
        __u64 us = tv->tv_usec;
        us += (__u64)tv->tv_sec * (__u64)1000000;
        return us;
}


/* Since the epoc.  More precise over long periods of time than
 * getRelativeCurMs
 */
static inline __u64 getCurUs(void) {
        struct timeval tv;
        do_gettimeofday(&tv);
        return tv_to_us(&tv);
}


char toupper(char in) {
	if ((in >= 'a') && (in <= 'z')) {
		in -= ('a' - 'A');
	}
	return in;
}

#define iswhitespace(x)\
	((x) == ' ' || (x) == '\n' || (x) == '\r' || (x) == '\r' )

#define skip_whitespace(x) 	{ while (iswhitespace(*x)) (x)++; }

static int copy_next_word(char *dst, char *src, int len) {
	char *p;
	for (p=src; p < src + len ; p++) {
		if ( iswhitespace(*p))
			break;
		*dst++ = *p;
	}
	return p - src;
}


static int toMacString(unsigned char* rslt_mac, const char* raw_mac) {
	// Turn HEX into bytes.  First, gather all the useful HEX
	char tmp[12]; //More than 12 is useless, at least right now
	char c;
	int j = 0; //tmp's index.
	int i;
	char tmp_bt[3];
	for (i = 0; i<strlen(raw_mac); i++) {
		c = toupper(raw_mac[i]);
		if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F'))) {
			tmp[j] = c;
			//VLOG_ERR(VLOG << " c: " << c << endl);
			if (j == 11) {
				break; //done
			}
			j++;
		}
		else {
			if ((c == ':') || (c == ' ') || (c == '.')) {
				// Ok, valid divider
			}
			else {
				// Invalid header
				return -EINVAL;
			}
		}
	}
	
	if (j != 11) {
		//msg->append("ERROR:  Not enough HEX values in the input string.\n");
		return -EINVAL;
	}
	
	for (i = 0; i<6; i++) {
		tmp_bt[0] = tmp[i*2];
		tmp_bt[1] = tmp[i*2 +1];
		tmp_bt[2] = 0;
		//VLOG_ERR(VLOG << " tmp_bt -:" << tmp_bt << ":- i: " << i << endl);
		rslt_mac[i] = (unsigned char)(simple_strtol(tmp_bt, NULL, 16) & 0xFF);
		//VLOG_ERR(VLOG << " rslt_mac[" << i << "]  -:" << rslt_mac[i] << ":-\n");
	}
	return 0;
}//toMacString


struct macvlan_vlan* macvlan_find_vlan_in_port(struct macvlan_port* port,
					       const char* ifname) {
	struct macvlan_vlan* vlan;
	for (vlan = port->vlan_list; vlan; vlan = vlan->next) {
		if (!strcmp(vlan->dev->name, ifname)) {
			return vlan;
		}
	}
	return NULL;
}	  


/* Find port by mac-vlan interface name (eth1#777) */
struct macvlan_port* macvlan_find_port_for_mvlan_ifname(const char* ifname) {
	struct macvlan_port* port;
	for (port = port_list; port; port = port->next) {
		if (macvlan_find_vlan_in_port(port, ifname)) {
			break;
		}
	}
	return port;
}

struct macvlan_port* macvlan_find_port_for_underlying_ifname(const char* ifname) {
	struct macvlan_port* port;
	//printk("finding port for underlying ifname: %s\n", ifname);
	for (port = port_list; port; port = port->next) {
		//printk("Testing port: %p name: %s\n", port, port->dev->name);
		if (strcmp(port->dev->name, ifname) == 0) {
			break;
		}
	}
	//printk("done finding port: %p\n", port);
	return port;
}	 

/*
 *	Rebuild the Ethernet MAC header. This is called after an ARP
 *	(or in future other address resolution) has completed on this
 *	sk_buff. We now let ARP fill in the other fields.
 *
 *	This routine CANNOT use cached dst->neigh!
 *	Really, it is used only when dst->neigh is wrong.
 *
 */
int macvlan_dev_rebuild_header(struct sk_buff *skb) {
	struct net_device *dev = skb->dev;
	struct ethhdr *veth = (struct ethhdr *)(skb->data);

	switch (veth->h_proto) {
#ifdef CONFIG_INET
	case __constant_htons(ETH_P_IP):

		return arp_find(veth->h_dest, skb);
#endif	
	default:
		DEBUG("%s: unable to resolve type %X addresses.\n", 
		      dev->name, (int)veth->h_proto);
	 
		memcpy(veth->h_source, dev->dev_addr, ETH_ALEN);
		break;
	};

	return 0;
}



static struct net_device_stats *macvlan_get_stats(struct net_device *dev)
{
	struct macvlan_vlan *vlan = dev->priv;

	return &vlan->statistics;
}

static int macvlan_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct macvlan_vlan *vlan = dev->priv;
	DEBUG("%s: \n", __PRETTY_FUNCTION__);
	vlan->statistics.tx_packets++;
	vlan->statistics.tx_bytes += skb->len;

	skb->dev = vlan->lowerdev;
	dev_queue_xmit(skb);
	return 0;
}

static int macvlan_open(struct net_device *dev)
{
	MOD_INC_USE_COUNT;
	netif_start_queue(dev);
	return 0;
}

static void macvlan_set_multicast_list(struct net_device *dev)
{
	/* TODO ??? */
}

static int macvlan_stop(struct net_device *dev)
{
	netif_stop_queue(dev);
	MOD_DEC_USE_COUNT;
	return 0;
}

static int macvlan_accept_fastpath(struct net_device *dev, struct dst_entry *dst)
{
	return -1;
}


/*
 *	Create the VLAN header for an arbitrary protocol layer 
 *
 *	saddr=NULL	means use device source address
 *	daddr=NULL	means leave destination address (eg unresolved arp)
 *
 *  This is called when the SKB is moving down the stack towards the
 *  physical devices.
 */
int macvlan_hard_header(struct sk_buff *skb, struct net_device *dev,
			unsigned short type, void *daddr, void *saddr,
			unsigned len)
{
	struct macvlan_vlan *vlan = dev->priv;
	
	DEBUG("%s: \n", __PRETTY_FUNCTION__);

	/* Before delegating work to the lower layer, enter our MAC-address */
	saddr = dev->dev_addr;

	dev = vlan->lowerdev;

	/* Now make the underlying real hard header */
	return dev->hard_header(skb, dev, type, daddr, saddr, len);
}


void macvlan_dev_destructor(struct net_device *dev) {
	atomic_dec(&mvl_vlan_counter);
	if (dev->priv) {
		//printk("dst: %s", dev->name);
		kfree(dev->priv);
		dev->priv = NULL;
	}
	else {
		//printk("dst2: %s", dev->name);
	}
}


static int macvlan_vlan_create(const char* port_name, int newifidx) {
	struct macvlan_vlan *vlan = NULL;
	struct macvlan_port* port;
	char newifname[IFNAMSIZ+1];
	struct net_device* td = NULL;
	unsigned long flags;
	int rv;
	
	MVL_WRITE_LOCK(flags);

	//printk("--*-- ");
	/* find the port to which ifname belongs */
	port = macvlan_find_port_for_underlying_ifname(port_name);
	if (!port) {
		MVL_WRITE_UNLOCK(flags);
		rv = -ENODEV;
		goto unlockout;
	}

	BUG_ON(!port->dev);

	//printk("1 ");
	if (newifidx < 0) {
		/* Find the next free index */
		int i;
		for (i = 0; i<MAX_MACVLANS_PER_PORT; i++) {
			snprintf(newifname, IFNAMSIZ, "%s#%d", port->dev->name, i);
			newifname[IFNAMSIZ] = 0;
			if ((td = dev_get_by_name(newifname)) == NULL) {
				newifidx = i;
				break;
			}
			dev_put(td);
		}

		if (newifidx < 0) {
			printk("macvlan: Could not find a free index, reached max: %i\n", i);
		}
	}
	
	//printk("2 ");
	/* generate a name for the new vlan */
	snprintf(newifname, IFNAMSIZ, "%s#%d", port->dev->name, newifidx);
	newifname[IFNAMSIZ] = 0;

	if ((td = dev_get_by_name(newifname)) != NULL) {
		DEBUG("macvlan: vlan by that name already exists\n");
		dev_put(td);
		rv = -EEXIST;
		goto unlockout;
	}

	//printk("3 ");
	if ((vlan = kmalloc(sizeof(*vlan), GFP_KERNEL)) == NULL) {
		DEBUG("macvlan: kmalloc failure\n");
		rv = -ENOMEM;
		goto unlockout;
	}

	memset(vlan, 0, sizeof(*vlan));

	//printk("4 ");
	if ((vlan->dev = kmalloc(sizeof(struct net_device), GFP_KERNEL)) == NULL) {
		rv = -ENOMEM;
		kfree(vlan);
		goto unlockout;
	}
	memset(vlan->dev, 0, sizeof(struct net_device));
	
	//printk("5 ");
	strcpy(vlan->dev->name, newifname);
	ether_setup(vlan->dev);
	
	dev_hold(vlan->dev); /* MVL code holds reference */
	
	vlan->dev->priv = vlan;
	vlan->port = port;
	vlan->lowerdev = port->dev;

	//printk("6 ");
	/* dev->do_ioctl = macvlan_do_ioctl; */
	vlan->dev->get_stats = macvlan_get_stats;
	vlan->dev->hard_start_xmit = macvlan_xmit;
	vlan->dev->hard_header = macvlan_hard_header;
	vlan->dev->rebuild_header = macvlan_dev_rebuild_header;
	vlan->dev->open = macvlan_open;
	vlan->dev->set_multicast_list = macvlan_set_multicast_list;
	vlan->dev->stop = macvlan_stop;
	vlan->dev->accept_fastpath = macvlan_accept_fastpath;
	vlan->dev->tx_queue_len = 0;
	vlan->dev->set_mac_address = NULL;
	vlan->dev->priv = vlan;
	vlan->dev->destructor = macvlan_dev_destructor;
	
	/* This will change if you are using Destination (local) binding,
	 * when you add a MAC to it..
	 */
	memcpy(vlan->dev->dev_addr, vlan->lowerdev->dev_addr, ETH_ALEN);

	DEBUG("macvlan: created vlan %p\n", vlan);

#ifdef MVL_CONFIG_PROC_FS
	//printk("7 ");
	if (vlan->port->proc_dir) {
		vlan->proc_ent = create_proc_read_entry(vlan->dev->name, S_IRUGO,
							vlan->port->proc_dir,
							read_mvl, vlan);
		if (!vlan->proc_ent) {
			printk("ERROR:  Could not create proc entry for device: %s\n",
			       vlan->dev->name);
		}
		else {
			vlan->proc_ent->write_proc = write_mvl;
		}
	}
#endif

	atomic_inc(&port->ndevs);
	
	/* link to list */
	//printk("8 ");
	vlan->next = port->vlan_list;
	port->vlan_list = vlan;

	//printk("End of mac_vlan create1, ref-cnt: %i\n", atomic_read(&dev->refcnt));
	
	MVL_WRITE_UNLOCK(flags);
	register_netdev(vlan->dev);

	//printk("End of mac_vlan create2, ref-cnt: %i\n", atomic_read(&dev->refcnt));

	atomic_inc(&mvl_vlan_counter);
	//printk("9\n");
	rv = 0;
	goto out;

 unlockout:
	MVL_WRITE_UNLOCK(flags);
 out:
	return rv;
} /* macvlan_vlan_create */


/* Has locking internally */
int macvlan_vlan_cleanup(const char* ifname) {
	int i;
	struct macvlan_port* port;
	struct macvlan_vlan* vlan;
	struct macvlan_vlan* walker;
	struct macvlan_vlan* prev;
	unsigned long flags;
	int rv;
	
	DEBUG(__FUNCTION__"(%p)\n",vlan);
	//printk("mvl_cln: %s", ifname);

	MVL_WRITE_LOCK(flags);
	/* NOTE:  Cannot depend on device name, it can be changed. --Ben */
	port = macvlan_find_port_for_mvlan_ifname(ifname);
	if (!port) {
		rv = -ENODEV;
		goto unlockout;
	}

	//printk("1 ");
	vlan = macvlan_find_vlan_in_port(port, ifname);
	BUG_ON(!vlan);

	if (vlan->dev->flags & IFF_UP) {
		rv = -EBUSY;
		goto unlockout;
	}

	//printk("2 ");
	for (i = 0; i<MACVLAN_HASH_LEN; i++) {
		struct macvlan_hash_entry* tmp = vlan->port->hash_table[i];
		struct macvlan_hash_entry* prev = NULL;
		while (tmp) {
			if (tmp->vlan == vlan) {
				if (prev) {
					prev->next = tmp->next;
					kfree(tmp);
					tmp = prev->next;
				}
				else {
					vlan->port->hash_table[i] = tmp->next;
					kfree(tmp);
					tmp = vlan->port->hash_table[i];
				}
			}
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
	}/* for all hash buckets */
	//printk("3 ");

#ifdef MVL_CONFIG_PROC_FS
	if (vlan->proc_ent) {
		remove_proc_entry(vlan->dev->name, vlan->port->proc_dir);
		vlan->proc_ent = NULL;
	}
#endif


	/*
	 * remove the vlan in question from the list
	 */
	prev = NULL;
	walker = port->vlan_list;
	while (walker) {
		if (walker == vlan) {
			if (prev) {
				prev->next = walker->next;
			}
			else {
				port->vlan_list = walker->next;
			}
			break;
		}
		prev = walker;
		walker = walker->next;
	}/* while */
	BUG_ON(walker != vlan);
	
	atomic_dec(&port->ndevs);

	//printk("4 ");
	//printk("End of mac_vlan cleanup1, ref-cnt: %i\n", atomic_read(&vlan->dev->refcnt));
	dev_put(vlan->dev);

	MVL_WRITE_UNLOCK(flags);

	//printk("End of mac_vlan cleanup2, ref-cnt: %i\n", atomic_read(&vlan->dev->refcnt));
	unregister_netdev(vlan->dev);
	
	/* VLAN will be deleted when the device is deleted */
	
	//printk("5 ");
	rv = 0;
	goto out;

 unlockout:
	MVL_WRITE_UNLOCK(flags);

 out:
	return rv;
	
} /* mac_vlan cleanup */



static int macvlan_port_set_flags(const char* ifname, int flags) {
	struct macvlan_port *port;
			
	/* find the port to which ifname belongs */
	port = macvlan_find_port_for_underlying_ifname(ifname);
	if (!port) {
		return -ENODEV;
	}
	else {
		port->flags = flags;
	}
	return 0;
}/* macvlan_port_set_flags */

static int macvlan_port_create(const char* ifname) {
	struct macvlan_port *port;
	struct net_device* dev;
	
	port = macvlan_find_port_for_underlying_ifname(ifname);
	if (port != NULL) {
		return -EEXIST;
	}
		
	dev = dev_get_by_name(ifname);
	if (dev == NULL) {
		return -ENODEV;
	}

	if ((dev->macvlan_priv != NULL)
	    || (dev->flags & IFF_LOOPBACK)
	    || (dev->type != ARPHRD_ETHER)) {
		printk("macvlan: lower layer failed"
		      " dev->macvlan_priv=%p dev->flags=%08x dev->type=%08x\n",
		      dev->macvlan_priv, dev->flags, dev->type);
		dev_put(dev);
		return -EINVAL;
	}

	if ((port = kmalloc(sizeof(*port), GFP_KERNEL)) == NULL) {
		dev_put(dev);
		return -ENOBUFS;
	}

	memset(port, 0, sizeof(*port));
	port->dev = dev;

	/* TODO:  Could use multicast filters in some NICs at least. */
	dev_set_promiscuity(dev, 1);
	dev->macvlan_priv = port;

#ifdef MVL_CONFIG_PROC_FS
	if (mvl_proc_dir) {
		port->proc_dir = proc_mkdir(port->dev->name, mvl_proc_dir);

		if (port->proc_dir) {
			port->proc_ent = create_proc_read_entry(PORT_CFG_FILE_NAME, S_IRUGO,
								port->proc_dir,
								read_mvl_port, port);
			if (port->proc_ent) {
				port->proc_ent->write_proc = write_mvl_port;
			}
			else {
				printk("macvlan: ERROR: failed to create proc entry for port: %s\n",
				       port->dev->name);
			}
		}
	}
#endif

	atomic_inc(&macvlan_nports);
	
	/* Link into our list */
	port->next = port_list;
	port_list = port;
	
	DEBUG("macvlan: created port=%p\n", port);
	return 0;
}/* macvlan_port_create */


/* Clears all memory, kfree's it if possible.
 */
static int macvlan_port_cleanup(const char* ifname) {
	struct macvlan_port *port;
	struct macvlan_port *prev;
	struct macvlan_port *walker;
	int i;
	
	port = macvlan_find_port_for_underlying_ifname(ifname);
	if (!port) {
		return -ENODEV;
	}

	if (port->vlan_list) {
		return -EBUSY;
	}

	/* hash table should be empty at this point */
	for (i = 0 ; i < MACVLAN_HASH_LEN; i++) {
		BUG_ON(port->hash_table[i]);
	}

	/* Remove from our port list */
	prev = NULL;
	walker = port_list;
	while (walker) {
		if (walker == port) {
			if (prev) {
				prev->next = walker->next;
			}
			else {
				port_list = walker->next;
			}
			break;
		}
		prev = walker;
		walker = walker->next;
	}
	BUG_ON(walker != port);
	
	
#ifdef MVL_CONFIG_PROC_FS
	if (port->proc_dir) {
		if (port->proc_ent) {
			remove_proc_entry(PORT_CFG_FILE_NAME, port->proc_dir);
			port->proc_ent = NULL;
		}
		
		remove_proc_entry(port->dev->name, mvl_proc_dir);
		port->proc_dir = NULL;
	}
#endif
	
	dev_set_promiscuity(port->dev, -1);
	port->dev->macvlan_priv = NULL;
	dev_put(port->dev);

	atomic_dec(&macvlan_nports);

	kfree(port);
	
	return 0;
}/* macvlan_port_cleanup */


static inline struct macvlan_vlan *macvlan_hash_lookup(struct macvlan_port *port,
						       const unsigned char *src) {
	/* 
	 * The hashing function is to simply
	 * take the bottom source address byte
	 */
	struct macvlan_hash_entry *entry;
	unsigned int bucket = VLAN_BUCKET(src);
	for (entry = port->hash_table[bucket]; entry; entry = entry->next) {
		if (memcmp(entry->mac, src, ETH_ALEN) == 0) {
			/*DEBUG("macvlan: matched %02x:%02x:%02x:%02x:%02x:%02x to vlan %p\n", 
			  src[0],src[1],src[2],src[3],src[4],src[5],entry->vlan); */
			return entry->vlan;
		}
	}
	return NULL;
}


static int macvlan_hash_add(const char* ifname,
			    const unsigned char* macaddr) {
	
	struct macvlan_port *port;
	struct macvlan_vlan *vlan;
	unsigned int bucket = VLAN_BUCKET(macaddr);
	struct macvlan_hash_entry* entry;
	

	/* find the port in question */
	port = macvlan_find_port_for_mvlan_ifname(ifname);
	if (!port) {
		return -ENODEV;
	}
	    
	/* find the vlan layered over this port */
	vlan = macvlan_find_vlan_in_port(port, ifname);
	BUG_ON(!vlan);
	
	/* check it's not already in the hash lookup table */
	if (macvlan_hash_lookup(port, macaddr)) {
		DEBUG("macvlan: user tried to add mac addr twice!\n");
		return -EEXIST;
	}

	if ((atomic_read(&vlan->nmacs) > 0)
	    && (port->flags & MVL_FILTER_ON_DEST)) {
		printk("macvlan:  Already have a MAC on this vlan: %s and we are filtering on DEST, so no more are allowed!\n",
		       ifname);
		return -EINVAL;
	}

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		return -ENOBUFS;
	}
	memset(entry, 0, sizeof(*entry));
	
	memcpy(entry->mac, macaddr, sizeof(entry->mac));
	entry->vlan = vlan;
	entry->next = port->hash_table[bucket];
	port->hash_table[bucket] = entry;
	DEBUG("macvlan: added %02x:%02x:%02x:%02x:%02x:%02x to vlan %p\n", 
	      entry->src[0],entry->src[1],entry->src[2],
	      entry->src[3],entry->src[4],entry->src[5],
	      vlan);

	atomic_inc(&vlan->nmacs);
	
	if (port->flags & MVL_FILTER_ON_DEST) {
		/* Set the MAC on the vlan device so that it sends pkts correctly. */
		memcpy(vlan->dev->dev_addr, macaddr, ETH_ALEN);
	}
	
	return 0;
} /* macvlan_hash_add */

/* cleans up the mac hash entry memory (kfree). */
static int macvlan_hash_rem(const char* vlan_ifname,
			    const unsigned char* mac) {
	int bucket = VLAN_BUCKET(mac);
	struct macvlan_port *port;
	struct macvlan_hash_entry *entry;
	struct macvlan_hash_entry* prev;
	    
	/* find the port in question */
	port = macvlan_find_port_for_mvlan_ifname(vlan_ifname);

	if (!port) {
		return -ENODEV;
	}

	entry = port->hash_table[bucket];
	prev = NULL;
	//printk("hash_rem, found port: %p  bucket: %i  entry: %p\n",
	//       port, bucket, entry);
	while (entry) {
		//printk("Testing entry: %p\n", entry);
		if (memcmp(entry->mac, mac, ETH_ALEN) == 0) {
			if (prev) {
				prev->next = entry->next;
			}
			else {
				port->hash_table[bucket] = entry->next;
			}
			atomic_dec(&entry->vlan->nmacs);
			kfree(entry);
			return 0;
		}
		prev = entry;
		entry = entry->next;
	}
	
	return -EINVAL;
}/* macvlan_hash_rem */


static int macvlan_ioctl_deviceless_stub(unsigned long arg) {
	int err = 0;
	struct macvlan_ioctl req;
	struct macvlan_ioctl_reply rep;
	unsigned long flags;
	
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (copy_from_user(&req, (void *)arg, sizeof(req)))
		return -EFAULT;

	memset(&rep, 0, sizeof(rep));

	switch (req.cmd)
	{
	case MACVLAN_ENABLE:
	{
		/* 
		 * enable creation of mac based vlans
		 * layered over an ethernet device
		 */
		char ifname[IFNAMSIZ];
	    
		/* Get name of ethernet device */
		if(copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';

		MVL_WRITE_LOCK(flags);
		err = macvlan_port_create(ifname);
		MVL_WRITE_UNLOCK(flags);
		
		break;
	}
	case MACVLAN_DISABLE:
	{
		/*
		 * disable creation of mac based vlans
		 * layered over an ethernet device
		 */
		char ifname[IFNAMSIZ];
		
		/* Get name of ethernet device */
		if(copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';

		MVL_WRITE_LOCK(flags);
		err = macvlan_port_cleanup(ifname);
		MVL_WRITE_UNLOCK(flags);

		break;
	}
	case MACVLAN_ADD:
	{
		/* 
		 * create a new mac based vlan
		 */
		char ifname[IFNAMSIZ];
		int ifidx;
		
		/* Get name of port over which we are creating a vlan */
		if(copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';
		
		/* Get index of new vlan we are creating */
		ifidx = req.ifidx;

		/* Has internal locking. */
		err = macvlan_vlan_create(ifname, ifidx);
		
		break;
	}
	case MACVLAN_SET_PORT_FLAGS:
	{
		/* 
		 * Set a macvlan_port's flags
		 */
		char ifname[IFNAMSIZ];
		
		/* Get name of port over which we are creating a vlan */
		if(copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';

		MVL_WRITE_LOCK(flags);
		err = macvlan_port_set_flags(ifname, req.ifidx);
		MVL_WRITE_UNLOCK(flags);
		
		break;
	}
	case MACVLAN_GET_PORT_FLAGS:
	{
		/* 
		 * Set a macvlan_port's flags
		 */
		struct macvlan_port *port;
		char ifname[IFNAMSIZ];
		
		/* Get name of port over which we are creating a vlan */
		if(copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';
		
		MVL_READ_LOCK(flags);
		/* find the port to which ifname belongs */
		port = macvlan_find_port_for_mvlan_ifname(ifname);
		if (!port) {
			err = -ENODEV;
		}
		else {
			rep.num = port->flags;
		}
		MVL_READ_UNLOCK(flags);
		
		if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
			err = -EFAULT;
		}

		break;
	}
	case MACVLAN_DEL:
	{
		/*
		 * destroy a mac based vlan
		 */
		char ifname[IFNAMSIZ];
		
		/* Get name of vlan to remove */
		if (copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';

		/* Has internal locking */
		err = macvlan_vlan_cleanup(ifname);
		break;
	}
	
	case MACVLAN_BIND:
	{
		/*
		 * Bind a MAC address to vlan
		 */
		char ifname[IFNAMSIZ];
		unsigned char macaddr[ETH_ALEN];
		
		/* Get name of vlan */
		if (copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';
		
		/* Get mac address to bind to vlan */
		if (copy_from_user(macaddr, (void *)req.macaddr, sizeof(macaddr))) {
			err = -EFAULT;
			break;
		}
		
		MVL_WRITE_LOCK(flags);
		err = macvlan_hash_add(ifname, macaddr);
		MVL_WRITE_UNLOCK(flags);
		break;
	}
	case MACVLAN_UNBIND:
	{
		/* 
		 * Unbind a MAC address from a vlan
		 */
		char ifname[IFNAMSIZ];
		unsigned char macaddr[ETH_ALEN];
	    
		/* Get name of vlan */
		if (copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';
	    
		/* Get mac address to unbind */
		if (copy_from_user(macaddr, (void *)req.macaddr, sizeof(macaddr))) {
			err = -EFAULT;
			break;
		}

		MVL_WRITE_LOCK(flags);
		err = macvlan_hash_rem(ifname, macaddr);
		MVL_WRITE_UNLOCK(flags);
		break;
	}

	case MACVLAN_IS_MACVLAN:
	{
		/* 
		 * Give user-space a chance of determining if we are a MAC-VLAN nor not.
		 *  (If the IOCTL fails, we are not, otherwise we are.)
		 */
		struct macvlan_port *port;
		char ifname[IFNAMSIZ];
	    
		/* Get name of vlan */
		if(copy_from_user(ifname, (void *)req.ifname, sizeof(ifname))) {
			err = -EFAULT;
			break;
		}
		ifname[IFNAMSIZ-1] = '\0';
	    
		MVL_READ_LOCK(flags);
		/* find the port in question */
		port = macvlan_find_port_for_mvlan_ifname(ifname);
		MVL_READ_UNLOCK(flags);

		if (!port) {
			/* printk("device: %s is NOT a MAC-VLAN\n", ifname); */
			err = -ENODEV;
		}
		else {
			/* printk("device: %s IS a MAC-VLAN\n", ifname); */
			err = 0;
		}
		break;
	}
	case MACVLAN_GET_NUM_PORTS:
	{
		/* 
		 * how many ethernet devices have mac based vlans enabled over them
		 */
		rep.num = atomic_read(&macvlan_nports);
		if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
			err = -EFAULT;
			break;
		}
		break;
	}
	case MACVLAN_GET_PORT_NAME:
	{
		/* 
		 * name the nth device which has mac based vlans enabled over it
		 */
		struct macvlan_port *port;
		int n = req.portidx;

		MVL_READ_LOCK(flags);
		/* find the port in question */
		for (port = port_list; port && n; port = port->next, n--);
		if (!port) {
			err = -ENODEV;
		}
		else {
			memcpy(rep.name, port->dev->name, IFNAMSIZ);
					
			if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
				err = -EFAULT;
			}
		}
		MVL_READ_UNLOCK(flags);
		break;
	}
	case MACVLAN_GET_NUM_VLANS:
	{
		/*
		 * how many vlans are layered over the nth mac-based
		 * vlan enabled device
		 */

		struct macvlan_port *port;
		int n = req.portidx;

		MVL_READ_LOCK(flags);
		/* find the port in question */
		for (port = port_list; port && n; port = port->next, n--);

		if (!port) {
			err = -ENODEV;
		}
		else {
			rep.num = atomic_read(&port->ndevs);
			if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
				err = -EFAULT;
			}
		}
		MVL_READ_UNLOCK(flags);
	
		break;
	}
	case MACVLAN_GET_VLAN_NAME:
	{
		/* 
		 * what's the name of the mth vlan layered over the nth
		 * mac-based-vlan enabled ethernet device
		 */
		struct macvlan_port *port;
		struct macvlan_vlan *vlan;
		int n = req.portidx;
		int m = req.ifidx;

		
		MVL_READ_LOCK(flags);
		/* find the port in question */
		for (port = port_list; port && n; port = port->next, n--);
		if (!port) {
			err = -EINVAL;
		}
		else {
			/* find the vlan in question */
			for (vlan = port->vlan_list; vlan && m; vlan = vlan->next, m--);

			if (!vlan) {
				err = -ENODEV;
			}
			else {
				memcpy(rep.name, vlan->dev->name, IFNAMSIZ);
			}
			if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
				err = -EFAULT;
			}
		}
		MVL_READ_UNLOCK(flags);
		break;
	}
	case MACVLAN_GET_NUM_MACS:
	{
		/* 
		 * how many mac addresses are owned by the mth vlan
		 * layered over the nth mac-based-vlan enabled 
		 * ethernet device
		 */
		struct macvlan_port *port;
		struct macvlan_vlan *vlan;
		int n = req.portidx;
		int m = req.ifidx;


		MVL_READ_LOCK(flags);
		/* find the port in question */
		for (port = port_list; port && n; port = port->next, n--);

		if (!port) {
			err = -EINVAL;
		}
		else {
			/* find the vlan in question */
			for (vlan = port->vlan_list; vlan && m; vlan = vlan->next, m--);

			if (!vlan) {
				err = -ENODEV;
			}
			else {
				rep.num = atomic_read(&vlan->nmacs);
			}
			if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
				err = -EFAULT;
			}
		}
		MVL_READ_UNLOCK(flags);
		break;
	}
	case MACVLAN_GET_MAC_NAME:
	{
		/* 
		 * what's the pth mac address owned by the mth vlan
		 * layered over the nth mac-based-vlan enabled 
		 * ethernet device
		 */
		struct macvlan_port *port;
		struct macvlan_vlan *vlan;
		struct macvlan_hash_entry *entry;
		int n = req.portidx;
		int m = req.ifidx;
		int p = req.macaddridx;

		MVL_READ_LOCK(flags);
		/* find the port in question */
		for (port = port_list; port && n; port = port->next, n--);

		if (!port) {
			err = -EINVAL;
		}
		else {
			/* find the vlan in question */
			for (vlan = port->vlan_list; vlan && m; vlan = vlan->next, m--);

			if (!vlan) {
				err = -ENODEV;
			}
			else {
				/* find the mac addr in question */
				int i;
				for (i = 0; i<MACVLAN_HASH_LEN; i++) {
					entry = port->hash_table[i];
					while (entry) {
						if (entry->vlan == vlan) {
							if (--p == 0) {
								memcpy(rep.name, entry->mac, sizeof(entry->mac));
								goto found_one;
							}
						}
						entry = entry->next;
					} /* while */
				}/* for */
				
				/* Didn't find one */
				err = -ENODEV;
			}

		found_one:		
			
			if (copy_to_user((void *)req.reply, &rep, sizeof(rep))) {
				err = -EFAULT;
			}
		}
		MVL_READ_UNLOCK(flags);
		break;
	}
	default:
		err = -EOPNOTSUPP;
		break;
	}

	/* printk("Returning err: %i\n", err); */
	return err;
}/* ioctl handler */


/* Return >= 0 if packet is consumed, otherwise return < 0. */
static inline int mvl_handle_frame_fos(struct macvlan_port* port, struct sk_buff* skb) {
	struct macvlan_vlan *vlan; /* the higher layer i/f to which skbuff is mapped */
	int rv;
	unsigned long flags;
	
	DEBUG("%s:  got port: %p, not filtering on DEST\n", __PRETTY_FUNCTION__, port);

	MVL_IRQ_RLOCK(flags);
	if (!(vlan = macvlan_hash_lookup(port, skb->mac.ethernet->h_source))) {
		/* not for us, but don't delete it, others may consume it */
		rv = -ENODEV;
	}
	else {
		if (!(vlan->dev->flags & IFF_UP)) {
			rv = 1;	 /* was consumed */
			kfree_skb(skb);
		}
		else {
			vlan->statistics.rx_packets++;
			/*  Count the lower-level's header to make our counters look more
			 *  like an ethernet device. */
			vlan->statistics.rx_bytes += (skb->len + vlan->lowerdev->hard_header_len);

			skb->dev = vlan->dev;
			dev_hold(skb->dev);
			if (memcmp(vlan->dev->dev_addr, skb->mac.ethernet->h_dest, ETH_ALEN)) {
				skb->pkt_type=PACKET_OTHERHOST;
			}
			else {
				skb->pkt_type = PACKET_HOST;
			}
			MVL_IRQ_RUNLOCK(flags);
			netif_rx(skb);
			dev_put(skb->dev);
			rv = 0;
			goto out;
		}
	}
	
	MVL_IRQ_RLOCK(flags);
 out:
	return rv;
} /* filter on source */


/* Return >= 0 if packet is consumed, otherwise return < 0. */
static inline int mvl_handle_frame_fod(struct macvlan_port* port, struct sk_buff* skb) {
	struct macvlan_vlan *vlan; /* the higher layer i/f to which skbuff is mapped */
	int rv;
	unsigned long flags;

	/* Filtering on destination.. */
	/* If it's a broadcast pkt, send it to all of them.  Otherwise,
	 * send it to just one of them.
	 */
	if ((skb->pkt_type == PACKET_BROADCAST) || (skb->pkt_type == PACKET_MULTICAST)) {
		/* never consume if we take this code branch, because it's bcast */
		DEBUG("%s:  got port: %p, filtering on DEST, type is bcast or multicast\n",
		      __PRETTY_FUNCTION__, port);
		//printk("fod: ");
		MVL_IRQ_RLOCK(flags);
		//printk("1 ");
		for (vlan = port->vlan_list; vlan; vlan = vlan->next) {
			//printk(".");
			DEBUG("%s:  got vlan: %s, nmacs: %i, up: %i\n",
			      __PRETTY_FUNCTION__, vlan->dev->name,
			      vlan->nmacs, (vlan->dev->flags & IFF_UP));
			if (atomic_read(&vlan->nmacs) && (vlan->dev->flags & IFF_UP)) {
				struct sk_buff* nskb;
				
				atomic_inc(&skb->users);
				nskb = skb_share_check(skb, GFP_ATOMIC);
				if (!nskb) {
					vlan->statistics.rx_fifo_errors++;
					vlan->statistics.rx_errors++;
				}
				else {
					vlan->statistics.rx_packets++;
					/*  Count the lower-level's header to make our counters
					 *  look more like an ethernet device. */
					vlan->statistics.rx_bytes +=
						(nskb->len + vlan->lowerdev->hard_header_len);
					vlan->statistics.multicast++;
					
					nskb->dev = vlan->dev;
					netif_rx(nskb);
				}
			}
		}
		//printk("2 ");
		rv = -1; /* did not consume this pkt, merely tasted it */
		MVL_IRQ_RUNLOCK(flags);
		goto out;
	}
	else {
		struct ethhdr *eth = skb->mac.ethernet;
		char* d = eth->h_dest;
		/* Not a broadcast, try to find our port based on DESTINATION */
		//printk("fodNB ");
		MVL_IRQ_RLOCK(flags);
		if (!(vlan = macvlan_hash_lookup(port, d))) {
			/* not for us */
			DEBUG("%s:  not a broadcast, and could not find vlan for dest: %2hx:%2hx:%2hx:%2hx:%2hx:%2hx\n",
			      __PRETTY_FUNCTION__, d[0], d[1], d[2], d[3], d[4], d[5]);
			
			rv = -ENODEV;
			//printk("1 ");
		}
		else {
			DEBUG("%s:  not a broadcast, found vlan for dest: "
			      "%2hx:%2hx:%2hx:%2hx:%2hx:%2hx, up: %i\n",
			      __PRETTY_FUNCTION__, d[0], d[1], d[2], d[3], d[4], d[5],
			      (vlan->dev->flags & IFF_UP));
		
			if (!(vlan->dev->flags & IFF_UP)) {
				kfree_skb(skb);
				rv = 0; /* consume */
			}
			else {
				vlan->statistics.rx_packets++;
				/*  Count the lower-level's header to make our counters
				 *  look more like an ethernet device. */
				vlan->statistics.rx_bytes +=
					(skb->len + vlan->lowerdev->hard_header_len);
		
				skb->dev = vlan->dev;
				if (!(eth->h_dest[0] & 1)) {
					/* if it's not multicast, see if it's
					 * for us, or not.
					 */
					if (memcmp(vlan->dev->dev_addr, eth->h_dest, ETH_ALEN)) {
						skb->pkt_type = PACKET_OTHERHOST;
					}
					else {
						skb->pkt_type = PACKET_HOST;
					}
				}
				dev_hold(skb->dev);
				MVL_IRQ_RUNLOCK(flags);
				//printk("2 ");
				netif_rx(skb);
				dev_put(skb->dev);
				//printk("3 ");
				rv = 0;
				goto out;
			}
		}
	}/* else, was not broadcast */

	MVL_IRQ_RUNLOCK(flags);
	//printk("4 ");

 out:
	//printk("5 ");
	return rv;
}/* filter on dest */


/* global entry point when receiving a pkt from lower-level devices.  Return
 * >= 0 if we consume, otherwise packet will be sent to the rest of the stack
 * as normal.
 *
 */
static int macvlan_handle_frame(struct sk_buff *skb)
{
	struct macvlan_port *port;  /* maps skbuffs arriving from a lower layer
				     * i/f to a higher layer i/f */
	int rv = 0;

	port = skb->dev->macvlan_priv;
	if (port->flags & MVL_FILTER_ON_DEST) {
		rv = mvl_handle_frame_fod(port, skb);
	}
	else {
		rv = mvl_handle_frame_fos(port, skb);
	}

	return rv;
}


#ifdef MVL_CONFIG_PROC_FS

static int read_mvl_glbl(char *page, char **start, off_t off,
			 int count, int *eof, void *data) {
	int	ret = -1;
	char	*p = page;
	int mx_len = (4096 - (p - page));
	
	if (! *eof ) {
		struct macvlan_port* port;
		int cnt;
		unsigned long flags;
		
                /* Global counts here... */
		p += sprintf(p, "MAC-VLAN module:\n");

		p += sprintf(p, " port count: %i  vlan_counter: %i\n",
			     atomic_read(&macvlan_nports),
			     atomic_read(&mvl_vlan_counter));

		MVL_READ_LOCK(flags);
		port = port_list;
		while (port) {
			p += sprintf(p, " %s  num_vlans: %i  flags: %x\n",
				     port->dev->name, atomic_read(&port->ndevs), port->flags);

			/* catch overflow */
			cnt = p - page;
			if (cnt > (mx_len - 60)) {
				if (mx_len - cnt >= 20) {
					p += sprintf(p, "OUT_OF_SPACE!\n");
				}
				break;
			}

			port = port->next;
		}

		ret = p - page;
		MVL_READ_UNLOCK(flags);
	}
	return ret;
} /* read_mvl_glbl */

static int write_mvl_glbl(struct file *file, const char *buffer,
			  unsigned long count, void *data) {
	char		*p;
	const char	*end;
	int		ret=count;
	int		len;
	char		dev_name[2][IFNAMSIZ];
        char* tmps = NULL;
	unsigned long flags;
        
        MVL_WRITE_LOCK(flags);
        
	end = buffer+count;

	for (p= (char *) buffer; p< end ; ) {
		if (iswhitespace(*p)) {
			p++;
			continue;
		}

		memset(dev_name[0], 0 ,IFNAMSIZ);
		memset(dev_name[1], 0 ,IFNAMSIZ);

		len = strlen("add_port ");
		if (strncmp(p, "add_port ", len)==0)
		{
			p += len;

			if ( (p + IFNAMSIZ) <= end)
				p += copy_next_word(dev_name[0], p, IFNAMSIZ);
			else
				p += copy_next_word(dev_name[0], p, end-p );

			skip_whitespace(p);

                        /* This can fail, but not sure how to return failure
                         * to user-space here.
                         */
                        macvlan_port_create(dev_name[0]);
                        goto forend;
		}
                
		len = strlen("remove_port ");
		if (strncmp(p,"remove_port ",len)==0) {
			p += len;

			if ( (p + IFNAMSIZ) <= end)
				p += copy_next_word(dev_name[0], p, IFNAMSIZ);
			else
				p += copy_next_word(dev_name[0], p, end-p );

                        skip_whitespace(p);

                        macvlan_port_cleanup(dev_name[0]);
                        goto forend;
		}

                len = strlen("debug_lvl ");
		if (strncmp(p,"debug_lvl ",len)==0)
		{
			p += len;

			if ( (p + IFNAMSIZ) <= end)
				p += copy_next_word(dev_name[0], p, IFNAMSIZ);
			else
				p += copy_next_word(dev_name[0], p, end-p );

                        skip_whitespace(p);

                        debug_lvl = simple_strtoul(dev_name[0], &tmps, 10);
                        goto forend;
		}

                printk("ERROR:  Unsupported command\n");

        forend:
		p++;
	}

        MVL_WRITE_UNLOCK(flags);

	return ret;
} /* write_mvl_glbl */

/* Proc file read for mac-vlan. */
static int read_mvl(char *page, char **start, off_t off,
		    int count, int *eof, void *data) {
	int	ret = -1;
	if (! *eof ) {
		char	*p = page;
		struct macvlan_vlan* vlan = (struct macvlan_vlan*)(data);
		struct macvlan_hash_entry* entry;
		int i;
		int count = 0;
		int cnt;
		int mx_len = 4096;
		unsigned long flags;
		
		
		MVL_READ_LOCK(flags);
		
                /* Global counts here... */
		p += sprintf(p, "MAC-VLAN %s:\n", vlan->dev->name);

		p += sprintf(p, " MAC count: %i  lower_dev: %s  macvlan-port: %s\n",
			     atomic_read(&vlan->nmacs), vlan->lowerdev->name,
			     vlan->port->dev->name);

		for (i = 0; i<MACVLAN_HASH_LEN; i++) {
			entry = vlan->port->hash_table[i];
			while (entry) {
				if (entry->vlan == vlan) {
					/* catch overflow */
					cnt = p - page;
					if (cnt > (mx_len - 40)) {
						if (mx_len - cnt >= 20) {
							p += sprintf(p, "OUT_OF_SPACE!\n");
						}
						goto outofspace;
					}

					p += sprintf(p, "  [%i] %02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n",
						     count, entry->mac[0], entry->mac[1], entry->mac[2], 
						     entry->mac[3], entry->mac[4], entry->mac[5]);
					count++;

				}
				entry = entry->next;
			}/* while */
		}/* for */

	outofspace:

		ret = p - page;

		MVL_READ_UNLOCK(flags);
	}
	return ret;
} /* read_mvl_glbl */


static int write_mvl(struct file *file, const char *buffer,
		     unsigned long count, void *data) {
	char		*p;
	const char	*end;
	int		ret=count;
	int		len;
	char            arg[MVL_MX_ARG_LEN+1];
	
	struct macvlan_vlan* vlan = (struct macvlan_vlan*)(data);
        char mac[ETH_ALEN];
	unsigned long flags;
	
        MVL_WRITE_LOCK(flags);
        
	end = buffer+count;

	for (p= (char *) buffer; p< end ; ) {
		if (iswhitespace(*p)) {
			p++;
			continue;
		}

		memset(arg, 0, MVL_MX_ARG_LEN+1);

		len = strlen("add_mac ");
		if (strncmp(p, "add_mac ", len)==0) {
			p += len;

			if ( (p + MVL_MX_ARG_LEN) <= end)
				p += copy_next_word(arg, p, MVL_MX_ARG_LEN);
			else
				p += copy_next_word(arg, p, end-p);

			skip_whitespace(p);

			if (toMacString(mac, arg) < 0) {
				printk("macvlan:  MAC format is incorrect: %s\n",
				       arg);
			}
			else {
				/* This can fail, but not sure how to return failure
				 * to user-space here.
				 */
				macvlan_hash_add(vlan->dev->name, mac);
			}
                        goto forend;
		}
                
		len = strlen("remove_mac ");
		if (strncmp(p,"remove_mac ",len)==0) {
			p += len;

			if ( (p + MVL_MX_ARG_LEN) <= end)
				p += copy_next_word(arg, p, MVL_MX_ARG_LEN);
			else
				p += copy_next_word(arg, p, end-p);

			skip_whitespace(p);

			if (toMacString(mac, arg) < 0) {
				printk("macvlan:  MAC format is incorrect: %s\n",
				       arg);
			}
			else {
				/* This can fail, but not sure how to return failure
				 * to user-space here.
				 */
				macvlan_hash_rem(vlan->dev->name, mac);
			}
                        goto forend;
		}

                printk("ERROR:  Unsupported command\n");

        forend:
		p++;
	}

        MVL_WRITE_UNLOCK(flags);

	return ret;
} /* write_mvl */


static int read_mvl_port(char *page, char **start, off_t off,
			 int count, int *eof, void *data) {
	int	ret = -1;
	char	*p = page;
	int mx_len = (4096 - (p - page));
	int i;
	
	if (! *eof ) {
		struct macvlan_port* port = (struct macvlan_port*)(data);
		int cnt;
		struct macvlan_vlan* vlan;
		struct macvlan_hash_entry* entry;
		unsigned long flags;

		MVL_READ_LOCK(flags);
		
                /* Global counts here... */
		p += sprintf(p, "MAC-VLAN Port: %s\n", port->dev->name);

		p += sprintf(p, " vlan count: %i\n", atomic_read(&port->ndevs));

		vlan = port->vlan_list;
		while (vlan) {
			p += sprintf(p, " %s\n", vlan->dev->name);

			/* catch overflow */
			cnt = p - page;
			if (cnt > (mx_len - 40)) {
				if (mx_len - cnt >= 20) {
					p += sprintf(p, "OUT_OF_SPACE!\n");
				}
				goto outofspace;
			}

			vlan = vlan->next;
		}

		/* MAC addr hash */

		for (i = 0; i<MACVLAN_HASH_LEN; i++) {
			if (port->hash_table[i]) {
				p += sprintf(p, " [%i] ", i);
				entry = port->hash_table[i];
				while (entry) {
					/* catch overflow */
					cnt = p - page;
					if (cnt > (mx_len - 40)) {
						if (mx_len - cnt >= 20) {
							p += sprintf(p, "OUT_OF_SPACE!\n");
						}
						goto outofspace;
					}
					
					p += sprintf(p, " %02hx:%02hx:%02hx:%02hx:%02hx:%02hx",
						     entry->mac[0], entry->mac[1], entry->mac[2], 
						     entry->mac[3], entry->mac[4], entry->mac[5]);

					entry = entry->next;
				}
				p += sprintf(p, "\n");
			}
		}
		
	outofspace:
		ret = p - page;
		MVL_READ_UNLOCK(flags);
	}
	return ret;
} /* read_mvl_glbl */


static int write_mvl_port(struct file *file, const char *buffer,
			  unsigned long count, void *data) {
	char		*p;
	const char	*end;
	int		ret=count;
	int		len;
	char		dev_name[2][IFNAMSIZ];
        char* tmps = NULL;
        struct macvlan_port* port = (struct macvlan_port*)(data);
	unsigned long flags;
	
	end = buffer+count;

	for (p= (char *) buffer; p< end ; ) {
		if (iswhitespace(*p)) {
			p++;
			continue;
		}

		memset(dev_name[0], 0 ,IFNAMSIZ);
		memset(dev_name[1], 0 ,IFNAMSIZ);

		len = strlen("add_vlan ");
		if (strncmp(p, "add_vlan ", len)==0) {
			p += len;

			if ( (p + IFNAMSIZ) <= end)
				p += copy_next_word(dev_name[0], p, IFNAMSIZ);
			else
				p += copy_next_word(dev_name[0], p, end-p );

			skip_whitespace(p);

                        /* This can fail, but not sure how to return failure
                         * to user-space here.
                         */
			/* has internal locking */
                        macvlan_vlan_create(port->dev->name,
					    simple_strtoul(dev_name[0], &tmps, 10));
                        goto forend;
		}

		len = strlen("set_flags ");
		if (strncmp(p, "set_flags ", len)==0) {
			p += len;

			if ( (p + IFNAMSIZ) <= end)
				p += copy_next_word(dev_name[0], p, IFNAMSIZ);
			else
				p += copy_next_word(dev_name[0], p, end-p );

			skip_whitespace(p);

                        /* This can fail, but not sure how to return failure
                         * to user-space here.
                         */

			MVL_WRITE_LOCK(flags);
                        macvlan_port_set_flags(port->dev->name,
					       simple_strtoul(dev_name[0], &tmps, 16));
			MVL_WRITE_UNLOCK(flags);
			goto forend;
		}

		len = strlen("remove_vlan ");
		if (strncmp(p,"remove_vlan ",len)==0) {
			p += len;

			if ( (p + IFNAMSIZ) <= end)
				p += copy_next_word(dev_name[0], p, IFNAMSIZ);
			else
				p += copy_next_word(dev_name[0], p, end-p );

                        skip_whitespace(p);

			/* Has internal locking */
                        macvlan_vlan_cleanup(dev_name[0]);
                        goto forend;
		}

                printk("ERROR:  Unsupported command\n");

        forend:
		p++;
	}

	return ret;
} /* write_mvl_port */


#endif


static int __init macvlan_init(void) {
	printk (KERN_INFO "MAC address based VLAN support Revision: 1.3\n");

	port_list = NULL;

	macvlan_ioctl_hook = macvlan_ioctl_deviceless_stub;
	macvlan_handle_frame_hook = macvlan_handle_frame;

#ifdef MVL_CONFIG_PROC_FS

        mvl_proc_dir = proc_mkdir(MVL_PROC_DIR, proc_net);
        if (mvl_proc_dir) {
		mvl_proc_cfg = create_proc_read_entry(MVL_PROC_CFG, S_IRUGO, mvl_proc_dir,
						      read_mvl_glbl, NULL);
		if (mvl_proc_cfg) {
			mvl_proc_cfg->write_proc = write_mvl_glbl;
		}
	}
#endif

	
	return 0;
}

static void macvlan_cleanup(void) {
	struct macvlan_port *port;
	
	macvlan_handle_frame_hook = NULL;
	macvlan_ioctl_hook = NULL;

	/* destroy all existing ports */
	while ((port = port_list)) {
		if (macvlan_port_cleanup(port->dev->name) < 0) {
			BUG_ON(1);
		}
	}

#ifdef MVL_CONFIG_PROC_FS
	if (mvl_proc_cfg) {
		remove_proc_entry(MVL_PROC_CFG, mvl_proc_dir);
		mvl_proc_cfg = NULL;
	}
	if (mvl_proc_dir) {
		remove_proc_entry(MVL_PROC_DIR, proc_net);
		mvl_proc_dir = NULL;
	}
#endif

}/* macvlan_cleanup */


module_init(macvlan_init);
module_exit(macvlan_cleanup);
MODULE_LICENSE("GPL");
