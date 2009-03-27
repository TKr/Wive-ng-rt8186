/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Generic frame diversion
 *
 * Version:	@(#)eth.c	0.5		04/03/2001
 *
 * Authors:	
 * 		Benoit LOCHER:	initial integration within the kernel with support for ethernet
 * 		Dave Miller:	improvement on the code (correctness, performance and source files)
 *
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/config.h>
#include <linux/init.h>
#include <net/dst.h>
#include <net/arp.h>
#include <net/sock.h>
#include <net/ipv6.h>
#include <net/ip.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/checksum.h>
#include <linux/divert.h>
#include <linux/sockios.h>
#include <linux/module.h>


const char sysctl_divert_version[32]="0.52";	/* Current version */

int dv_init(void);
void dv_cleanup(void);



/*
 * Uncomment this *only* to trace a suspect behaviour of the diverter
 * this will produce lots of debug info in the log
 */

// #define DIVERT_DEBUG


#ifdef DIVERT_DEBUG
#	define	MSGDBG(m)	printk(KERN_DEBUG "divert:%s (line %d in %s)\n", m, __LINE__, __FILE__)
#else
#	define	MSGDBG(m)
#endif

#ifdef MODULE


int init_module(void)
{
	int	if_index, ret_code;
	NETDEVICE	*dev;
		
	printk(KERN_INFO "divert: loading module...\n");
	dv_init();
	
	/*
	 * Allocate a divert_blk for each ethernet interface of the system.
	 */
	if_index=0;
	printk(KERN_DEBUG "divert: looping through devices...\n");
	for(dev=dev_base; dev != NULL; dev=dev->next)
	{
		printk(KERN_DEBUG "divert: checking for device %s\n", dev->name);
		if (!strncmp(dev->name, "eth", 3))
		{
			printk(KERN_DEBUG "divert: allocating divert_blk for %s\n", dev->name);
			ret_code=alloc_divert_blk(dev);
			if (ret_code)
				return ret_code;	
		}
		if_index++;
	}
	printk(KERN_DEBUG "divert: finished looping through devices (%d found)\n", if_index);
	printk(KERN_INFO "divert: module loaded.\n");
	return 0;
}


void cleanup_module(void)
{
	int if_index;
	NETDEVICE   *dev;

	printk(KERN_INFO "divert: unloading module...\n");
	/*
	 * Free the divert_blk allocated for ethernet interfaces of the system
	 */
	if_index=0;
	printk(KERN_DEBUG "divert: looping through devices...\n");
	for(dev=dev_base; dev != NULL; dev=dev->next)
	{
		if (!strncmp(dev->name, "eth", 3))
		{
			printk(KERN_DEBUG "divert: freeing divert_blk for %s\n", dev->name);
			free_divert_blk(dev);
		}
		if_index++;
	}
	printk(KERN_DEBUG "divert: finished looping through devices (%d found)\n", if_index);
	dv_cleanup();
	printk(KERN_INFO "divert: module unloaded.\n");
	return;
}

#endif /* MODULE */


int __init dv_init(void)
{
	printk(KERN_INFO "NET4: Frame Diverter %s\n", sysctl_divert_version);
	divert_frame_hook			= divert_frame;
	divert_ioctl_hook			= divert_ioctl;
	alloc_divert_blk_hook		= alloc_divert_blk;
	free_divert_blk_hook		= free_divert_blk;
	strcpy(sysctl_divert_version_hook, sysctl_divert_version);
	dv_init_hook				= dv_init;
	return 0;
}


void dv_cleanup(void)
{
	divert_frame_hook = NULL;
	divert_ioctl_hook = NULL;
	alloc_divert_blk_hook = NULL;
	free_divert_blk_hook = NULL;
	strcpy(sysctl_divert_version_hook, "");
	dv_init_hook = NULL;
	return;
}


/*
 * Allocate a divert_blk for a device. This must be an ethernet nic.
 */
 
int alloc_divert_blk(NETDEVICE *dev)
{
	int alloc_size=(sizeof(struct divert_blk)+3)&~3;
	
//	if (!strncmp(dev->name, "eth", 3))
//	{
	if (dev->type == ARPHRD_ETHER) {
		printk(KERN_DEBUG "divert: allocating divert_blk for %s\n",
			dev->name);
		dev->divert=(struct divert_blk *)kmalloc(alloc_size, GFP_KERNEL);
		if (dev->divert==NULL)
		{
			printk(KERN_DEBUG "divert: unable to allocate divert_blk for %s\n",
				dev->name);
			return -EFAULT;
		}
		else
		{
			dev->divert->rwlock= RW_LOCK_UNLOCKED;
			dev->divert->divert=0;
			dev->divert->rule=NULL;
		}
	}
	else
	{
		printk(KERN_DEBUG "divert: not allocating divert_blk for non-ethernet device %s\n",
			dev->name);
		dev->divert=NULL;
	}
	return 0;
}



/*
 * Free a divert_blk allocated by the above function, if it was
 * allocated on that device.
 *
 */
void free_divert_blk(NETDEVICE  *dev)
{
	if (dev->divert)
	{
		if (dev->divert->rule != NULL)
		{
			printk(KERN_INFO "divert: flushing rules\n");
			divert_flush_rules(dev);
		}
		kfree(dev->divert);
		dev->divert=NULL;
		printk(KERN_DEBUG "divert: freed divert_blk for %s\n",
				dev->name);
	}
	else
	{
		printk(KERN_DEBUG "divert: no divert_blk to free, %s not ethernet\n",
				dev->name);
	}
	return;
}                                                                                                                                     


struct divert_rule *divert_alloc_rule(void)
{
	struct divert_rule	*rule;
	int			alloc_size=(sizeof(struct divert_rule)+3)&~3;

	MSGDBG("entering divert_alloc_rule");
	rule=(struct divert_rule *)kmalloc(alloc_size, GFP_KERNEL);
	if (rule==NULL)
		return NULL;
	memset(rule, 0, alloc_size);
	rule->prev=NULL;
	rule->next=NULL;
	MSGDBG("leaving divert_alloc_rule");
	return rule;
}


void divert_free_rule(struct divert_rule *rule)
{
	MSGDBG("entering divert_free_rule");
	kfree(rule);
	MSGDBG("leaving divert_free_rule");
}


int divert_append_rule(NETDEVICE *dev, struct divert_rule *new_rule)
{
	struct divert_rule	*rule;
	unsigned long 		flags;

	MSGDBG("entering divert_append_rule");
	
	if (dev->divert==NULL)
		return -EFAULT;
	
	/* write-lock the rules */
	write_lock_irqsave(&dev->divert->rwlock, flags);
	
	/* special case, 1st rule appended */
	new_rule->next=NULL;

	rule=dev->divert->rule;

	if (dev->divert->rule==NULL)
	{
		dev->divert->rule=new_rule;
	}
	else
	{
		rule=dev->divert->rule;
	
		while(rule->next!=NULL)
			rule=rule->next;
		rule->next=new_rule;
		new_rule->prev=rule;
		
	}
	
	/* release the write-lock on the rules */
	write_unlock_irqrestore(&dev->divert->rwlock, flags);
#if defined(CONFIG_NET_DIVERT_MODULE)
	/* Increment use count for module if one rule is present on this device */
	if (dev->divert->rule->next == NULL)
		MOD_INC_USE_COUNT;
#endif	/* CONFIG_NET_DIVERT_MODULE */
	
	MSGDBG("leaving divert_append_rule");
	
	return 0;
}


int divert_insert_rule(NETDEVICE *dev, struct divert_rule *new_rule, int position)
{
	struct divert_rule      *rule=NULL,
						   	*prev=NULL;	
	unsigned long			flags;
	
	MSGDBG("entering divert_insert_rule");
	
	if (dev->divert==NULL)
		return -EFAULT;

	/* write-lock the rules */
	write_lock_irqsave(&dev->divert->rwlock, flags);
	
	rule=divert_get_rule(dev, position);
	if (rule==NULL)
	{
		write_unlock_irqrestore(&dev->divert->rwlock, flags);	
		return -EINVAL;
	}

	prev=rule->prev;
	
	if (prev != NULL)
	{
		prev->next=new_rule;
	}

	new_rule->prev=prev;
	new_rule->next=rule;

	rule->prev=new_rule;
	
	if (position==1)
		dev->divert->rule=new_rule;

	/* release the write-lock */
	write_unlock_irqrestore(&dev->divert->rwlock, flags);

#if defined(CONFIG_NET_DIVERT_MODULE)
	/* Increment use count for module if one rule is present to this interface */
	if (dev->divert->rule->next==NULL)
		MOD_INC_USE_COUNT;
#endif	/* CONFIG_NET_DIVERT_MODULE */
	
	MSGDBG("leaving divert_insert_rule");
	
	return 0;
}


int divert_delete_rule(NETDEVICE *dev, int position)
{
	struct divert_rule	*rule, 
						*prev=NULL, 
						*next=NULL;
	unsigned long		flags;

	MSGDBG("entering divert_delete_rule");
	
	/* write-lock the rules */
	write_lock_irqsave(&dev->divert->rwlock, flags);
	
	rule=divert_get_rule(dev, position);
	if (rule==NULL)
	{
		write_unlock_irqrestore(&dev->divert->rwlock, flags);
		return -EINVAL;
	}

	prev=rule->prev;
	next=rule->next;

	if (next != NULL)
	{
		next->prev=prev;
	}

	if (prev != NULL)
	{
		prev->next=next;
	}
	
	if (position==1)
		dev->divert->rule=next;
	
	divert_free_rule(rule);
	
	/* release the write-lock on the rules */
	write_unlock_irqrestore(&dev->divert->rwlock, flags);

#if defined(CONFIG_NET_DIVERT_MODULE)
	/* Decrement use count on this device if the last rule has been removed */
	if (dev->divert->rule == NULL)
		MOD_DEC_USE_COUNT;
#endif /* CONFIG_NET_DIVERT_MODULE */
	
	MSGDBG("leaving divert_delete_rule");
	
	return 0;
}


int divert_replace_rule(NETDEVICE *dev, struct divert_rule *new_rule, int position)
{
	struct divert_rule	*rule;
	unsigned long		flags;

	MSGDBG("entering divert_replace_rule");
	
	/* write-lock the rules */
	write_lock_irqsave(&dev->divert->rwlock, flags);
	
	rule=divert_get_rule(dev, position);
	if (rule==NULL)
	{
		/* release the write-lock on the rules */
		write_unlock_irqrestore(&dev->divert->rwlock, flags);
		return -EINVAL;
	}

	if (rule->next != NULL)
		rule->next->prev=new_rule;
	
	if (rule->prev != NULL)
		rule->prev->next=new_rule;
	
	new_rule->next=rule->next;
	new_rule->prev=rule->prev;

	if (position==1)
		dev->divert->rule=new_rule;
	
	divert_free_rule(rule);

	/* release the write-lock */
	write_unlock_irqrestore(&dev->divert->rwlock, flags);
	
	MSGDBG("leaving divert_replace_rule");
	
	return 0;
}
	

int divert_flush_rules(NETDEVICE *dev)
{
	struct divert_rule	*rule,
						*next;
	unsigned long		flags;

	MSGDBG("entering divert_flush_rules");
	
	/* No diversion possible on this interface ? */
	if (!dev->divert)
		return -EINVAL;

	/* there are no rule set on this interface */
	if (!dev->divert->rule)
		return -EINVAL;
	
	/* write-lock the rules */
	write_lock_irqsave(&dev->divert->rwlock, flags);
	
	rule=dev->divert->rule;
	dev->divert->rule=NULL;
	while(rule!=NULL)
	{
		next=rule->next;
		divert_free_rule(rule);
		rule=next;
	}

	/* release the write-lock */
	write_unlock_irqrestore(&dev->divert->rwlock, flags);

#if defined(CONFIG_NET_DIVERT_MODULE)
	/* Decrement use count on this module when rules are empty on a device */
	MOD_DEC_USE_COUNT;
#endif	/* CONFIG_NET_DIVERT_MODULE */
	
	MSGDBG("leaving divert_flush_rules");
	
	return 0;
}


/*
 *
 * Must be called with divert->rwlock locked
 *
 */
struct divert_rule *divert_get_rule(NETDEVICE *dev, int rule_num)
{
	struct divert_rule	*rule;
	int		num=1;

	MSGDBG("entering divert_get_rule");
	
	if (dev==NULL)
		return NULL;

	if (dev->divert==NULL)
		return NULL;
	
	rule=dev->divert->rule;
	while(rule!=NULL && rule_num!=num)
	{
		num++;
		rule=rule->next;
	}

	printk(KERN_INFO "divert_get_rule: returning rule #%d(%p)\n", rule_num, rule);
	
	MSGDBG("leaving divert_get_rule");
	
	return	rule;
}


/* Some basic sanity checks on the arguments passed to divert_ioctl() */
int check_args(struct divert_cf *div_cf, NETDEVICE **dev)
{
	MSGDBG("entering check_args");
	
	if (dev==NULL)
		return -EFAULT;

	/* Let's try to find the ifname */
	*dev=(NETDEVICE *)dev_get_by_name(div_cf->dev_name);

	/* dev should NOT be null */
	if (*dev==NULL)
	{
		return -EINVAL;
	}
	
	/* user issuing the ioctl must be a super one :) */
	if (!suser())
	{
		dev_put(*dev);
		return -EPERM;
	}

	/* Device must have a divert_blk member NOT null */
	if ((*dev)->divert==NULL)
	{
		dev_put(*dev);
		return -EFAULT;
	}

	MSGDBG("leaving check_args");
	
	
	return 0;
}


void hton_cidr(divert_cidr *cidr)
{
	cidr->ipnum=htonl(cidr->ipnum);
	cidr->ipmask=htonl(cidr->ipmask);
}


void ntoh_cidr(divert_cidr *cidr)
{
	cidr->ipnum=ntohl(cidr->ipnum);
	cidr->ipmask=ntohl(cidr->ipmask);
}


int fill_rule_from_arg(struct divert_cf *div_cf, struct divert_rule *rule)
{
	divert_cidr	scidr, dcidr;
	rule->proto_num=div_cf->proto_num;
	rule->proto_blk=div_cf->proto_blk;
	rule->divert_locally=div_cf->divert_locally;
	memcpy(rule->dest_mac, div_cf->dest_mac, ETH_ALEN);
	switch(div_cf->proto_num)
	{
		case DIVERT_PROTO_IP:
			rule->process_rule=process_ip_rule;
			scidr=rule->proto_blk.ip.scidr;
			dcidr=rule->proto_blk.ip.dcidr;
			hton_cidr(&rule->proto_blk.ip.scidr);
			hton_cidr(&rule->proto_blk.ip.dcidr);
			break;

		case DIVERT_PROTO_ICMP:
			rule->process_rule=process_ip_icmp_rule;
			scidr=rule->proto_blk.icmp.scidr;
			dcidr=rule->proto_blk.icmp.dcidr;
			hton_cidr(&rule->proto_blk.icmp.scidr);
			hton_cidr(&rule->proto_blk.icmp.dcidr);
			break;

		case DIVERT_PROTO_TCP:
			rule->process_rule=process_ip_tcp_rule;
			scidr=rule->proto_blk.tcp.scidr;
			dcidr=rule->proto_blk.tcp.dcidr;
			hton_cidr(&rule->proto_blk.tcp.scidr);
			hton_cidr(&rule->proto_blk.tcp.dcidr);
			break;

		case DIVERT_PROTO_UDP:
			rule->process_rule=process_ip_udp_rule;
			scidr=rule->proto_blk.udp.scidr;
			dcidr=rule->proto_blk.udp.dcidr;
			hton_cidr(&rule->proto_blk.udp.scidr);
			hton_cidr(&rule->proto_blk.udp.dcidr);
			break;

		default:
			printk(KERN_INFO "divert: unsupported protocol: %d\n", div_cf->proto_num);
	}
	printk(KERN_INFO "divert: proto=%d, source=%08X/%08X, dest=%08X/%08X\n",
					div_cf->proto_num,
					(int)scidr.ipnum, (int)scidr.ipmask,
					(int)dcidr.ipnum, (int)dcidr.ipmask);
	return 0;
}


int fill_arg_from_rule(struct divert_rule *rule, struct divert_cf *div_cf)
{
	div_cf->proto_num=rule->proto_num;
	div_cf->proto_blk=rule->proto_blk;
	div_cf->divert_locally=rule->divert_locally;
	memcpy(div_cf->dest_mac, rule->dest_mac, ETH_ALEN);
	printk(KERN_INFO "divert: (*) proto=%d, source=%08X/%08X, dest=%08X/%08X\n",
					div_cf->proto_num, 
					(int)div_cf->proto_blk.ip.scidr.ipnum, (int)div_cf->proto_blk.ip.scidr.ipmask,
					(int)div_cf->proto_blk.ip.dcidr.ipnum, (int)div_cf->proto_blk.ip.dcidr.ipmask);
	switch(div_cf->proto_num)
	{
		case DIVERT_PROTO_IP:
			ntoh_cidr(&div_cf->proto_blk.ip.scidr);
			ntoh_cidr(&div_cf->proto_blk.ip.dcidr);
			break;

		case DIVERT_PROTO_TCP:
			ntoh_cidr(&div_cf->proto_blk.tcp.scidr);
			ntoh_cidr(&div_cf->proto_blk.tcp.dcidr);
			break;

		case DIVERT_PROTO_UDP:
			ntoh_cidr(&div_cf->proto_blk.udp.scidr);
			ntoh_cidr(&div_cf->proto_blk.udp.dcidr);
			break;

		case DIVERT_PROTO_ICMP:
			ntoh_cidr(&div_cf->proto_blk.icmp.scidr);
			ntoh_cidr(&div_cf->proto_blk.icmp.dcidr);
			break;
	}
	return 0;
}


int divert_ioctl(unsigned int cmd, struct divert_cf *arg)
{
	struct divert_cf	div_cf;
	NETDEVICE			*dev;
	int					oper, ret;
	struct divert_rule	*rule;

	if (copy_from_user(&div_cf, arg, sizeof(struct divert_cf)))
		return -EFAULT;
	
	ret=check_args(&div_cf, &dev);
	if (ret)
	{
		return ret;
	}
	
	// Here, we've locked the device (ref_cnt) with check_args
	
	oper=div_cf.cmd;
	
	switch(cmd)
	{
		case SIOCGIFDIVERT:
			switch(oper)
			{
				case DIVERT_GET_RULE:
					rule=divert_get_rule(dev, div_cf.rule_num);
					if (rule==NULL)
					{
						dev_put(dev);
						return -EINVAL;
					}
					ret=fill_arg_from_rule(rule, &div_cf);
					if (ret)
					{
						dev_put(dev);
						return ret;
					}
					break;

				case DIVERT_GET_STATUS:
					div_cf.status=dev->divert->divert;
					break;

				default:
					{
						dev_put(dev);
						return -EINVAL;
					}
			}
			if (copy_to_user(arg, &div_cf ,sizeof(struct divert_cf)))
			{
				dev_put(dev);
				return -EFAULT;
			}
			break;

		case SIOCSIFDIVERT:
			switch(oper)
			{
				case DIVERT_SET_STATE:
					if (div_cf.status ^ dev->divert->divert)
					{
						dev->divert->divert=!dev->divert->divert;
						return 0;
					}
					else
					{
						dev_put(dev);
						return -EALREADY;
					}
					break;

				case DIVERT_APPEND_RULE:
					rule=divert_alloc_rule();
					if (rule==NULL)
					{
						dev_put(dev);
						return -EFAULT;
					}
					ret=fill_rule_from_arg(&div_cf, rule);
					if (ret)
					{
						/* Cleanup allocated memory */
						divert_free_rule(rule);
						dev_put(dev);
						return ret;
					}
					ret=divert_append_rule(dev, rule);
					if (ret)
					{
						/* Cleanup allocated memory */
						divert_free_rule(rule);
						dev_put(dev);
						return ret;
					}
					break;

				case DIVERT_INSERT_RULE:
					rule=divert_alloc_rule();
					if (rule==NULL)
					{
						dev_put(dev);
						return -EFAULT;
					}
					ret=fill_rule_from_arg(&div_cf, rule);
					if (ret)
					{
						/* Cleanup allocated memory */
						divert_free_rule(rule);
						dev_put(dev);
						return ret;
					}
					ret=divert_insert_rule(dev, rule, div_cf.rule_num);
					if (ret)
					{
						/* Cleanup allocated memory */
						divert_free_rule(rule);
						dev_put(dev);
						return ret;
					}
					break;

				case DIVERT_REPLACE_RULE:
					rule=divert_alloc_rule();
					if (rule==NULL)
					{
						dev_put(dev);
						return -EFAULT;
					}
					ret=fill_rule_from_arg(&div_cf, rule);
					if (ret)
					{
						/* Cleanup allocated memory */
						divert_free_rule(rule);
						dev_put(dev);
						return ret;
					}
					ret=divert_replace_rule(dev, rule, div_cf.rule_num);
					if (ret)
					{
						/* Cleanup allocated memory */
						divert_free_rule(rule);
						dev_put(dev);
						return ret;
					}
					break;

				case DIVERT_DELETE_RULE:
					ret=divert_delete_rule(dev, div_cf.rule_num);
					if (ret)
					{
						dev_put(dev);
						return ret;
					}
					break;

				case DIVERT_FLUSH_RULES:
					ret=divert_flush_rules(dev);
					if (ret)
					{
						dev_put(dev);
						return ret;
					}
					break;
					
				default:
					{
						dev_put(dev);
						return -EINVAL;
					}
			}
			break;

		default:
			{
				dev_put(dev);
				return -EINVAL;
			}
	}
	dev_put(dev);
	return ret;
}


/*
 * Check if packet should have its dest mac address set to the box itself
 * for diversion
 */

#define	ETH_DIVERT_FRAME(skb) \
	memcpy(skb->mac.ethernet, skb->dev->dev_addr, ETH_ALEN); \
	skb->pkt_type=PACKET_HOST
	

#define	CHECK_CIDR(ip, cidr, negate) ((negate)? \
				(((ip)&(cidr.ipmask))!=(cidr.ipnum)) : (((ip)&(cidr.ipmask))==(cidr.ipnum)))

#define CHECK_PORT(port, start, end, negate) ((negate)	? \
				(((port) <  (start)) && ((port) >  (end)))	:\
				(((port) >= (start)) && ((port) <= (end))))
	
	
int process_ip_rule(struct sk_buff *skb, struct divert_rule *rule)
{
	struct iphdr	*iph;
	unsigned char	*skb_data_end=skb->data+skb->len;
	
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "entering process_ip_rule()\n");
#endif

	iph=(struct iphdr *)(skb->data);
	/* Check for possible (maliciously) malformed IP frame (thanks Dave) */
	if (((iph->ihl<<2)+(unsigned char*)(iph))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed IP packet !\n");
		return 0;	/* don't eat the frame */
	}
	if (!CHECK_CIDR(iph->saddr, rule->proto_blk.ip.scidr, rule->proto_blk.ip.scidr_negate) ||
	    !CHECK_CIDR(iph->daddr, rule->proto_blk.ip.dcidr, rule->proto_blk.ip.dcidr_negate))
		return 0;
	if (rule->divert_locally)
	{
		skb->pkt_type=PACKET_HOST;
		memcpy(skb->mac.ethernet, skb->dev->dev_addr, ETH_ALEN);
	}
	else
		memcpy(skb->mac.ethernet, rule->dest_mac, ETH_ALEN);
	return 1;	/* packet has been diverted */
}


int process_ip_tcp_rule(struct sk_buff *skb, struct divert_rule *rule)
{
	struct iphdr    *iph;
	struct tcphdr	*tcph;
	unsigned char   *skb_data_end=skb->data+skb->len;
	unsigned short  sport, dport;
	
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "divert: entering process_ip_tcp_rule()\n");
#endif
	
	iph=(struct iphdr *)(skb->data);
	/* Check for possible (maliciously) malformed IP frame (thanks Dave) */
	if (((iph->ihl<<2)+(unsigned char*)(iph))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed IP packet !\n");
		return 0;       /* don't eat the frame */
	}
	tcph=(struct tcphdr *)(((unsigned char *)iph) + (iph->ihl<<2));
	/* Check for possible (maliciously) malformed IP frame (thanx Dave) */
	if (((unsigned char *)(tcph+1))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed TCP packet !\n");
		return 0;
	}
	sport=ntohs(tcph->source);
	dport=ntohs(tcph->dest);
	if (!CHECK_CIDR(iph->saddr, rule->proto_blk.tcp.scidr, rule->proto_blk.tcp.scidr_negate) ||
	    !CHECK_CIDR(iph->daddr, rule->proto_blk.tcp.dcidr, rule->proto_blk.tcp.dcidr_negate))
		return 0; /* ip addresses do not match */
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "divert: tcp addresses matched, checking ports\n");

	printk(KERN_DEBUG "sport=%d(%s %d, %d), dport=%d(%s %d, %d)\n",
		sport,
		rule->proto_blk.tcp.sport_negate?"!":"",
		rule->proto_blk.tcp.sport_start,
		rule->proto_blk.tcp.sport_end,
		dport,
		rule->proto_blk.tcp.dport_negate?"!":"",
		rule->proto_blk.tcp.dport_start,
		rule->proto_blk.tcp.dport_end
		);
	
#endif
	if (!CHECK_PORT(sport, rule->proto_blk.tcp.sport_start, rule->proto_blk.tcp.sport_end, rule->proto_blk.tcp.sport_negate) || 
		!CHECK_PORT(dport, rule->proto_blk.tcp.dport_start, rule->proto_blk.tcp.dport_end, rule->proto_blk.tcp.dport_negate))
		return 0; /* ports do not match */
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "divert: tcp ports matched, diverting\n");
#endif
	if (rule->divert_locally)
	{
		skb->pkt_type=PACKET_HOST;
		memcpy(skb->mac.ethernet, skb->dev->dev_addr, ETH_ALEN);
	}
	else
		memcpy(skb->mac.ethernet, rule->dest_mac, ETH_ALEN);
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "divert: tcp packet diverted\n");
#endif
	return 1;	/* packet has been diverted */
}


int process_ip_udp_rule(struct sk_buff *skb, struct divert_rule *rule)
{
	struct iphdr    *iph;
	struct udphdr	*udph;
	unsigned char   *skb_data_end=skb->data+skb->len;
	unsigned short  sport, dport;
	
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "entering process_ip_udp_rule()\n");
#endif
	
	iph=(struct iphdr *)(skb->data);
	/* Check for possible (maliciously) malformed IP frame (thanks Dave) */
	if (((iph->ihl<<2)+(unsigned char*)(iph))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed IP packet !\n");
		return 0;       /* don't eat the frame */
	}
	udph=(struct udphdr *)(((unsigned char *)iph) + (iph->ihl<<2));
	/* Check for possible (maliciously) malformed IP packet (thanks Dave) */
	if (((unsigned char *)(udph+1))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed UDP packet !\n");
		return 0;	/* don't eat the frame */
	}
	sport=ntohs(udph->source);
	dport=ntohs(udph->dest);
	if (!CHECK_CIDR(iph->saddr, rule->proto_blk.udp.scidr, rule->proto_blk.udp.scidr_negate) ||
	    !CHECK_CIDR(iph->daddr, rule->proto_blk.udp.dcidr, rule->proto_blk.udp.dcidr_negate))
		return 0; /* ip addresses do not match */
	if (!CHECK_PORT(sport, rule->proto_blk.udp.sport_start, rule->proto_blk.udp.sport_end, rule->proto_blk.udp.sport_negate) ||
		!CHECK_PORT(dport, rule->proto_blk.udp.dport_start, rule->proto_blk.udp.dport_end, rule->proto_blk.udp.dport_negate))
		return 0; /* ports do not match */
	if (rule->divert_locally)
	{
		skb->pkt_type=PACKET_HOST;
		memcpy(skb->mac.ethernet, skb->dev->dev_addr, ETH_ALEN);
	}
	else
		memcpy(skb->mac.ethernet, rule->dest_mac, ETH_ALEN);
	return 1;       /* packet has been diverted */
}

	
int process_ip_icmp_rule(struct sk_buff *skb, struct divert_rule *rule)
{
	struct iphdr    *iph;
	struct icmphdr	*icmph;
	unsigned char   *skb_data_end=skb->data+skb->len;
	unsigned short  type;

#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "entering process_ip_icmp_rule()\n");
#endif
	
	iph=(struct iphdr *)(skb->data);
	/* Check for possible (maliciously) malformed IP frame (thanks Dave) */
	if (((iph->ihl<<2)+(unsigned char*)(iph))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed IP packet !\n");
		return 0; /* don't eat the packet */
	}
	icmph=(struct icmphdr *)(((unsigned char *)iph) + (iph->ihl<<2));
	/* Check for possible (maliciously) malformed IP packet (thanks Dave) */
	if (((unsigned char *)(icmph+1))>=skb_data_end)
	{
		printk(KERN_INFO "divert: malformed ICMP packet !\n");
		return 0; /* don't eat the frame */
	}
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "icmp: %08X(%08X/%08X):%d --> %08X(%08X/%08X):%d (%d)\n", 
					iph->saddr, (int)rule->proto_blk.icmp.scidr.ipnum, (int)rule->proto_blk.icmp.scidr.ipmask, 
					CHECK_CIDR(iph->saddr, rule->proto_blk.icmp.scidr, rule->proto_blk.icmp.scidr_negate),
					iph->daddr, (int)rule->proto_blk.icmp.dcidr.ipnum, (int)rule->proto_blk.icmp.dcidr.ipmask,
					CHECK_CIDR(iph->daddr, rule->proto_blk.icmp.dcidr, rule->proto_blk.icmp.dcidr_negate),
					icmph->type);
#endif
	if (!CHECK_CIDR(iph->saddr, rule->proto_blk.icmp.scidr, rule->proto_blk.icmp.scidr_negate) ||
	    !CHECK_CIDR(iph->daddr, rule->proto_blk.icmp.dcidr, rule->proto_blk.icmp.dcidr_negate))
		return 0; /* ip addresses do not match */
	type=icmph->type;
	if (rule->proto_blk.icmp.type!=255)
	{
		if (   (!(rule->proto_blk.icmp.type_negate)?
				(type!=rule->proto_blk.icmp.type):
				(type==rule->proto_blk.icmp.type)	))
			return 0; /* icmp types do not match */
	}
	if (rule->divert_locally)
	{
		skb->pkt_type=PACKET_HOST;
		memcpy(skb->mac.ethernet, skb->dev->dev_addr, ETH_ALEN);
	}
	else
		memcpy(skb->mac.ethernet, rule->dest_mac, ETH_ALEN);
	return 1;
}


#define	HTONS(a)	((((a)>>8)&0x00FF) | (((a)<<8)&0xFF00))


void divert_frame(struct sk_buff *skb)
{
	struct ethhdr			*eth=skb->mac.ethernet;
	struct iphdr			*iph;
	struct divert_rule		*rule=skb->dev->divert->rule;
	unsigned long			flags;
	unsigned int			proto1, proto2, proto3;

	/* read-lock the rule list for this interface */
	read_lock_irqsave(&skb->dev->divert->rwlock, flags);

	proto1=proto2=proto3=0;
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "eth->h_proto=%04X\n", eth->h_proto);
#endif
	
	switch(eth->h_proto)
	{
		case HTONS(ETH_P_IP):
			proto1=DIVERT_PROTO_IP;
			iph=(struct iphdr *)(skb->data);
#ifdef DIVERT_DEBUG
			printk(KERN_DEBUG "iph->protocol=%04X\n", iph->protocol);
#endif
			switch(iph->protocol)
			{
				case IPPROTO_TCP:
					proto2=DIVERT_PROTO_TCP;
					break;
					
				case IPPROTO_UDP:
					proto2=DIVERT_PROTO_UDP;
					break;
				
				case IPPROTO_ICMP:
					proto2=DIVERT_PROTO_ICMP;
					break;
			}
			break;

		case HTONS(ETH_P_ARP):
			proto1=DIVERT_PROTO_ARP;
			break;

	}
#ifdef DIVERT_DEBUG
	printk(KERN_DEBUG "entering divert_frame(%d, %d, %d)\n", proto1, proto2, proto3);
#endif
	
	while(rule)
	{
		if (rule->proto_num==proto1 || rule->proto_num==proto2 || rule->proto_num==proto3)
		{
			/*
			 * if rule_process() returns !=0 then stop further 
			 * diversion on this packet 
			 */
			if (rule->process_rule && rule->process_rule(skb, rule))
				break;
		}
		rule=rule->next;

	}

	/* release the read lock on this interface */
	read_unlock_irqrestore(&skb->dev->divert->rwlock, flags);
}

