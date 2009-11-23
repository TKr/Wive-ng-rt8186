/*
  *	A Linux Ethernet driver for the RealTek 865x chips
  *
  *	$Id: eth865x.c,v 1.153 2009/07/31 01:24:06 joeylin Exp $
  *
  *		Copyright (c)	Realtek Semiconductor Corporation, 2007  
  *		All rights reserved.
  *
  */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/if_vlan.h>
#include <net/pkt_sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <asm/rtl865x/interrupt.h>

#include "asicregs.h"
#include "rtl_types.h"  
#include "swCore.h"
#include "swNic_poll.h"
#include "vlanTable.h"

#ifdef CONFIG_RTL865X_HW_TABLES
#include "tblDrv/rtl865x_lightrome.h"
#endif

#ifdef CONFIG_HW_PROTOCOL_VLAN_TBL
#include "rtl865xC_tblAsicDrv.h"
#endif

#ifdef CONFIG_RTK_VOIP
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
	#define RX_TASKLET
	#define TX_TASKLET
	#ifdef CONFIG_RTL_8190
		#define BR_SHORTCUT
	#endif
#else //!CONFIG_RTK_VOIP

	#define TX_TASKLET
	#if defined(CONFIG_NET_WIRELESS_AGN) || defined(CONFIG_NET_WIRELESS_AG)
		#define BR_SHORTCUT
	#endif
#if defined(CONFIG_RTL8197B_PANA) || defined(CONFIG_RTL865X_PANAHOST)
	#define RX_TASKLET
#else
	//#define RX_TASKLET
#endif

	#define RTK_QUE
	
	#define TIME_STAMP  ( jiffies*10000  +   (REG32(TC0CNT)>>8) *5 ) //jiffies every 10 ms count 1
														            //TC0CNT every 5 us count 1
#endif

#ifdef CONFIG_RTL_KERNEL_MIPS16_DRVETH
#include <asm/mips16_lib.h>
#endif

//#define DYNAMIC_ADJUST_TASKLET
#ifdef CONFIG_RTL8196_RTL8366
#include "RTL8366RB_DRIVER/gpio.h"
#include "RTL8366RB_DRIVER/rtl8366rb_apiBasic.h"
#endif

#ifdef CONFIG_RTK_VLAN_SUPPORT
#include <linux/rtk_vlan.h>
#endif

#if 0
#define DEBUG_ERR printk
#else
#define DEBUG_ERR(format, args...)
#endif

//---------------------------------------------------------------------------
#define DRV_NAME			"Realtek 865x"
#define DRV_VERSION		"0.6"
#define DRV_RELDATE     "Mar 26, 2008"

#ifdef CONFIG_RTL8196_RTL8366
/* prevent boardcast packet rebound ;PlusWang 0429 */
#define PREVENT_BCAST_REBOUND
#endif

#ifdef PREVENT_BCAST_REBOUND
#define UINT32_DIFF(a, b)		((a >= b)? (a - b):(0xffffffff - b + a + 1))
#define P_BCAST_NUM 	5
static int PBR_index ;
struct bcast_entry_s{
	unsigned long time_stamp;
	unsigned char BCAST_SA[6];
};
struct bcast_tr_s{
	struct bcast_entry_s entry[P_BCAST_NUM];
};

struct bcast_tr_s bcast;
#endif
//#define RX_OFFSET 2
#ifdef CONFIG_RTL8197B_PANA
#define NUM_RX_PKTHDR_DESC	256
#define NUM_TX_PKTHDR_DESC	512
#else
#define NUM_RX_PKTHDR_DESC	512
#define NUM_TX_PKTHDR_DESC	1024
#endif

#ifdef CONFIG_RTL8196B_GW_8M
	#undef NUM_RX_PKTHDR_DESC
	#undef NUM_TX_PKTHDR_DESC
	#define NUM_RX_PKTHDR_DESC	ETH_REFILL_THRESHOLD+16
	#define NUM_TX_PKTHDR_DESC	256
#endif

//#define MBUF_LEN	1600
//#define CROSS_LAN_MBUF_LEN		(MBUF_LEN+16)

#define NEXT_DEV(cp)			(cp->dev_next ? cp->dev_next : cp->dev_prev)
#define NEXT_CP(cp)			((struct re_private *)NEXT_DEV(cp)->priv)
#define IS_FIRST_DEV(cp)	(NEXT_CP(cp)->opened ? 0 : 1)
#define GET_IRQ_OWNER(cp) (cp->irq_owner ? cp->dev : NEXT_DEV(cp))
#define RTL8651_IOCTL_GETWANLINKSTATUS 2000
#define RTL8651_IOCTL_GETWANLINKSPEED 	2100
#define RTL8651_IOCTL_SETWANLINKSPEED 	2101

#define RTL8651_IOCTL_GETLANLINKSTATUS 2102
#define RTL8651_IOCTL_GETLANLINKSPEED    2103
#define RTL8651_IOCTL_GETETHERLINKDUPLEX 2104

/*
#ifdef CONFIG_RTK_MESH
#define RTL8651_IOCTL_GETLANLINKSTATUSALL 2105
#endif
*/
	

//---------------------------------------------------------------------------
struct re_private {
	u32			id;            /* VLAN id, not vlan index */
	u32			portmask;     /* member port mask */
	u32			portnum;     	/* number of member ports */
	u32			netinit;
    struct net_device   *dev;
    struct net_device   *dev_prev;
    struct net_device   *dev_next;		

#ifdef RX_TASKLET
    struct tasklet_struct   rx_dsr_tasklet;
#endif

#ifdef TX_TASKLET
    struct tasklet_struct   tx_dsr_tasklet;
#endif

    spinlock_t			lock;
    struct net_device_stats net_stats;

#if defined(DYNAMIC_ADJUST_TASKLET) || defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(BR_SHORTCUT) || defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
    struct timer_list expire_timer; 
#endif

#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR)
    unsigned int    tx_avarage;
    unsigned int    tx_peak;
    unsigned int    rx_avarage;
    unsigned int    rx_peak;
    unsigned int    tx_byte_cnt;
    unsigned int    rx_byte_cnt;    
#endif
#if defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
    unsigned int    tx_avarage;
    unsigned int    tx_peak;
    unsigned int    rx_avarage;
    unsigned int    rx_peak;
    unsigned int    tx_byte_cnt;
    unsigned int    rx_byte_cnt;    
#endif

#ifdef CONFIG_RTK_VLAN_SUPPORT
	struct vlan_info	vlan_setting;
#endif

    unsigned int vid; //vlan id
    unsigned int port; //member port
    int opened; 
    int  irq_owner; //record which dev request IRQ
};


struct  init_vlan_setting {
	unsigned short vid;
	unsigned int portmask;
	unsigned char mac[6];
};

__DRAM_SECTION_  unsigned char cached_eth_addr[6];
__DRAM_SECTION_  struct net_device *cached_dev=NULL;
#ifdef CONFIG_WIRELESS_LAN_MODULE
 EXPORT_SYMBOL(cached_eth_addr);
 EXPORT_SYMBOL(cached_dev);
 #ifdef PERF_DUMP
 int32 (*Fn_rtl8651_romeperfEnterPoint)( uint32 index )=NULL;
  int32  (*Fn_rtl8651_romeperfExitPoint)(uint32 index)=NULL;
 #endif
#endif
//---------------------------------------------------------------------------
static char version[] __devinitdata = \
KERN_INFO DRV_NAME " Ethernet driver v" DRV_VERSION " (" DRV_RELDATE ")\n";

#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)

#define ETH_INTF_NUM	5
static struct net_device *reNet[ETH_INTF_NUM] = {NULL, NULL, NULL, NULL, NULL};
int L2_table_disabled = 0;

#else
#define ETH_INTF_NUM	2
static struct net_device *reNet[ETH_INTF_NUM] = {NULL, NULL};
#endif

static int eth_flag=12; // bit01: tasklet control,  0 dynamic tasklet, 1 - disable tasklet, 2 - always tasklet , 
									// bit2 - bridge shortcut control (1: enable, 0: disable)
									// bit3 - enable IP multicast route (1: enable, 0: disable)
#define TASKLET_MASK	(BIT(0)|BIT(1))

#ifdef DYNAMIC_ADJUST_TASKLET
static int rx_tasklet_enabled = 0;			
static int rx_pkt_thres=0;
static int rx_cnt;

#else
static int rx_tasklet_enabled = 1;			
#endif

#ifdef BR_SHORTCUT
static int pkt_cnt=0, enable_brsc=0;
#endif

#ifdef DELAY_REFILL_ETH_RX_BUF
#define MAX_PRE_ALLOC_RX_SKB	64
#else
#define MAX_PRE_ALLOC_RX_SKB	160
#endif

#ifdef CONFIG_RTL8196B_GW_8M
	#undef MAX_PRE_ALLOC_RX_SKB
	#define MAX_PRE_ALLOC_RX_SKB	48
#endif

#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
//#ifdef CONFIG_RTL8214_SUPPORT
#if 0
	#define MAX_ETH_SKB_NUM	(NUM_RX_PKTHDR_DESC + MAX_PRE_ALLOC_RX_SKB + 512)
#else
	#ifdef CONFIG_RTL8196B
	
		#ifdef DELAY_REFILL_ETH_RX_BUF
		#define MAX_ETH_SKB_NUM	(NUM_RX_PKTHDR_DESC + MAX_PRE_ALLOC_RX_SKB + 600)

		#else
		#define MAX_ETH_SKB_NUM	(NUM_RX_PKTHDR_DESC + MAX_PRE_ALLOC_RX_SKB + 400)
		#endif

	#else
	#define MAX_ETH_SKB_NUM	(NUM_RX_PKTHDR_DESC + MAX_PRE_ALLOC_RX_SKB + 256)
	#endif

	#ifdef CONFIG_RTL8196B_GW_8M
		#undef MAX_ETH_SKB_NUM
		#define MAX_ETH_SKB_NUM	(NUM_RX_PKTHDR_DESC + MAX_PRE_ALLOC_RX_SKB + 32)
	#endif	
#endif
	int eth_skb_free_num=MAX_ETH_SKB_NUM;

extern int reused_skb_num;
static struct sk_buff *dev_alloc_skb_priv_eth(unsigned int size);
static void init_priv_eth_skb_buf(void);
#endif

#ifdef RTK_QUE
struct ring_que {
	int qlen;
	int qmax;	
	int head;
	int tail;
	struct sk_buff *ring[MAX_PRE_ALLOC_RX_SKB+1];
};
__DRAM_SECTION_ static struct ring_que rx_skb_queue;

#else

__DRAM_SECTION_ static struct sk_buff_head rx_skb_queue; 
#endif

static unsigned int rxRingSize[RTL865X_SWNIC_RXRING_MAX_PKTDESC] = {NUM_RX_PKTHDR_DESC, 0, 0, 0, 0, 0};
static unsigned int txRingSize[RTL865X_SWNIC_TXRING_MAX_PKTDESC] = {NUM_TX_PKTHDR_DESC, 0};
static struct  init_vlan_setting vlanSetting[] = {
#ifdef CONFIG_RTL8197B_PANA
    {LAN_VID,  (ALL_PORTS & (~BIT(0)))  ,  {0x00, 0x00, 0x00, 0x00, 0x00, 0x98} },  // eth0(LAN) VLAN P1-P4
    {WAN_VID,  BIT(0) ,                    {0x00, 0x00, 0x00, 0x00, 0x00, 0x97} },  // eth1(WAN) VLAN P0 
#elif defined(CONFIG_RTL8196B_GW_MP) || defined(CONFIG_RTL8196B_AP_ROOT)
    {LAN_VID,  (ALL_PORTS)  ,  {0x00, 0x00, 0x00, 0x00, 0x00, 0x11} },  // eth0(LAN) VLAN P1-P4    
    {WAN_VID,  0 ,                               {0x00, 0x00, 0x00, 0x00, 0x00, 0x22} },  // eth1(WAN) VLAN P0 
#else
    {LAN_VID,  (ALL_PORTS & (~BIT(0)))  ,  {0x00, 0x00, 0x00, 0x00, 0x00, 0x11} },  // eth0(LAN) VLAN P1-P4
    {WAN_VID,  BIT(0) ,                               {0x00, 0x00, 0x00, 0x00, 0x00, 0x22} },  // eth1(WAN) VLAN P0 
#endif

#if defined(CONFIG_RTK_GUEST_ZONE)
    {LAN2_VID,  BIT(2),                               {0x00, 0xe0, 0x4c, 0xf0, 0x00, 0x02} },  // eth2(LAN) for Guest Zone
    {LAN3_VID,  BIT(3),                               {0x00, 0xe0, 0x4c, 0xf0, 0x00, 0x03} },  // eth3(LAN) for Guest Zone
    {LAN4_VID,  BIT(4),                               {0x00, 0xe0, 0x4c, 0xf0, 0x00, 0x04} },  // eth4(LAN) for Guest Zone
#endif
#if defined(CONFIG_RTK_VLAN_SUPPORT)
    {LAN2_VID,  BIT(3),                               {0x00, 0xe0, 0x4c, 0xf0, 0x00, 0x02} },  // eth2(LAN) for Guest Zone
    {LAN3_VID,  BIT(2),                               {0x00, 0xe0, 0x4c, 0xf0, 0x00, 0x03} },  // eth3(LAN) for Guest Zone
    {LAN4_VID,  BIT(1),                               {0x00, 0xe0, 0x4c, 0xf0, 0x00, 0x04} },  // eth4(LAN) for Guest Zone
#endif
};
const uint32 cPVCR[6][4] = {
	{(LAN_VID << 16) | WAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID,},
	{(WAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID,},
	{(LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | WAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID,},
	{(LAN_VID << 16) | LAN_VID, (WAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID,},
	{(LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | WAN_VID, (LAN_VID << 16) | LAN_VID,},
	{(LAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID, (WAN_VID << 16) | LAN_VID, (LAN_VID << 16) | LAN_VID,},
};

#ifdef CONFIG_RTL865X_HW_TABLES
extern struct rtl865x_lrConfig *lrConfig;
#else
void SoftNAT_OP_Mode(int count);
#endif

#ifdef CONFIG_RTK_VLAN_SUPPORT
extern int  rx_vlan_process(struct net_device *dev, struct vlan_info *info, struct sk_buff *skb);
extern int  tx_vlan_process(struct net_device *dev, struct vlan_info *info, struct sk_buff *skb, int wlan_pri);
#endif

int re865x_ioctl (struct net_device *dev, struct ifreq *rq, int cmd);
static unsigned int totalVlans = sizeof(vlanSetting)/sizeof(struct init_vlan_setting);
/* wan_port will be 0 ~ 5 */
unsigned int wan_port; 

unsigned int chip_id; 
unsigned int chip_revision_id; 

#ifdef CONFIG_RTK_VOIP_WAN_VLAN
extern unsigned int    wan_vlan_id_proto;
extern unsigned int    wan_vlan_id_data;
extern unsigned int    wan_vlan_id_video;
#endif

extern int32 rtl8651_getAsicEthernetPHYReg(uint32 phyId, uint32 regId, uint32 *rData);
extern int32 rtl8651_setAsicEthernetPHYReg(uint32 phyId, uint32 regId, uint32 wData);

#ifdef CONFIG_RTK_VOIP_PORT_LINK
struct timer_list link_state_timer;
struct sock *voip_rtnl;
int link_state_timer_work = 0;
#endif
//---------------------------------------------------------------------------
#if 0
#define PRINT_INFO printk

void eth_debug_out(unsigned char *label, unsigned char *data, int data_length)
{
    int i,j;
    int num_blocks;
    int block_remainder;

    num_blocks = data_length >> 4;
    block_remainder = data_length & 15;

	if (label) {
	    PRINT_INFO("%s\n", label);
	}

	if (data==NULL || data_length==0)
		return;

    for (i=0; i<num_blocks; i++)
    {
        PRINT_INFO("\t");
        for (j=0; j<16; j++)
        {
            PRINT_INFO("%02x ", data[j + (i<<4)]);
        }
        PRINT_INFO("\n");
    }

    if (block_remainder > 0)
    {
        PRINT_INFO("\t");
        for (j=0; j<block_remainder; j++)
        {
            PRINT_INFO("%02x ", data[j+(num_blocks<<4)]);
        }
        PRINT_INFO("\n");
    }
}
#endif

#ifdef RTK_QUE
static void rtk_queue_init(struct ring_que *que)
{
	memset(que, '\0', sizeof(struct ring_que));
	que->qmax = MAX_PRE_ALLOC_RX_SKB;	
}

static int rtk_queue_tail(struct ring_que *que, struct sk_buff *skb)
{
	int next;
	uint32 flags;

	save_and_cli(flags);

	if (que->head == que->qmax)
		next = 0;
	else
		next = que->head + 1;
	
	if (que->qlen >= que->qmax || next == que->tail) {
		printk("%s: ring-queue full!\n", __FUNCTION__);
		restore_flags(flags);							
		return 0;
	}	
	
	que->ring[que->head] = skb;
	que->head = next;
	que->qlen++;

	restore_flags(flags);					
	
	return 1;
}

__IRAM_SECTION_
static struct sk_buff *rtk_dequeue(struct ring_que *que)
{
	struct sk_buff *skb;
	uint32 flags;

	save_and_cli(flags);

	if (que->qlen <= 0 || que->tail == que->head) {
		restore_flags(flags);							
		return NULL;
	}

	skb = que->ring[que->tail];
		
	if (que->tail == que->qmax)
		que->tail  = 0;
	else
		que->tail++;

	que->qlen--;	

	restore_flags(flags);					
	
	return (struct sk_buff *)skb;
}
#endif // RTK_QUE

#ifdef CONFIG_RTK_VOIP_PORT_LINK
static int rtnetlink_fill_ifinfo(struct sk_buff *skb, struct net_device *dev,
				 int type, u32 pid, u32 seq, u32 change)
{
	struct ifinfomsg *r;
	struct nlmsghdr  *nlh;
	unsigned char	 *b = skb->tail;

	nlh = NLMSG_PUT(skb, pid, seq, type, sizeof(*r));
	if (pid) nlh->nlmsg_flags |= NLM_F_MULTI;
	r = NLMSG_DATA(nlh);
	r->ifi_family = AF_UNSPEC;
	r->ifi_type = dev->type;
	r->ifi_index = dev->ifindex;
	r->ifi_flags = dev->flags;
	r->ifi_change = change;
	nlh->nlmsg_len = skb->tail - b;
	return skb->len;

nlmsg_failure:
	skb_trim(skb, b - skb->data);
	return -1;
}

static void netlink_sedmsg(unsigned long task_priv){
	struct re_private *cp = (struct re_private *)task_priv;
	struct net_device* dev = cp ->dev;
	struct sk_buff *skb;
	if (voip_rtnl == NULL)
		panic("rtnetlink_init: cannot initialize rtnetlink\n");
	//skb = dev_alloc_skb(sizeof(struct ifinfomsg));
	skb = alloc_skb(sizeof(struct ifinfomsg),GFP_ATOMIC);
	//skb = alloc_skb(sizeof(struct ifinfomsg),GFP_KERNEL);
	if (!skb)
		return;
	if (rtnetlink_fill_ifinfo(skb, dev, RTM_LINKCHANGE, 0, 0, 0) < 0) {
		kfree_skb(skb);
		return;
	}
	NETLINK_CB(skb).dst_groups = RTMGRP_LINK;
	netlink_broadcast(voip_rtnl, skb, 0, RTMGRP_LINK, GFP_KERNEL);
}
static void link_state_timer_action(unsigned long task_priv)
{
	static int LinkState_pre = 0;
	struct re_private *cp = ((struct net_device *)task_priv)->priv;
	int LinkState = 0;
	int tmp = 0;
	int i = 0;
	for( i = 0; i < 5; i ++ ) {
		rtl8651_getAsicEthernetLinkStatus(i , &tmp);
		if( tmp )
			LinkState |= ( 1 << i );
	}
	if(LinkState_pre != LinkState){
		netlink_sedmsg((unsigned long)cp);
	}
	LinkState_pre = LinkState;
   	mod_timer(&link_state_timer, jiffies + 100);
}
#endif

#if	0	//def CONFIG_RTK_PORT_HW_IGMPSNOOPING
/*
  process hw multicast forwarding table ,
  when port is non cable plug in should not exist this one table entry 
  co-work with bridge module ;
  check define flag: "CONFIG_RTK_PORT_HW_IGMPSNOOPING" at linux-2.4.18/net/Config.in
  plusWang 2009-0311
*/
#include "rtl865xC_tblAsicDrv.h"
extern int br_portlist_update(	unsigned int IP , int port ,int sourceRM);

int32 rtl8651_getAsicEthernetLinkStatus(uint32 port, int8 *linkUp) 
{

	if(port >= (RTL8651_PORT_NUMBER+3) || linkUp == NULL)
		return FAILED;
	
#if	1	//def CONFIG_RTK_VOIP
	int status = READ_MEM32( PSRP0 + port * 4 );
	if(status & PortStatusLinkUp)
		*linkUp = TRUE;
	else
		*linkUp = FALSE;
#else
	*linkUp = rtl8651AsicEthernetTable[port].linkUp == 1? TRUE: FALSE;
#endif
	return SUCCESS;
}


//PLUS
void interrupt_link_state_for_multicast(unsigned long task_priv)
{

	static int LinkState_init = 0;
	static int LinkState_pre = 0;
	int LinkState = 0;
	char tmp = 0x0;
	int PortNum;
	int eindex;	//entry index

	//check port1~port4
	for( PortNum = 1; PortNum < 5; PortNum ++ ) {
		rtl8651_getAsicEthernetLinkStatus(PortNum , &tmp);
		//if this port is up then return TURE
		if( tmp )
			LinkState |= ( 1 << PortNum );
	}

	if(LinkState_init == 0 ){
		LinkState_pre = LinkState;
		LinkState_init = 1;
		return;
	}

	/*some port state has changed*/ 
	
	if(LinkState_pre != LinkState){

		
		for( PortNum = 1; PortNum < 5; PortNum ++ ) {

			//just check 1->0 case
			if(( (LinkState & (1<<PortNum)) != (LinkState_pre & (1<<PortNum)) ) && ((LinkState & (1<<PortNum))==0))
			{
	
				rtl865x_tblAsicDrv_multiCastParam_t asic_mc;
				
				for( eindex = 0; eindex < RTL8651_IPMULTICASTTBL_SIZE; eindex++ ) {
					// by eindex 0 to 63 , if can't found go on by next
					if (rtl8651_getAsicIpMulticastTable(eindex,  &asic_mc) == FAILED)
						continue;

					/*when source port is plug-out*/ 
					if (asic_mc.port == PortNum)
					{						
						br_portlist_update(asic_mc.dip ,PortNum ,1);
						//panic_printk("source port be removed ; (port:%d;index=%d)!!\n", PortNum , eindex);						
						//dump_multicast_table();		
						break;
					}

					/*when client port is plug-out*/ 
					if(asic_mc.mbr & (1<<PortNum)){						
						br_portlist_update(asic_mc.dip , PortNum ,0);
						//panic_printk("some client gone; (port:%d;index=%d)!!\n",PortNum,eindex);	
						//dump_multicast_table();	
						break;
					}	
					
				}


			
			}
		}		
	}
	
	LinkState_pre = LinkState;

	
}
	
#endif



//---------------------------------------------------------------------------
/* static */

/* 
#ifdef CONFIG_RTK_MESH

extern void br_signal_pathsel(void);

#if 0


#include "rtl8651_layer2.h" 


void interrupt_link_state_for_mesh()
{

	static int LinkState_init = 0;
	static int LinkState_pre = 0;
	int LinkState = 0;
	char tmp = 0x0;
	int PortNum;

	//check port1~port4
	for( PortNum = 1; PortNum < 5; PortNum ++ ) {
		rtl8651_getAsicEthernetLinkStatus(PortNum , &tmp);
		//if this port is up then return TURE
		if( tmp )
			LinkState |= ( 1 << PortNum );
	}

	if(LinkState_init == 0 ){
		LinkState_pre = LinkState;
		LinkState_init = 1;
		return;
	}

	
	if(LinkState_pre != LinkState){
		
		for( PortNum = 1; PortNum < 5; PortNum ++ ) {
			//just check 1->0 case
			if(( (LinkState & (1<<PortNum)) != (LinkState_pre & (1<<PortNum)) ))
			{
				if (LinkState == 0) {
					if (LinkState_pre > 0)
						br_signal_pathsel();
				}
				else {
					if (LinkState_pre == 0)
						br_signal_pathsel();	
				}
			}
		}		
	}
	
	LinkState_pre = LinkState;

	
}
#endif

#endif
*/

void refill_rx_skb(void)
{
	struct sk_buff *skb;

	while (rx_skb_queue.qlen < MAX_PRE_ALLOC_RX_SKB) {

#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
		skb = dev_alloc_skb_priv_eth(CROSS_LAN_MBUF_LEN);
#else
		skb = dev_alloc_skb(CROSS_LAN_MBUF_LEN);
#endif
		if (skb == NULL) { 
//			DEBUG_ERR("EthDrv: dev_alloc_skb() failed!\n");		
			return;
		}
		skb_reserve(skb, RX_OFFSET);	
#ifdef RTK_QUE
		rtk_queue_tail(&rx_skb_queue, skb);
#else		
		__skb_queue_tail(&rx_skb_queue, skb);
#endif
	}
}

//---------------------------------------------------------------------------
static void free_rx_skb(void)
{
	struct sk_buff *skb;

	swNic_freeRxBuf();

	while  (rx_skb_queue.qlen > 0) {
#ifdef RTK_QUE
		skb = rtk_dequeue(&rx_skb_queue);
#else
		skb = __skb_dequeue(&rx_skb_queue);
#endif
		dev_kfree_skb_any(skb);
	}
}

//---------------------------------------------------------------------------
void *UNCACHED_MALLOC(int size) 
{
	return ((void *)(((uint32)kmalloc(size, GFP_KERNEL)) | UNCACHE_MASK));	
}

//---------------------------------------------------------------------------
__IRAM_SECTION_
__MIPS16
unsigned char *alloc_rx_buf(void **skb, int buflen)
{
	struct sk_buff *new_skb;

	if (rx_skb_queue.qlen == 0) {
#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
		new_skb = dev_alloc_skb_priv_eth(CROSS_LAN_MBUF_LEN);
#else        
		new_skb = dev_alloc_skb(CROSS_LAN_MBUF_LEN);
#endif
		if (new_skb == NULL) { 
			DEBUG_ERR("EthDrv: dev_alloc_skb() failed!\n");		
		}
		else 
			skb_reserve(new_skb, RX_OFFSET);	
	}
	else {
#ifdef RTK_QUE
		new_skb = rtk_dequeue(&rx_skb_queue);
#else
		new_skb = __skb_dequeue(&rx_skb_queue);
#endif
	}
	
	if (new_skb == NULL) 
		return NULL;	
	*skb = new_skb;

	#ifdef RTL865X_RX_RUNOUT_BUG
	/* store the skb pointer in a DW in front of  new_skb->data, it will be used in swNic_receive() */
	*(uint32 *)(new_skb->data-6) = (uint32)(new_skb);
	#endif
	
	return new_skb->data;	
}

//---------------------------------------------------------------------------
void free_rx_buf(void *skb)
{
	dev_kfree_skb_any((struct sk_buff *)skb);
}

//---------------------------------------------------------------------------
void tx_done_callback(void *skb)
{
//	dev_kfree_skb_irq((struct sk_buff *)skb);	
	dev_kfree_skb_any((struct sk_buff *)skb);
}

#ifdef CONFIG_RTL865X_HW_PPTPL2TP
void rtl865x_pptpl2tp_extport_recv(struct sk_buff *skb)
{
	struct ethhdr *eth;
	unsigned char mac[] = { 0x00, 0xe0, 0x4c, 0x00, 0x00, 0x01 };
	
	eth = (struct ethhdr *)skb->data;
	if (!memcmp(eth->h_dest, mac, 6)) {
		skb->cb[0] = 'P'; skb->cb[1] = 'P'; skb->cb[2] = 'P';
	}
	else {
		skb->cb[0] = 'N'; skb->cb[1] = 'A'; skb->cb[2] = 'T';
	}
	skb->protocol = eth_type_trans(skb, skb->dev);
	if (skb->pkt_type == PACKET_OTHERHOST)
		skb->pkt_type = PACKET_HOST;
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	netif_rx(skb);			
}

void rtl865x_pptpl2tp_extport_xmit(struct sk_buff *skb)
{
	unsigned int vid, port;
	
	vid = 10;
	port = BIT(7);
    if (swNic_send((void *)skb, skb->data, skb->len, vid, port) < 0) {   
		dev_kfree_skb_irq(skb);
    }	
}
#endif

//---------------------------------------------------------------------------
#ifdef BR_SHORTCUT
#if CONFIG_WIRELESS_LAN_MODULE
 #ifdef PERF_DUMP
extern  int32 (*Fn_rtl8651_romeperfEnterPoint)( uint32 index );
extern  int32  (*Fn_rtl8651_romeperfExitPoint)(uint32 index);
EXPORT_SYMBOL(Fn_rtl8651_romeperfEnterPoint);
EXPORT_SYMBOL(Fn_rtl8651_romeperfExitPoint);
 #endif
extern struct net_device* (*wirelessnet_hook_shortcut)(unsigned char *da);
EXPORT_SYMBOL(wirelessnet_hook_shortcut);
struct net_device* (*wirelessnet_hook_shortcut)(unsigned char *da)=NULL;
#endif
#endif

//---------------------------------------------------------------------------
__IRAM_SECTION_
__MIPS16
static void interrupt_dsr_rx(unsigned long task_priv)
{
	struct re_private *cp = (struct re_private *)task_priv;
	struct re_private *cp_next;
	struct re_private *cp_this;
	struct sk_buff *skb;
 	int len;
 	unsigned int vid,pid;
	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	struct re_private *cp2;
	int i2;
	#endif
#ifdef PREVENT_BCAST_REBOUND	
	unsigned char* DA;
	unsigned long  current_time = jiffies;
	struct bcast_tr_s *bacst_Ptr = &bcast ;	
#endif
	
#ifdef CONFIG_RTK_VOIP
 	unsigned long start_time = jiffies;
#endif
	unsigned rx_work = 10000;
	
#ifdef BR_SHORTCUT
#if CONFIG_WIRELESS_LAN_MODULE
#else
	extern struct net_device *get_shortcut_dev(unsigned char *da);
#endif
	struct net_device *dev;
#endif	

	cp_next = NEXT_CP(cp);

	while (rx_work--) {   		
#ifdef CONFIG_RTK_VOIP
		extern char chanEnabled[2];

		if ( (jiffies - start_time) > 1 && (chanEnabled[0] || chanEnabled[1]) ){
#ifdef RX_TASKLET		
			tasklet_hi_schedule(&cp->rx_dsr_tasklet);			
#endif	
			break;
		}
#endif
		if (swNic_receive((void **)&skb, &len, &vid, &pid) !=  0)			
	        	break;

		// Verify IP/TCP/UDP checksum
		if (skb->ip_summed != 0) {
#ifdef CONFIG_RTL8196B_KLD   
	if ((vid != WAN_VID) || 
    	(((ntohl(*(unsigned short *)(skb->data + 12)) != 0x0800) || (*(skb->data + 23) != 1 /* IPPROTO_ICMP */)) &&
     	((ntohl(*(unsigned short *)(skb->data + 12)) != 0x8864) || (*(skb->data + 31) != 1 /* IPPROTO_ICMP */)))
      )
 #endif
 		{
			dev_kfree_skb_any(skb);
			continue;
		}
}

#ifdef CONFIG_RTK_VLAN_SUPPORT
	if (L2_table_disabled) {		
		for (i2=0; i2<5; i2++) {			
			cp2 = reNet[i2]->priv;	
			if (cp2->opened && cp2->port == pid) {
				cp_this = cp2;
				break;
			}			
		}
		if (i2 != 5) 
			goto rx_packet;					
		else {
			dev_kfree_skb_any(skb);
			continue;			
		}		
	}
#endif

#ifdef CONFIG_RTL865X_HW_TABLES
		/* for the issue: WAN port PC can not ping br0 (192.168.1.254) in Bridge mode */
    		if (cp->opened && (cp->port & pid))				
       	 		cp_this = cp;
		else if (cp_next->opened && (cp_next->port & pid))
			cp_this = cp_next;
		#ifdef CONFIG_RTL865X_HW_PPTPL2TP
		//else if (vid == 10)
		else if (pid & BIT(7))
			cp_this = cp;
		#endif
	    	else {    
			dev_kfree_skb_any(skb);
			continue;
    		}
#else
    		if (cp->opened && cp->vid==vid)				
       	 		cp_this = cp;
		else if (cp_next->opened && cp_next->vid==vid)
			cp_this = cp_next;
		#ifdef CONFIG_RTK_VOIP_WAN_VLAN         
	    	else if (cp_next->opened && vid==wan_vlan_id_proto)
			cp_this = cp_next;
		else if (cp_next->opened && vid==wan_vlan_id_video)
			cp_this = cp_next;
		else if (cp_next->opened && vid==wan_vlan_id_data)
			cp_this = cp_next;
		#endif	    	
	    	else {    
			#if defined(CONFIG_RTK_GUEST_ZONE)
			if (L2_table_disabled) {
				for (i2 = 2; i2 < 5; i2++) {
					cp2 = reNet[i2]->priv;
					if (cp2->opened && cp2->vid == vid) {
						cp_this = cp2;
						break;
					}
				}
				if (i2 >= 5) {
					dev_kfree_skb_any(skb);
					continue;
				}				
			}
			else
			#endif
			{
			dev_kfree_skb_any(skb);
			continue;
			}
    		}
#if !defined(CONFIG_RTL8197B_PANA) && !defined(CONFIG_RTL865X_PANAHOST)
		if  (memcmp(&skb->data[6], cp_this->dev->dev_addr, 6)== 0)// check source mac
		{
                        //printk("source mac is device self\n");
                        dev_kfree_skb_any(skb);
                       continue;
		}
#endif
#endif

#ifdef CONFIG_RTK_VLAN_SUPPORT
rx_packet:
#endif
		skb_put(skb, len);
   		skb->dev = cp_this->dev;
#ifdef CONFIG_RTL8196_RTL8366
		if (!memcmp(cp_this->dev->dev_addr, &skb->data[6], 6)) {
			dev_kfree_skb_any(skb);
			continue;
		}
		if (*((unsigned short *)(skb->data+ETH_ALEN*2)) == __constant_htons(ETH_P_8021Q)) 
		{
			uint8 *data = skb->data;
			memmove(data + VLAN_HLEN, data, 2 * VLAN_ETH_ALEN);
			skb->len -= 4;
			skb->data += 4;

		}

		
#endif
   		

#ifdef CONFIG_RTK_VOIP_WAN_VLAN         
		if (*(uint16*)(&(skb->data[12])) == htons(0x8100))
		{
			//printk("Get VLAN tagged packet\n");
			memmove(
				skb->data+4,
				skb->data,
				2*sizeof(ether_addr_t)
			);
			skb_pull(skb, 4);
		}
#endif	

#ifdef CONFIG_RTL865X_HW_PPTPL2TP
		if (pid & BIT(7)) {
			rtl865x_pptpl2tp_extport_recv(skb);
			continue;
		}
		skb->cb[0] = '\0';
#endif

		cp_this->net_stats.rx_packets++;	
		cp_this->net_stats.rx_bytes += skb->len;	
		cp_this->dev->last_rx = jiffies;
		
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR)
		cp_this->rx_byte_cnt += skb->len;
#endif
#if defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
		cp_this->rx_byte_cnt += skb->len;
#endif

#if !defined(CONFIG_RTL8197B_PANA) && !defined(CONFIG_RTL865X_PANAHOST)
		// drop the packet that SA is my MAC addr
		if (!memcmp(cp_this->dev->dev_addr, &skb->data[6], 6)) {
			dev_kfree_skb_any(skb);
			continue;
		}
#endif

#ifdef PREVENT_BCAST_REBOUND
	DA = skb->data;
	if(DA[0] & 0x01){
			int i3;
			int found = 0;			
			//panic_printk("\n<===eth rx===\n");						
			for(i3=0 ; i3 < P_BCAST_NUM ; i3++){				
				if (memcmp(&skb->data[6], &bacst_Ptr->entry[i3].BCAST_SA , 6)== 0
					&& (UINT32_DIFF(current_time , bacst_Ptr->entry[i3].time_stamp) < 3000 ))
				{
                       //panic_printk("BROADCAST packet rebound ; this source mac must be filter\n");
                       dev_kfree_skb_any(skb);
					   found = 1;
					   break;	

				}
			}			
			if(found == 1)	
				continue;

		
	}
#endif	

#ifdef CONFIG_RTK_VLAN_SUPPORT
	if (cp_this->vlan_setting.global_vlan) {
		if (rx_vlan_process(cp_this->dev, &cp_this->vlan_setting, skb)){
			cp_this->net_stats.rx_dropped++;
			dev_kfree_skb_any(skb);		
			continue;
		}
	}
#endif

#if CONFIG_WIRELESS_LAN_MODULE
		if (enable_brsc && !(skb->data[0] & 0x01) &&
				(eth_flag & BIT(2))  &&
				(cp_this->dev->br_port) ) {
			if(wirelessnet_hook_shortcut!=NULL)
			{
			    if(((dev = wirelessnet_hook_shortcut(skb->data)) != NULL))
				{
					memcpy(cached_eth_addr, &skb->data[6], 6);
					cached_dev = cp_this->dev;
					dev->hard_start_xmit(skb, dev);
				}
			    else
				{
				    goto NO_BR;
				}
			}
			else
			{
			   goto NO_BR;
			}


		}
		else
#else
#ifdef BR_SHORTCUT

		if (enable_brsc && !(skb->data[0] & 0x01) &&
				(eth_flag & BIT(2))  &&
				(cp_this->dev->br_port) &&
				((dev = get_shortcut_dev(skb->data)) != NULL)) {
			memcpy(cached_eth_addr, &skb->data[6], 6);
			cached_dev = cp_this->dev;				
			skb->dev = dev;
			dev->hard_start_xmit(skb, dev);	
		}
		else
#endif
#endif
		{
#ifdef CONFIG_RTL8186_KB
			skb->__unused = 0;
#endif

#ifdef DYNAMIC_ADJUST_TASKLET
			if (rx_pkt_thres > 0 && cp_this->dev->name[3] == '0') // eth0
				rx_cnt++;
#endif			

#ifdef BR_SHORTCUT
			pkt_cnt++;
#endif

			skb->protocol = eth_type_trans(skb, cp_this->dev);
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			skb->cb[3] = (char)pid; // add for IGMP snooping, david+2008-11-05
			skb->cb[4] = (char)cp_this->vid; // add for IGMP snooping, david+2008-11-05

#ifdef SUPPORT_INPUT_PKT_QUEUE_VOIP
			if (skb->data[1]) //DSCP!=0
			{ 
				netif_rx_voip(skb);
			} 
			else
			{
			netif_rx(skb);			
		}
#else

			netif_rx(skb);			
#endif
		}
    	}

#ifdef RX_TASKLET
    	REG32(CPUIIMR) |= RX_DONE_IE_ALL;
#endif
}

//---------------------------------------------------------------------------
static void interrupt_dsr_tx(unsigned long task_priv)
{
    struct re_private *cp = (struct re_private *)task_priv;
	int free_desc_num;
	unsigned long flags;


	refill_rx_skb();

	save_and_cli(flags);

	free_desc_num = swNic_txDone(); 

	restore_flags(flags);

    if (free_desc_num >= (txRingSize[0]/4)) {
		if (cp->opened && netif_queue_stopped(cp->dev)) 			
			netif_wake_queue(cp->dev);
		
		if (NEXT_CP(cp)->opened && netif_queue_stopped(NEXT_DEV(cp)))
			netif_wake_queue(NEXT_DEV(cp));			
   	}
		
#ifdef TX_TASKLET		
    REG32(CPUIIMR) |= TX_ALL_DONE_IE_ALL;		
#endif

}

//---------------------------------------------------------------------------
static void interrupt_isr(int irq, void *dev_instance, struct pt_regs *regs)
{
    struct net_device *dev = dev_instance;
    struct re_private *cp = dev->priv;
	unsigned int status;

	status =*(volatile unsigned int*)(CPUIISR); 
    *(volatile unsigned int *)(CPUIISR) = status; 

    if (!status || (status == 0xFFFF)) {
		spin_unlock(&cp->lock);			
		return;
	}

#if	0	//def CONFIG_RTK_PORT_HW_IGMPSNOOPING
    if (status & LINK_CHANGE_IP) {
		//printk("%s: link changed.\n",dev->name);
		interrupt_link_state_for_multicast((unsigned long)cp);
    }
#endif

/*
#ifdef CONFIG_RTK_MESH
	if (status & LINK_CHANGE_IP) {
		//printk("%s: link changed.\n",dev->name);
		interrupt_link_state_for_mesh();
	}
#endif
*/

    if (status & PKTHDR_DESC_RUNOUT_IP_ALL) {
		DEBUG_ERR("EthDrv: packet RUNOUT error!\n");
		//*(volatile unsigned int *)(CPUIIMR)|=PKTHDR_DESC_RUNOUT_IE_ALL;  
		//*(volatile unsigned int *)(CPUIISR)=PKTHDR_DESC_RUNOUT_IP_ALL;   
    }

	if (status & (RX_DONE_IP_ALL |PKTHDR_DESC_RUNOUT_IP_ALL)) {
#ifdef RX_TASKLET		
		*(volatile unsigned int *)(CPUIIMR) &= ~RX_DONE_IE_ALL;			
			tasklet_hi_schedule(&cp->rx_dsr_tasklet);			
#else	
		interrupt_dsr_rx((unsigned long)cp);	
#endif
	}		

	if (status &TX_ALL_DONE_IP_ALL) {
#ifdef TX_TASKLET		
		*(volatile unsigned int *)(CPUIIMR) &= ~TX_ALL_DONE_IP_ALL;
		tasklet_schedule(&cp->tx_dsr_tasklet);						
#else
		interrupt_dsr_tx((unsigned long)cp);
#endif			
	}			
}

//----------------------------------------------------------------------------
__IRAM_SECTION_
__MIPS16
static int rtl865x_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
//	unsigned long flags;
 	struct re_private *cp = dev->priv;
/*prevent boardcast packet turn round */
#ifdef PREVENT_BCAST_REBOUND	
	unsigned  char *DA ;
	struct bcast_tr_s *bacst_Ptr = &bcast ;
#endif

#if !defined(CONFIG_RTL8197B_PANA) && !defined(CONFIG_RTL865X_PANAHOST)
	if (!reNet[0] || !((struct re_private *)reNet[0]->priv)->opened) {
		cp->net_stats.tx_dropped++;
		dev_kfree_skb_any(skb);		
		return 0;	
	}
#endif

#ifdef CONFIG_RTK_VLAN_SUPPORT
	if (cp->vlan_setting.global_vlan) {
		if (tx_vlan_process(dev, &cp->vlan_setting, skb, 0)) {
			cp->net_stats.tx_dropped++;
			dev_kfree_skb_any(skb);		
			return 0;				
		}
	}
#endif

#ifdef	PREVENT_BCAST_REBOUND
	DA = skb->data;
	if(DA[0] & 0x01){

			//unsigned char* DDA;
			//panic_printk("\n====eth tx===>\n");			

			PBR_index %= P_BCAST_NUM;
			memcpy(&bacst_Ptr->entry[PBR_index].BCAST_SA , DA+6 ,6);
			bacst_Ptr->entry[PBR_index].time_stamp = jiffies ;
			//DDA = &bacst_Ptr->entry[PBR_index].BCAST_SA ;						
			PBR_index ++ ;
								
			//panic_printk("saved mac:%02X:%02X:%02X-%02X:%02X:%02X\n",
			//	DDA[0],DDA[1],DDA[2],DDA[3],DDA[4],DDA[5]);			
			//panic_printk("source mac:%02X:%02X:%02X-%02X:%02X:%02X\n",
			//	DA[6],DA[7],DA[8],DA[9],DA[10],DA[11]);

	}
#endif

//	spin_lock_irqsave (&cp->lock, flags);
    dev->trans_start = jiffies;

#ifdef CONFIG_RTL8196_RTL8366
	if (*((unsigned short *)(skb->data+ETH_ALEN*2)) != __constant_htons(ETH_P_8021Q)) 
	{

		{
		if (skb_headroom(skb) < 4 && skb_cow(skb, 4) !=0 )
			{
				printk("%s-%d: error! (skb_headroom(skb) == %d < 4). Enlarge it!\n",
				__FUNCTION__, __LINE__, skb_headroom(skb));
				while (1) ;
			}
		skb_push(skb, VLAN_HLEN);

		memmove(skb->data, skb->data + VLAN_HLEN, 2 * VLAN_ETH_ALEN);
		*(uint16*)(&(skb->data[12])) = __constant_htons(ETH_P_8021Q);		/* TPID */
		*(uint16*)(&(skb->data[14])) = htons(cp->id);			/* VID */
		*(uint8*)(&(skb->data[14])) &= 0x0f;					/* clear most-significant nibble of byte 14 */
//		*(uint8*)(&(skb->data[14])) |= priorityField;
		}

		skb->cb[9] = 1;
	}
#endif

    if (swNic_send((void *)skb, skb->data, skb->len, cp->vid, cp->port) < 0) {	// tx queue full
		DEBUG_ERR("%s: tx failed!\n", dev->name);
			
		netif_stop_queue(dev);
#ifdef CONFIG_RTK_VOIP
		// avoid packet lost!
		struct Qdisc *q=dev->qdisc;
		q->ops->requeue(skb, q);
		netif_schedule(dev);		
		return 0;
#else		
		cp->net_stats.tx_dropped++;
//		dev_kfree_skb_irq(skb);		
		dev_kfree_skb_any(skb);		

//		spin_unlock_irqrestore(&cp->lock, flags);		
		//return 1;
		/* tx queue full and drop this packet, return 0 to indicate the caller that the packet is done.
		 * return 1 the caller will resend it again and cause core dump (skb->list != NULL) in __kfree_skb().
		 */
		return 0;
#endif		
    }
	cp->net_stats.tx_packets++;		
	cp->net_stats.tx_bytes += skb->len;
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
		cp->tx_byte_cnt += skb->len;
#endif		

#ifdef BR_SHORTCUT
	pkt_cnt++;
#endif

//	spin_unlock_irqrestore(&cp->lock, flags);   
    return 0;
}

//---------------------------------------------------------------------------
static struct net_device_stats *rtl865x_get_stats(struct net_device *dev)
{
    struct re_private *cp = dev->priv;

#ifdef CONFIG_RTL865X_HW_TABLES		
	extern int gHwNatEnabled;

	if (gHwNatEnabled > 0) {
		int vid, port;
		uint32 val;
		
		for (vid=0; vid<2; vid++) {
			if (!memcmp(vlanSetting[vid].mac, dev->dev_addr, 6))
				break;
		}
		if (vid == 2) {
			printk("%s: get statistics error!\n", __FUNCTION__);
			return NULL;
		}
		memset(&cp->net_stats,  '\0', sizeof(struct net_device_stats));

		if (!netif_running(dev))
			goto ret;
		for (port=0; port<MAX_PORT_NUMBER; port++) {	
			if 	(vlanSetting[vid].portmask & (1<<port)) {
				val = REG32(MIB_COUNTER_BASE+ 0x108 + 0x80*port);		
				if (val > 0) 
					cp->net_stats.rx_packets += val;

				val = REG32(MIB_COUNTER_BASE+ 0x13c + 0x80*port);	
				if (val > 0) 
					cp->net_stats.rx_packets += val;

				val = REG32(MIB_COUNTER_BASE+ 0x140 + 0x80*port);	
				if (val > 0) 
					cp->net_stats.rx_packets += val;
				
				val = REG32(MIB_COUNTER_BASE+ 0x100 + 0x80*port);
				if (val > 0) 
					cp->net_stats.rx_bytes += val;								

				val = REG32(MIB_COUNTER_BASE+ 0x808 + 0x80*port);		
				if (val > 0)
					cp->net_stats.tx_packets += val;

				val = REG32(MIB_COUNTER_BASE+ 0x80c + 0x80*port);		
				if (val > 0)
					cp->net_stats.tx_packets += val;
				
				val = REG32(MIB_COUNTER_BASE+ 0x810 + 0x80*port);		
				if (val > 0)
					cp->net_stats.tx_packets += val;

				val = REG32(MIB_COUNTER_BASE+ 0x800 + 0x80*port);		
				if (val > 0)
					cp->net_stats.tx_bytes += val;				
			}		
		}				
	}

ret:	
#endif

#ifdef CONFIG_RTL8196B_TLD
	if (cp->opened) {
		int i;

		cp->net_stats.collisions = 0;
		for (i=0; i<MAX_PORT_NUMBER; i++) {			
			if (cp->port & BIT(i)) {
				cp->net_stats.collisions += READ_MEM32(MIB_COUNTER_BASE+ OFFSET_ETHERSTATSCOLLISIONS_P0 + MIB_ADDROFFSETBYPORT*i);
			}			
		}
	}
#endif

    return &cp->net_stats;	
}

/*
#ifdef CONFIG_RTK_MESH
int check_LAN_linkstatus_any(void)
{
	unsigned int port_num=0;
	
#ifdef CONFIG_RTL8196_RTL8366
	int link_status=0;
    
	for (port_num = 0; port_num < 4; port_num++){
		rtl8366rb_getPHYLinkStatus (port_num, &link_status);//PHY4
		if (link_status==1)
			return 1;
	}

#else
	for (port_num = 1; port_num < 5; port_num++){
		if(READ_MEM32(PSRP0 + (port_num) * 4) & PortStatusLinkUp){	
			//printk("port %d plugged!!\n", port_num);
			return 1;
		}
		//printk("port %d unplug\n", port_num);
	}
#endif

	return 0;
}
#endif

*/

//---------------------------------------------------------------------------
static void rtl865x_stop_hw(struct net_device *dev, struct re_private *cp)
{
	swCore_down();
}

#ifdef CONFIG_RTL8214_SUPPORT
extern unsigned int ExtP1P4GigaPhyMode;
#endif			

#ifdef CONFIG_RTL_KERNEL_MIPS16_DRVETH
__NOMIPS16
#endif 
int re865x_ioctl (struct net_device *dev, struct ifreq *rq, int cmd)
{
	int32 rc = 0;
	unsigned long *data;
	int32 args[4];
	int32  * pRet;
	uint32 statCtrlReg, phyId = wan_port;
	unsigned int port_num=0;
	if (cmd != SIOCDEVPRIVATE)
	{
		goto normal;
	}

	data = (unsigned long *)rq->ifr_data;

	if (copy_from_user(args, data, 4*sizeof(unsigned long)))
	{
		return -EFAULT;
	}

	switch (args[0])
	{
		case RTL8651_IOCTL_GETWANLINKSTATUS:
            pRet = (int32 *)args[3];
			*pRet = FAILED;
			rc = SUCCESS;
            #ifdef CONFIG_RTL8196_RTL8366
            {
                int link_status=0;
                /*Get link status of PHY 1 */
                rtl8366rb_getPHYLinkStatus (4, &link_status);//PHY4
		if(link_status)
                {
                    *pRet = SUCCESS;
                }
            }
            #else
			if(READ_MEM32(PSRP0 + (wan_port) * 4) & PortStatusLinkUp){
				*pRet = SUCCESS;
			}
            #endif
			break;	
		case RTL8651_IOCTL_GETLANLINKSTATUS:
			pRet = (int32 *)args[3];
			
			rc = SUCCESS;
			memcpy(&port_num, pRet, 4);
			if(READ_MEM32(PSRP0 + (port_num) * 4) & PortStatusLinkUp){	
				*pRet = SUCCESS;
			}else{
				*pRet = FAILED;
			}
			break;			
/*
#ifdef CONFIG_RTK_MESH
		case RTL8651_IOCTL_GETLANLINKSTATUSALL:
			pRet = (int32 *)args[3];
			*pRet = FAILED;
			
			rc = SUCCESS;
			if (check_LAN_linkstatus_any()!=0)
				*pRet = SUCCESS;
			
			break;			
#endif
*/
		case RTL8651_IOCTL_GETLANLINKSPEED:
			pRet = (int32 *)args[3];
			rc = FAILED;
			memcpy(&port_num, pRet, 4);
			switch(READ_MEM32(PSRP0 + (port_num) * 4) & PortStatusLinkSpeed_MASK)
			{
				case PortStatusLinkSpeed10M:
					*pRet = PortStatusLinkSpeed10M;
					rc = SUCCESS;
					break;
				case PortStatusLinkSpeed100M:
					*pRet = PortStatusLinkSpeed100M;
					rc = SUCCESS;
					break;
				case  PortStatusLinkSpeed1000M:
					*pRet = PortStatusLinkSpeed1000M;
					rc = SUCCESS;
					break;
				default:
					break;
			}
			break;	
			
			
		case RTL8651_IOCTL_GETETHERLINKDUPLEX:
			pRet = (int32 *)args[3];
			rc = SUCCESS;
			memcpy(&port_num, pRet, 4);
			if(READ_MEM32(PSRP0 + (port_num) * 4) & PortStatusDuplex){	
				*pRet = SUCCESS;
			}else{
				*pRet = FAILED;
			}
			break;	
			
			
			
		case RTL8651_IOCTL_GETWANLINKSPEED:
			pRet = (int32 *)args[3];
			*pRet = FAILED;
			rc = FAILED;
			
			switch(READ_MEM32(PSRP0 + (wan_port) * 4) & PortStatusLinkSpeed_MASK)
			{
				case PortStatusLinkSpeed10M:
					*pRet = PortStatusLinkSpeed10M;
					rc = SUCCESS;
					break;
				case PortStatusLinkSpeed100M:
					*pRet = PortStatusLinkSpeed100M;
					rc = SUCCESS;
					break;
				case  PortStatusLinkSpeed1000M:
					*pRet = PortStatusLinkSpeed1000M;
					rc = SUCCESS;
					break;
				default:
					break;
			}
			break;
		
		case RTL8651_IOCTL_SETWANLINKSPEED:
			/* 
				*(int32 *)args[3]: 0: Auto, 0b1000: force to 10M, Auto, 0b1001: force to 100M, Auto, 0b1010: force to Giga, 
			 */

			pRet = (int32 *)args[3];
			// bit23: 0:auto; 1:force
			// bit21:bit20 10M:00, 100M:01, 1000M:10
			//REG32(PCRP4) = (REG32(PCRP4) & (~0x00b00000) | (*pRet<<20));			

			if (wan_port == 5) /* 8211 */
				phyId = GIGA_P5_PHYID;
			
			#ifdef CONFIG_RTL8214_SUPPORT
			if (ExtP1P4GigaPhyMode && (wan_port>=1) && (wan_port <=4)) 
				phyId = CONFIG_RTL8214_PHYID_P1+wan_port-1;
			#endif
			
			rtl8651_getAsicEthernetPHYReg( phyId, 4, &statCtrlReg );
			statCtrlReg &= ~(CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD);
			if (*pRet == 8)
				statCtrlReg |= (CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD);
			else if (*pRet == 9)
				statCtrlReg |= (CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD);
			else
				statCtrlReg |= (CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD);
				
			rtl8651_setAsicEthernetPHYReg( phyId, 4, statCtrlReg );
			rtl8651_getAsicEthernetPHYReg( phyId, 0, &statCtrlReg );

			statCtrlReg |= RESTART_AUTONEGO;
			rtl8651_setAsicEthernetPHYReg( phyId, 0, statCtrlReg );				

			rc = SUCCESS;	
			break;
			
			
		default:
			rc = SUCCESS;
			break;
	}

	return rc;
normal:	
	switch (cmd)
        {
	    default:
		rc = -EOPNOTSUPP;
		break;
	}
	return rc;			

}

#ifdef CONFIG_RTK_CHECK_ETH_TX_HANG
static int rtl865x_close(struct net_device *dev);
static int rtl865x_open(struct net_device *dev);
extern int32 free_pending_tx_skb(void);
int check_tx_desc_hang(void);
static int tx_hang_cnt = 0;
static struct net_device_stats tmp_net_stats[ETH_INTF_NUM];
static int if_opend[ETH_INTF_NUM];

static void reinit_eth_intf(void)
{
	uint32 flags;
	struct re_private *cp;
	int i;

	save_and_cli(flags);

	for (i=0; i<ETH_INTF_NUM; i++) {
		cp = (struct re_private *)reNet[i]->priv;

		if_opend[i] = cp->opened;
		if (if_opend[i]) {
			memcpy(&tmp_net_stats[i], &cp->net_stats, sizeof(struct net_device_stats)); 
			rtl865x_close(reNet[i]); 
		}
	}
	
	free_pending_tx_skb();

	REG32(PCRP0) = REG32(PCRP0) & ~EnablePHYIf;	/* Disable PHY interface. */
	REG32(PCRP1) = REG32(PCRP1) & ~EnablePHYIf;	/* Disable PHY interface. */
	REG32(PCRP2) = REG32(PCRP2) & ~EnablePHYIf;	/* Disable PHY interface. */
	REG32(PCRP3) = REG32(PCRP3) & ~EnablePHYIf;	/* Disable PHY interface. */
	REG32(PCRP4) = REG32(PCRP4) & ~EnablePHYIf;	/* Disable PHY interface. */

	/* Queue Reset Register */
	REG32(QRR) = QRST;
	mdelay(10);        
	REG32(QRR) = 0;
	mdelay(10);        

	REG32(SSIR) = 0;
	mdelay(10);        
	REG32(SSIR) = SwitchSemiRst;
	mdelay(10);        
	REG32(SSIR) = TRXRDY;
	mdelay(10);        
		   
	for (i=0; i<ETH_INTF_NUM; i++) {

		if (if_opend[i]) {
			rtl865x_open(reNet[i]); 
			cp = (struct re_private *)reNet[i]->priv;
			memcpy(&cp->net_stats, &tmp_net_stats[i], sizeof(struct net_device_stats)); 
		}
	}

	//REG32(GISR) = LX0_BFRAME_IP;		// write clear for bit 1 "LBC 0 bus frame time-out interrupt pending flag"

	tx_hang_cnt++;
	restore_flags(flags);
}
#endif

//---------------------------------------------------------------------------
#if defined(DYNAMIC_ADJUST_TASKLET) || defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(BR_SHORTCUT) || defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD) //|| defined(CONFIG_RTK_MESH)
static void one_sec_timer(unsigned long task_priv)
{
	uint32 flags;
	struct re_private *cp_next;
    	struct re_private *cp = ((struct net_device *)task_priv)->priv;
	cp_next = NEXT_CP(cp);
	//spin_lock_irqsave (&cp->lock, flags);
	save_and_cli(flags);

#ifdef DYNAMIC_ADJUST_TASKLET
	if (((struct net_device *)task_priv)->name[3] == '0' && rx_pkt_thres > 0 && 
		((eth_flag&TASKLET_MASK) == 0))  { 
		if (rx_cnt > rx_pkt_thres) {
			if (!rx_tasklet_enabled) {
				rx_tasklet_enabled = 1;
			}							
		}               
		else {
			if (rx_tasklet_enabled) { // tasklet enabled
				rx_tasklet_enabled = 0;
			}						
		}       
		rx_cnt = 0;     
	}   
#endif
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
	//eth0
	cp->tx_avarage = (cp->tx_avarage/10)*7 + (cp->tx_byte_cnt/10)*3;
	if (cp->tx_avarage > cp->tx_peak)
		cp->tx_peak = cp->tx_avarage;
	cp->tx_byte_cnt = 0;
    
	cp->rx_avarage = (cp->rx_avarage/10)*7 + (cp->rx_byte_cnt/10)*3;
	if (cp->rx_avarage > cp->rx_peak)
		cp->rx_peak = cp->rx_avarage;
	cp->rx_byte_cnt = 0;   
	//eth1
	cp_next->tx_avarage = (cp_next->tx_avarage/10)*7 + (cp_next->tx_byte_cnt/10)*3;
	if (cp_next->tx_avarage > cp_next->tx_peak)
		cp_next->tx_peak = cp_next->tx_avarage;
	cp_next->tx_byte_cnt = 0;
    
	cp_next->rx_avarage = (cp_next->rx_avarage/10)*7 + (cp_next->rx_byte_cnt/10)*3;
	if (cp_next->rx_avarage > cp_next->rx_peak)
		cp_next->rx_peak = cp_next->rx_avarage;
	cp_next->rx_byte_cnt = 0;    
#endif

#ifdef BR_SHORTCUT
	if (pkt_cnt > 100)
		enable_brsc=1;
	else
		enable_brsc=0;
#endif

/*
#ifdef CONFIG_RTK_MESH

	static int link_state = 0;
	int current_ls;

	current_ls = check_LAN_linkstatus_any();
	if (current_ls!=link_state){
		br_signal_pathsel();
		link_state = current_ls;
	}
	
	
#endif
*/

	mod_timer(&cp->expire_timer, jiffies + 100);

	#ifdef CONFIG_RTK_CHECK_ETH_TX_HANG
	if (check_tx_desc_hang()) {
		reinit_eth_intf();
	}
	#endif
	
	//spin_unlock_irqrestore(&cp->lock, flags);   
	restore_flags(flags);					
}
#endif

//---------------------------------------------------------------------------
#ifdef CONFIG_RTL_KERNEL_MIPS16_DRVETH
__NOMIPS16
#endif 
static int rtl865x_set_hwaddr(struct net_device *dev, void *addr)
{
	unsigned long flags;
	int i;
	unsigned char *p;
#ifndef CONFIG_RTL865X_HW_TABLES
	rtl_netif_param_t np;
	int ret;
#endif

	p = ((struct sockaddr *)addr)->sa_data;

	save_flags(flags); cli();
	for (i = 0; i<6; ++i) {
		dev->dev_addr[i] = p[i];
	}

	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	i = dev->name[3] - '0';
	if ((i >=0) && (i <5))
		memcpy(vlanSetting[i].mac, dev->dev_addr, 6);

	#else
	if (!strcmp(dev->name, "eth0"))
		i = 0;
	else
		i = 1;	
	memcpy(vlanSetting[i].mac, dev->dev_addr, 6);
	#endif

#ifdef CONFIG_RTL865X_HW_TABLES /* Forrest */
	if (!strcmp(dev->name, "eth0"))
		rtl865x_updateNetIfTabMac("br0", p); /* Alias eth0 to br0 */
	else
		rtl865x_updateNetIfTabMac(dev->name, p);
#else
	swCore_init(-1); // reset interface table
	for (i=0; i<totalVlans; i++) {    
		bzero((void *) &np, sizeof(rtl_netif_param_t));
		np.vid = vlanSetting[i].vid;
		np.valid = 1;
		#ifdef CONFIG_HW_MULTICAST_TBL
		np.enableRoute = 1;
		#else
		np.enableRoute = 0;
		#endif
		np.inAclEnd = 0;
		np.inAclStart = 0;
		np.outAclEnd = 0;
		np.outAclStart = 0;
		memcpy(&np.gMac, &vlanSetting[i].mac[0], 6);                        
		np.macAddrNumber = 1;
		np.mtu = 1500;
		ret = swCore_netifCreate(i, &np);
		if ( ret != 0 ) {
			printk("865x-nic: swCore_netifCreate() failed:%d\n", ret );
			return -1;
		}
	}
#endif /* Forrest */
	restore_flags(flags);
	return 0;
}


//---------------------------------------------------------------------------
#ifdef CONFIG_RTL865X_HW_TABLES

#define INIT_CHECK(expr) do {\
    if(((int32)expr)!=SUCCESS){\
        rtlglue_printf("Error >>> %s:%d failed !\n", __FUNCTION__,__LINE__);\
            return FAILED;\
    }\
}while(0)

uint16 _fid = 1;

int re_init_vlanTable(int mode)
{
	#ifdef CONFIG_RTL8196B
	#ifdef CONFIG_RTL865X_WTDOG
	unsigned long wtval;
	#endif
	#endif
	
	if (mode == 0) { /* Gateway mode */
		lrConfig[0].memPort = ALL_PORTS & (~BIT(wan_port));
		lrConfig[0].untagSet = lrConfig[0].memPort;
		lrConfig[1].memPort = BIT(wan_port);
		lrConfig[1].untagSet = lrConfig[1].memPort;
		#ifndef CONFIG_RTL865X_HW_PPTPL2TP
		lrConfig[2].memPort = BIT(wan_port);
		lrConfig[2].untagSet = lrConfig[2].memPort;
		#endif
  
		lrConfig[0].fid = 1;
		_fid = 1;
		INIT_CHECK(rtl865x_lightRomeConfig());
		
	}
	else {
		#ifdef TWO_VLANS_IN_BRIDGE_MODE
			lrConfig[0].memPort = ALL_PORTS & (~BIT(wan_port));
			lrConfig[0].untagSet = lrConfig[0].memPort;
			lrConfig[1].memPort = BIT(wan_port) | CPU_PORT;
			lrConfig[1].untagSet = lrConfig[1].memPort;
			#ifndef CONFIG_RTL865X_HW_PPTPL2TP
			lrConfig[2].memPort = BIT(wan_port) | CPU_PORT;
			lrConfig[2].untagSet = lrConfig[2].memPort;
	  		#endif
			lrConfig[0].fid = 0;
			_fid = 0;
			INIT_CHECK(rtl865x_lightRomeConfig());

		#else

			lrConfig[0].memPort = ALL_PORTS;
			lrConfig[0].untagSet = lrConfig[0].memPort;
			lrConfig[1].memPort = 0;
			lrConfig[1].untagSet = 0;
			#ifndef CONFIG_RTL865X_HW_PPTPL2TP
			lrConfig[2].memPort = 0;
			lrConfig[2].untagSet = 0;
			#endif
	            
			lrConfig[0].fid = 0;
			_fid = 0;
			#ifdef CONFIG_RTL8196B

			#ifdef CONFIG_RTL865X_WTDOG
			wtval = *((volatile unsigned long *)WDTCNR);
			*((volatile unsigned long *)WDTCNR) = (WDSTOP_PATTERN << WDTE_OFFSET);
			#endif
			
			FullAndSemiReset();
			rtl8651_extDeviceinit();
			rtl865x_lightRomeInit(); 
			rtl865x_lightRomeConfig();
			//rtl8651_extDeviceInitTimer(); 
			#ifdef CONFIG_RTL865X_WTDOG
			*((volatile unsigned long *)WDTCNR) |=  1 << 23;
			*((volatile unsigned long *)WDTCNR) = wtval;
			#endif
			
			#else
			INIT_CHECK(rtl865x_lightRomeConfig());
			
			#endif
		#endif
		
	}

	return 0;	
}

#endif

static int rtl865x_init_hw(void)
{
#ifndef CONFIG_RTL865X_HW_TABLES
	rtl_netif_param_t np;
	rtl_vlan_param_t vp;
	struct  init_vlan_setting *setting;    
	unsigned int ret;    
	int ethno;
#endif
	
	if (swCore_init(totalVlans)) {
		printk("865x-nic: swCore_init() failed!\n");   
		return -1;
	}

	/* Initialize NIC module */
	if (swNic_init(rxRingSize, NUM_RX_PKTHDR_DESC, txRingSize, MBUF_LEN))  {
		printk("865x-nic: swNic_init failed!\n");            
		return -1;
	}

#ifndef CONFIG_RTL865X_HW_TABLES /* Forrest */
	setting = vlanSetting;
	for (ethno = 0; ethno < totalVlans; ethno++) {        
		/* Create NetIF */
		bzero((void *) &np, sizeof(rtl_netif_param_t));
		np.vid = setting[ethno].vid;
		np.valid = 1;
		#ifdef CONFIG_HW_MULTICAST_TBL
		np.enableRoute = 1;
		#else
		np.enableRoute = 0;
		#endif
		np.inAclEnd = 0;
		np.inAclStart = 0;
		np.outAclEnd = 0;
		np.outAclStart = 0;
		memcpy(&np.gMac, &setting[ethno].mac[0], 6);
                        
		np.macAddrNumber = 1;
		np.mtu = 1500;
		ret = swCore_netifCreate(ethno, &np);
		if ( ret != 0 ) {
			printk("865x-nic: swCore_netifCreate() failed:%d\n", ret );
			return -1;
		}
            
		/* Create VLAN */
		bzero((void *) &vp, sizeof(rtl_vlan_param_t));
		#ifdef CONFIG_RTL8196_RTL8366
		vp.egressUntag &= ~(0x41);
		vp.memberPort = 0x41;
		#else
		vp.egressUntag = setting[ethno].portmask;
		vp.memberPort = setting[ethno].portmask;
		#endif

		#ifdef CONFIG_RTL8196_RTL8366
		vp.fid = 0;
		#else
		vp.fid = ethno;
		#endif
		ret = swCore_vlanCreate(setting[ethno].vid, &vp);  //P1-P4

		if ( ret != 0 ) {           
			printk("865x-nic: swCore_vlanCreate() failed:%d\n", ret );
			return -1;  
		}
	}				
#endif /* Forrest */

#if defined(CONFIG_RTL865X_HW_TABLES) || defined(CONFIG_HW_MULTICAST_TBL)
	if (eth_flag & BIT(3))
		REG32(FFCR) = REG32(FFCR)  | IPMltCstCtrl_Enable;
	else
		REG32(FFCR) = REG32(FFCR)  & ~IPMltCstCtrl_Enable;		
#endif

#ifdef CONFIG_RTK_VLAN_SUPPORT
	if (L2_table_disabled) 
		REG32(SWTCR0) = REG32(SWTCR0)  | EnUkVIDtoCPU;
	else
		REG32(SWTCR0) = REG32(SWTCR0)  & (~EnUkVIDtoCPU);
#endif

	return 0;	
}


//---------------------------------------------------------------------------
#ifdef CONFIG_RTL865X_HW_TABLES		
static void reset_hw_mib_counter(struct net_device *dev)
{
	int vid, port;
	
	for (vid=0; vid<2; vid++) {
		if (!memcmp(vlanSetting[vid].mac, dev->dev_addr, 6))
			break;
	}
	if (vid == 2) {
		printk("%s: get vid error!\n", __FUNCTION__);
		return;
	}
	for (port=0; port<MAX_PORT_NUMBER; port++) {
		if 	(vlanSetting[vid].portmask & (1<<port)) 
		   WRITE_MEM32(MIB_CONTROL, (1<<port*2) | (1<<(port*2+1)));		
	}
}
#endif

//---------------------------------------------------------------------------
static int rtl865x_open(struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	uint32 flags;
	int rc;

	if (cp->opened)
		return 0;

	save_and_cli(flags);

	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	if (dev->name[3] < '2') 
	#endif
	
//    if (IS_FIRST_DEV(cp)) { 
	if (!NEXT_CP(cp)->opened) {     // this is the first open dev

#ifdef RTK_QUE
		rtk_queue_init(&rx_skb_queue);  
#else
		skb_queue_head_init(&rx_skb_queue);  
#endif

		//spin_lock_irqsave (&cp->lock, flags);

		rc = rtl865x_init_hw();

		//spin_unlock_irqrestore(&cp->lock, flags);		

		if (rc) {
			printk("rtl865x_init_hw() failed!\n");
			restore_flags(flags);
			return 1;
		}

#ifdef RX_TASKLET
		tasklet_init(&cp->rx_dsr_tasklet, interrupt_dsr_rx, (unsigned long)cp);
#endif

#ifdef TX_TASKLET
		tasklet_init(&cp->tx_dsr_tasklet, interrupt_dsr_tx, (unsigned long)cp);
#endif

		rc = request_irq(dev->irq, interrupt_isr, SA_INTERRUPT, dev->name, dev);
		if (rc) {
			printk("request_irq() error!\n");
			goto err_out_hw;
		}
		cp->irq_owner =1;

		swCore_start();			
	}
    
	cp->opened = 1;

#ifdef CONFIG_RTL865X_HW_TABLES	
	reset_hw_mib_counter(dev);
#endif

	netif_start_queue(dev);
#ifdef CONFIG_RTK_VOIP_PORT_LINK
	if ( !link_state_timer_work)
	{
		voip_rtnl = netlink_kernel_create(NETLINK_ROUTE, NULL);
		init_timer(&link_state_timer);
		link_state_timer.expires = jiffies + 100;
		link_state_timer.data = dev;
		link_state_timer.function = link_state_timer_action;
		mod_timer(&link_state_timer, jiffies + 100);
		link_state_timer_work = 1;
	}
#endif

#if defined(DYNAMIC_ADJUST_TASKLET) || defined(BR_SHORTCUT)
#if !defined(CONFIG_RTL8186_TR) && !defined(CONFIG_RTL8196B_TR)
	if (dev->name[3] == '0') 
#endif		
#ifndef CONFIG_RTL865X_AC
	if (dev->name[3] == '0') 
#endif
#ifndef CONFIG_RTL865X_KLD
	if (dev->name[3] == '0') 
#endif
	{	
		init_timer(&cp->expire_timer);
		cp->expire_timer.expires = jiffies + 100;
		cp->expire_timer.data = (unsigned long)dev;
		cp->expire_timer.function = one_sec_timer;
		mod_timer(&cp->expire_timer, jiffies + 100);		

#ifdef DYNAMIC_ADJUST_TASKLET			
		rx_cnt = 0;
#endif
#ifdef BR_SHORTCUT
		pkt_cnt = 0;
#endif
	}
#endif

	restore_flags(flags);
	return 0;

err_out_hw:
	rtl865x_stop_hw(dev, cp);
	restore_flags(flags);
	return rc;
}

//---------------------------------------------------------------------------
static int rtl865x_close(struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	uint32 flags;

	if (!cp->opened)
		return 0;

	save_and_cli(flags);

#ifdef CONFIG_RTK_VOIP_PORT_LINK
	if (link_state_timer_work){
		if (timer_pending(&link_state_timer)){
       		    del_timer_sync(&link_state_timer);
       		    link_state_timer_work = 0;
       		}
       	}
#endif
	
	netif_stop_queue(dev);

	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	if (dev->name[3] < '2') 
	#endif

	if (!NEXT_CP(cp)->opened) {		
		rtl865x_stop_hw(dev, cp);       
						
		free_irq(dev->irq, GET_IRQ_OWNER(cp));				
		((struct re_private *)GET_IRQ_OWNER(cp)->priv)->irq_owner = 0;        

#ifdef RX_TASKLET
		tasklet_kill(&cp->rx_dsr_tasklet);	
#endif

#ifdef TX_TASKLET
		tasklet_kill(&cp->tx_dsr_tasklet);
#endif

		free_rx_skb();
	}

	memset(&cp->net_stats, '\0', sizeof(struct net_device_stats));
	
	cp->opened = 0;
		
#if defined(DYNAMIC_ADJUST_TASKLET) || defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(BR_SHORTCUT) || defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
	if (timer_pending(&cp->expire_timer))
		del_timer_sync(&cp->expire_timer);
#endif

#ifdef BR_SHORTCUT
	if (dev == cached_dev)
		cached_dev=NULL;
#endif

#ifdef CONFIG_RTL865X_HW_TABLES	
	reset_hw_mib_counter(dev);
#endif

	restore_flags(flags);				

	return 0;
}

//---------------------------------------------------------------------------
#ifdef CONFIG_RTL_KERNEL_MIPS16_DRVETH
__NOMIPS16
#endif 
static int write_proc(struct file *file, const char *buffer,
              unsigned long count, void *data)
{
    unsigned char tmpbuf[100];

    if (count < 2)
        return -EFAULT;

    if (buffer && !copy_from_user(tmpbuf, buffer, count)) {
        sscanf(tmpbuf, "%d", &eth_flag);

		if ((eth_flag & TASKLET_MASK) == 1)
			rx_tasklet_enabled = 0;
		if ((eth_flag & TASKLET_MASK) == 2)
			rx_tasklet_enabled = 1;	

#if defined(CONFIG_RTL865X_HW_TABLES) || defined(CONFIG_HW_MULTICAST_TBL)
		if (eth_flag & BIT(3))
			REG32(FFCR) = REG32(FFCR)  | IPMltCstCtrl_Enable;
		else
			REG32(FFCR) = REG32(FFCR)  & ~IPMltCstCtrl_Enable;		
#endif
				
        return count;
    }
    return -EFAULT;
}

//---------------------------------------------------------------------------
#ifdef DYNAMIC_ADJUST_TASKLET
static int write_proc_rxthres(struct file *file, const char *buffer,
              unsigned long count, void *data)
{
    unsigned char tmpbuf[100];

    if (count < 2)
        return -EFAULT;
    if (buffer && !copy_from_user(tmpbuf, buffer, count)) {
        sscanf(tmpbuf, "%d", &rx_pkt_thres);
        if (rx_pkt_thres) {
				rx_cnt = 0;
				eth_flag = 0 | (eth_flag & ~TASKLET_MASK);
        }       
        return count;
    }
    return -EFAULT;
}
#endif

//---------------------------------------------------------------------------
#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
static int read_proc_eth_stats(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{
	int len;

	#ifdef CONFIG_RTK_CHECK_ETH_TX_HANG
	len = sprintf(page, "  eth_skb_free_num: %d, reused_cnt: %d, tx_hang_cnt: %d\n", eth_skb_free_num+rx_skb_queue.qlen, reused_skb_num, tx_hang_cnt);  
	#else
	len = sprintf(page, "  eth_skb_free_num: %d, reused_cnt: %d\n", eth_skb_free_num+rx_skb_queue.qlen, reused_skb_num);  
	#endif
	
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}
#endif

//---------------------------------------------------------------------------
#ifdef CONFIG_PROC_FS
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR)
static int read_proc_stats(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{

    struct net_device *dev = (struct net_device *)data;
    struct re_private *cp = dev->priv;
    int len;

    len = sprintf(page, "%d %d %d %d\n", cp->tx_avarage, cp->tx_peak, cp->rx_avarage, cp->rx_peak);  
    if (len <= off+count) 
        *eof = 1;      
    *start = page + off;      
    len -= off;      
    if (len > count) 
        len = count;      
    if (len < 0) 
        len = 0;      
    return len; 

}

//---------------------------------------------------------------------------
static int write_proc_stats(struct file *file, const char *buffer,
              unsigned long count, void *data)
{
    struct net_device *dev = (struct net_device *)data;
    struct re_private *cp = dev->priv;

    cp->tx_avarage = cp->tx_peak = cp->tx_byte_cnt = 0;
    cp->rx_avarage = cp->rx_peak = cp->rx_byte_cnt = 0;

    return count;
}
#endif // CONFIG_RTL8186_TR

#if defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
static int read_proc_stats(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{

    struct net_device *dev = (struct net_device *)data;
    struct re_private *cp = dev->priv;
    int len;

    len = sprintf(page, "%d %d %d %d %ld\n", cp->tx_avarage, cp->tx_peak, cp->rx_avarage, cp->rx_peak, cp->net_stats.rx_packets);  
    if (len <= off+count) 
        *eof = 1;      
    *start = page + off;      
    len -= off;      
    if (len > count) 
        len = count;      
    if (len < 0) 
        len = 0;      
    return len; 

}

//---------------------------------------------------------------------------
static int write_proc_stats(struct file *file, const char *buffer,
              unsigned long count, void *data)
{
    struct net_device *dev = (struct net_device *)data;
    struct re_private *cp = dev->priv;

    cp->tx_avarage = cp->tx_peak = cp->tx_byte_cnt = 0;
    cp->rx_avarage = cp->rx_peak = cp->rx_byte_cnt = 0;
#if defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
	memset(&cp->net_stats, 0, sizeof(struct net_device_stats));
#endif

    return count;
}
#endif // CONFIG_RTL865X_AC || CONFIG_RTL865X_KLD


#ifdef CONFIG_RTK_VLAN_SUPPORT
static int read_proc_vlan(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{

    struct net_device *dev = (struct net_device *)data;
    struct re_private *cp = dev->priv;
    int len;

    len = sprintf(page, "gvlan=%d, lan=%d, vlan=%d, tag=%d, vid=%d, priority=%d, cfi=%d\n", 
		cp->vlan_setting.global_vlan, cp->vlan_setting.is_lan, cp->vlan_setting.vlan, cp->vlan_setting.tag, 
		cp->vlan_setting.id, cp->vlan_setting.pri, cp->vlan_setting.cfi);
			
    if (len <= off+count) 
        *eof = 1;      
    *start = page + off;      
    len -= off;      
    if (len > count) 
        len = count;      
    if (len < 0) 
        len = 0;      
    return len; 
}


static int write_proc_vlan(struct file *file, const char *buffer,
              unsigned long count, void *data)
{
    struct net_device *dev = (struct net_device *)data;
    struct re_private *cp = dev->priv;
	char tmp[128];

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 128)) {
		int num = sscanf(tmp, "%d %d %d %d %d %d %d", 
			&cp->vlan_setting.global_vlan, &cp->vlan_setting.is_lan, 
			&cp->vlan_setting.vlan, &cp->vlan_setting.tag, 
			&cp->vlan_setting.id, &cp->vlan_setting.pri, 
			&cp->vlan_setting.cfi);

		if (num !=  7) {
			panic_printk("invalid vlan parameter!\n");
		}
	}
	return count;	
}
#endif // CONFIG_RTK_VLAN_SUPPORT


static struct proc_dir_entry *proc_wan_port=NULL;

static int read_proc_wan_port(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", wan_port);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}

/* 
 * Usage: echo "$WAN_PORT$OP_MODE" > /proc/wan_port, or echo "$WAN_PORT" > /proc/wan_port
 */
#ifdef CONFIG_RTL_KERNEL_MIPS16_DRVETH
__NOMIPS16
#endif 
static int write_proc_wan_port(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	unsigned char tmp[4];
	#ifdef CONFIG_RTL865X_HW_TABLES
    	struct re_private *cp;
	#endif
	int op_mode = 0; //gateway

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 2)) {

		if ((tmp[0] >= '0') && (tmp[0] <= '5')) {
			wan_port = tmp[0] - '0';

			if ((tmp[1] == '1') && (tmp[1] == '2'))
				op_mode = 1; //bridge/WISP

			#ifdef CONFIG_RTL865X_HW_TABLES

			if (op_mode == 0) {
				/* set LAN/WAN port mask */
				vlanSetting[0].portmask = ALL_PORTS & (~BIT(wan_port));
				vlanSetting[1].portmask = BIT(wan_port);
			}
			else {
				vlanSetting[0].portmask = ALL_PORTS;
				vlanSetting[1].portmask = 0;

			}
			
			// eth0(LAN) VLAN
			cp = reNet[0]->priv;
			cp->port = vlanSetting[0].portmask;
		
			// eth1(WAN) VLAN
			cp = reNet[1]->priv;
			cp->port = vlanSetting[1].portmask;

			re_init_vlanTable(op_mode);

			#else
			if (op_mode == 0) 
				op_mode = 2;
			SoftNAT_OP_Mode(op_mode);
			#endif
		}
				
		return count;
	}
	return -EFAULT;
}

#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
static struct proc_dir_entry *proc_disable_L2=NULL;

static int read_proc_disable_L2(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", L2_table_disabled);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}

static int write_proc_disable_L2(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	unsigned char tmp[4];

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 2)) {

		L2_table_disabled = tmp[0] - '0';
		if (L2_table_disabled)
			L2_table_disabled = 1;

		SoftNAT_OP_Mode(2);
				
		return count;
	}
	return -EFAULT;
}
#endif

#ifdef CONFIG_RTL8196B_KLD
static struct proc_dir_entry *proc_port_speed=NULL;

static int write_proc_port_speed(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	unsigned char tmp[4];
	uint32 statCtrlReg, phyId; // = wan_port;

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 3)) {
		/* tmp[0]: '0' -- auto,	   '8' -- 10M
				   '9' -- 100M,	   'a' -- Giga
		    tmp[2]: port number
		 */
		phyId = tmp[2] - '0'; 
		rtl8651_getAsicEthernetPHYReg( phyId, 4, &statCtrlReg );
		statCtrlReg &= ~(CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD);
		if (tmp[0] == '8')
			statCtrlReg |= (CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD);
		else if (tmp[0] == '9')
			statCtrlReg |= (CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD);
		else
			statCtrlReg |= (CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD);
			
		rtl8651_setAsicEthernetPHYReg( phyId, 4, statCtrlReg );
		rtl8651_getAsicEthernetPHYReg( phyId, 0, &statCtrlReg );

		statCtrlReg |= RESTART_AUTONEGO;
		rtl8651_setAsicEthernetPHYReg( phyId, 0, statCtrlReg );				

		return count;
	}
	return -EFAULT;
}
#endif

#endif

#if defined(CONFIG_HW_MULTICAST_TBL) || defined(CONFIG_RTK_VLAN_SUPPORT)
/* Chip Version */
#define RTL865X_CHIP_VER_RTL865XB	0x01
#define RTL865X_CHIP_VER_RTL865XC	0x02
#define RTL865X_CHIP_VER_RTL8196B	0x03

#ifdef CONFIG_RTL865X_LIGHT_ROMEDRV
extern int32 RtkHomeGatewayChipRevisionID;
extern int32 RtkHomeGatewayChipNameID;
#else
int32 RtkHomeGatewayChipRevisionID;
int32 RtkHomeGatewayChipNameID;
#endif
#endif

#ifdef CONFIG_HW_PROTOCOL_VLAN_TBL
int oldStatus = 0;
static int isCreated;
static struct proc_dir_entry *res_custom_passthru=NULL;
static char passThru_flag[2] = { 0x30, 0};
static const unsigned char passthru_vlan_mac[6] = { 0x00, 0x12, 0x34, 0x56, 0x78, 0x90 };

static int custom_Passthru_read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;	
	len = sprintf(page, "%c\n", passThru_flag[0]);
	if (len <= off+count) 
		*eof = 1;

	*start = page + off;
	len -= off;

	if (len>count) 
		len = count;

	if (len<0) len = 0;

	return len;
}

static void passthru_vlan_create(void)
{
	rtl_netif_param_t np;
	rtl_vlan_param_t vp;

	/* Create NetIF */
	bzero((void *) &np, sizeof(rtl_netif_param_t));
	np.vid = PASSTHRU_VLAN_ID;
	np.valid = 1;
	np.enableRoute = 1;
	np.inAclEnd = 0;
	np.inAclStart = 0;
	np.outAclEnd = 0;
	np.outAclStart = 0;
	memcpy(&np.gMac, passthru_vlan_mac, 6);
                        
	np.macAddrNumber = 1;
	np.mtu = 1500;
	swCore_netifCreate(RTL865XC_NETINTERFACE_NUMBER - 1, &np);
            
	/* Create VLAN */
	bzero((void *) &vp, sizeof(rtl_vlan_param_t));
	vp.egressUntag = ALL_PORTS;
	vp.memberPort = ALL_PORTS;
	vp.fid = 0;
	swCore_vlanCreate(PASSTHRU_VLAN_ID, &vp);

	return;
}

void updateProtocolBasedVLAN(void)
{
	int i, _add;

	_add = (oldStatus & IP6_PASSTHRU_MASK) ? TRUE : FALSE;
	
	for(i=0; i<RTL865XC_PORT_NUMBER; i++)
		rtl8651_setProtocolBasedVLAN(IP6_PASSTHRU_RULEID, i, _add, PASSTHRU_VLAN_ID);
	
	passthru_vlan_create();
}

static int custom_Passthru_write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	int flag,i;

	if (buffer && !copy_from_user(passThru_flag, buffer, 1))
	{			
		flag = passThru_flag[0] - '0';
		if(flag ^ oldStatus)
		{			
			//IPv6 PassThru
			if((flag & IP6_PASSTHRU_MASK) ^ (oldStatus & IP6_PASSTHRU_MASK)) 
			{			
				if(flag & IP6_PASSTHRU_MASK)
				{//add				
					for(i=0; i<RTL865XC_PORT_NUMBER; i++)
					{
						rtl8651_setProtocolBasedVLAN(IP6_PASSTHRU_RULEID, i, TRUE, PASSTHRU_VLAN_ID);
					}				
				}
				else
				{//delete
					for(i=0; i<RTL865XC_PORT_NUMBER; i++)
					{
						rtl8651_setProtocolBasedVLAN(IP6_PASSTHRU_RULEID, i, FALSE, PASSTHRU_VLAN_ID);
					}
				}
			}
			#if 0
			//PPPoE PassThru
			if((flag & PPPOE_PASSTHRU_MASK) ^ (oldStatus & PPPOE_PASSTHRU_MASK)) 
			{			
				if(flag & PPPOE_PASSTHRU_MASK)
				{//add				
					for(i=0; i<RTL865XC_PORT_NUMBER; i++)
					{
						rtl8651_setProtocolBasedVLAN(RTL8651_PBV_RULE_PPPOE_CONTROL, i, TRUE, PASSTHRU_VLAN_ID);
						rtl8651_setProtocolBasedVLAN(RTL8651_PBV_RULE_PPPOE_SESSION, i, TRUE, PASSTHRU_VLAN_ID);
					}				
				}
				else
				{//delete
					for(i=0; i<RTL865XC_PORT_NUMBER; i++)
					{
						rtl8651_setProtocolBasedVLAN(RTL8651_PBV_RULE_PPPOE_CONTROL, i, FALSE, PASSTHRU_VLAN_ID);
						rtl8651_setProtocolBasedVLAN(RTL8651_PBV_RULE_PPPOE_SESSION, i, FALSE, PASSTHRU_VLAN_ID);
					}
				}
			}
			#endif
			//vlan
			if(!isCreated)
			{
				passthru_vlan_create();
				isCreated=1;
			}
		}
		oldStatus=flag;
		return count;
	}
	return -EFAULT;
}

int32 rtl8651_customPassthru_init(void)
{
	oldStatus=0;
	isCreated=0;
	res_custom_passthru = create_proc_entry("custom_Passthru", 0, NULL);	
	if(res_custom_passthru)
	{
		res_custom_passthru->read_proc = custom_Passthru_read_proc;
		res_custom_passthru->write_proc = custom_Passthru_write_proc;
	}
	//rtl8651_defineProtocolBasedVLAN( IP6_PASSTHRU_RULEID, 0x0, 0x86DD );
	return 0;
}
#endif

//---------------------------------------------------------------------------
static int __init rtl865x_probe(int ethno)
{
#ifdef DYNAMIC_ADJUST_TASKLET
	struct proc_dir_entry *res1;
#endif	

#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTK_VLAN_SUPPORT)
	struct proc_dir_entry *res_stats_root;
	struct proc_dir_entry *res_stats;
#endif

#if defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
	struct proc_dir_entry *res_stats_root;
	struct proc_dir_entry *res_stats;
#endif

#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
	struct proc_dir_entry *res2;
#endif

#ifdef MODULE
	printk("%s", version);
#endif

	struct net_device *dev;
	struct re_private *cp;
	int rc;
	void *regs;
	unsigned i;
	struct proc_dir_entry *res;

#ifndef MODULE
	static int version_printed;
	if (version_printed++ == 0)
		printk("%s", version);
#endif

#ifdef CONFIG_RTL8214_SUPPORT
	if (ethno == 0) { 

		//for EMI add
		REG32(0xbb804150) = 0x01037d15;
		REG32(0xbb804118) = 0x1600037f;

		//For RTL8211C learning
		rtl8651_getAsicEthernetPHYReg( GIGA_P5_PHYID, 0, &rc );
		
		//for 8652 auto detect wan port
		wan_port = ProbeWanPort(1);
		vlanSetting[0].portmask = ALL_PORTS & (~BIT(wan_port)); // eth0 lan		
		vlanSetting[1].portmask=BIT(wan_port); //eth1 wan
		
#ifdef CONFIG_RTL865X_HW_TABLES /* Forrest */
		lrConfig[0].memPort = ALL_PORTS & (~BIT(wan_port));	// eth0(LAN) VLAN
		lrConfig[0].untagSet = lrConfig[0].memPort;	// eth0(LAN) VLAN
		lrConfig[1].memPort = BIT(wan_port);	// eth1(WAN) VLAN
		lrConfig[1].untagSet = lrConfig[1].memPort;	// eth1(WAN) VLAN	
		#ifndef CONFIG_RTL865X_HW_PPTPL2TP
		lrConfig[2].memPort = BIT(wan_port);
		lrConfig[2].untagSet = lrConfig[2].memPort;
		#endif
#endif /* Forrest */		
	}    
#endif

	if (ethno == 0) { 
		chip_id = (READ_MEM32(CVIDR)) >> 16; 
		if (chip_id == 0x8196) 
			chip_revision_id = (READ_MEM32(CVIDR)) & 0x0f;
		else
			chip_revision_id = ((READ_MEM32(CRMR)) >> 12) & 0x0f;

		#ifdef CONFIG_HW_MULTICAST_TBL
		if (chip_id == 0x8196) 
			RtkHomeGatewayChipNameID = RTL865X_CHIP_VER_RTL8196B;
		else
			RtkHomeGatewayChipNameID = RTL865X_CHIP_VER_RTL865XC;
		
		RtkHomeGatewayChipRevisionID = chip_revision_id;
		#endif
	}

#ifdef CONFIG_RTL865X_HW_TABLES
	if (ethno == 0) { 
		// be initialized once only
		FullAndSemiReset();
		rtl8651_extDeviceinit();
		INIT_CHECK(rtl865x_lightRomeInit()); 
		INIT_CHECK(rtl865x_lightRomeConfig());
		rtl8651_extDeviceInitTimer(); 
	}
#endif    

	dev = alloc_etherdev(sizeof(struct re_private));
	if (!dev)
		return -ENOMEM;
		
	SET_MODULE_OWNER(dev);
	cp = dev->priv;
	cp->dev = dev;
	reNet[ethno] = dev;

	if (ethno == 1) {
		cp->dev_prev = reNet[0];
		((struct re_private *)reNet[0]->priv)->dev_next = dev;
	}
			
	spin_lock_init(&cp->lock);

	 /* Set Default MAC address */
	for (i = 0; i < 6; i++)
		((u8 *)(dev->dev_addr))[i] =vlanSetting[ethno].mac[i];

	cp->vid = vlanSetting[ethno].vid;
	cp->port = vlanSetting[ethno].portmask;           

	regs = (void *)((ethno) ? SWMACCR_BASE : SWMACCR_BASE);
	dev->base_addr = (unsigned long) regs;
	dev->irq = ICU_NIC;

	dev->open = rtl865x_open;
	dev->stop = rtl865x_close;
	dev->do_ioctl = re865x_ioctl;
	dev->hard_start_xmit = rtl865x_start_xmit;
	dev->get_stats = rtl865x_get_stats;
	dev->set_mac_address = rtl865x_set_hwaddr;

#ifdef CP_TX_CHECKSUM
	dev->features |= NETIF_F_SG | NETIF_F_IP_CSUM;
#endif

	rc = register_netdev(dev);
	if (rc)
		goto err_out_iomap;

	printk(KERN_INFO "%s: %s at 0x%lx, "
		"%02x:%02x:%02x:%02x:%02x:%02x, "
		"IRQ %d\n",
		dev->name,
		"RTL865x-NIC",
		dev->base_addr,
		dev->dev_addr[0], dev->dev_addr[1],
		dev->dev_addr[2], dev->dev_addr[3],
		dev->dev_addr[4], dev->dev_addr[5],
		dev->irq);

#ifdef CONFIG_PROC_FS
	if (ethno == 0) {
		res = create_proc_entry("eth_flag", 0, NULL);
    		if (res)
			res->write_proc = write_proc;
    
		#ifdef DYNAMIC_ADJUST_TASKLET
		res1 = create_proc_entry("rx_pkt_thres", 0, NULL);
		if (res1)
			res1->write_proc = write_proc_rxthres;
		#endif      

		#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
		if ((res2 = create_proc_read_entry("eth_stats", 0644, NULL,
			read_proc_eth_stats, (void *)dev)) == NULL) {
			printk("create_proc_read_entry failed!\n");
			goto err_out_iomap;
		}
		#endif
	}
	
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTK_VLAN_SUPPORT)
	res_stats_root = proc_mkdir(dev->name, NULL);
	if (res_stats_root == NULL) {
		printk("proc_mkdir failed!\n");
		goto err_out_iomap;
	}
	
#ifdef CONFIG_RTK_VLAN_SUPPORT
	if ((res_stats = create_proc_read_entry("mib_vlan", 0644, res_stats_root,
			read_proc_vlan, (void *)dev)) == NULL) {
		printk("create_proc_read_entry failed!\n");
		goto err_out_iomap;
	}
	res_stats->write_proc = write_proc_vlan;	
#endif	

#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR)
	if ((res_stats = create_proc_read_entry("stats", 0644, res_stats_root,
		read_proc_stats, (void *)dev)) == NULL) {
		printk("create_proc_read_entry failed!\n");
		goto err_out_iomap;
	}
	res_stats->write_proc = write_proc_stats;
#endif
#endif

#if defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD)
	#if defined(CONFIG_RTL8196B_KLD)
		if (ethno == 1) { 
	#endif
	res_stats_root = proc_mkdir(dev->name, NULL);
	if (res_stats_root == NULL) {
		printk("proc_mkdir failed!\n");
		goto err_out_iomap;
	}
	if ((res_stats = create_proc_read_entry("stats", 0644, res_stats_root,
		read_proc_stats, (void *)dev)) == NULL) {
		printk("create_proc_read_entry failed!\n");
		goto err_out_iomap;
	}
	res_stats->write_proc = write_proc_stats;
	
	#if defined(CONFIG_RTL8196B_KLD)
	} 
	#endif
	
#endif
#endif // CONFIG_PROC_FS

#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
	if (ethno == 0) { 
		init_priv_eth_skb_buf();
	}
#endif		

#ifdef CONFIG_HW_PROTOCOL_VLAN_TBL
	if (ethno == 0) { 
		rtl8651_customPassthru_init();
	}
#endif

	return 0;

err_out_iomap:
	printk("in err_out_iomap\n");
	iounmap(regs);
	kfree(dev);
	return -1 ;
}

//---------------------------------------------------------------------------
#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
#define ETH_SKB_BUF_SIZE	(CROSS_LAN_MBUF_LEN+sizeof(struct skb_shared_info)+128)
#define ETH_MAGIC_CODE		"865x"

struct priv_skb_buf2 {
	unsigned char magic[4];
	struct list_head	list;	
	unsigned char buf[ETH_SKB_BUF_SIZE];
};
#ifdef CONFIG_RTL8196B
struct priv_skb_buf2 eth_skb_buf[MAX_ETH_SKB_NUM];
#else
static struct priv_skb_buf2 eth_skb_buf[MAX_ETH_SKB_NUM];
#endif
static struct list_head eth_skbbuf_list;

extern struct sk_buff *dev_alloc_8190_skb(unsigned char *data, int size);

//---------------------------------------------------------------------------
static void init_priv_eth_skb_buf(void)
{
	int i;

	memset(eth_skb_buf, '\0', sizeof(struct priv_skb_buf2)*MAX_ETH_SKB_NUM);
	INIT_LIST_HEAD(&eth_skbbuf_list);
	
	for (i=0; i<MAX_ETH_SKB_NUM; i++)  {
		memcpy(eth_skb_buf[i].magic, ETH_MAGIC_CODE, 4);	
		INIT_LIST_HEAD(&eth_skb_buf[i].list);
		list_add_tail(&eth_skb_buf[i].list, &eth_skbbuf_list);	
        
	}
}

//---------------------------------------------------------------------------
static __inline__ unsigned char *get_buf_from_poll(struct list_head *phead, unsigned int *count)
{
	unsigned long flags;
	unsigned char *buf;
	struct list_head *plist;

	save_and_cli(flags);

	if (list_empty(phead)) {
		restore_flags(flags);
//		DEBUG_ERR("eth_drv: phead=%X buf is empty now!\n", (unsigned int)phead);
		return NULL;
	}

	if (*count == 0) {
		restore_flags(flags);
//		DEBUG_ERR("eth_drv: phead=%X under-run!\n", (unsigned int)phead);
		return NULL;
	}

	*count = *count - 1;
	plist = phead->next;
	list_del_init(plist);
	buf = (unsigned char *)((unsigned int)plist + sizeof (struct list_head));
	restore_flags(flags);
	return buf;
}

//---------------------------------------------------------------------------
static __inline__ void release_buf_to_poll(unsigned char *pbuf, struct list_head	*phead, unsigned int *count)
{
	unsigned long flags;
	struct list_head *plist;

	save_and_cli(flags);

	*count = *count + 1;
	plist = (struct list_head *)((unsigned int)pbuf - sizeof(struct list_head));
	list_add_tail(plist, phead);
	restore_flags(flags);
}

//---------------------------------------------------------------------------
void free_rtl865x_eth_priv_buf(unsigned char *head)
{
	#ifdef DELAY_REFILL_ETH_RX_BUF
	extern int return_to_rx_pkthdr_ring(unsigned char *head);
	if (!return_to_rx_pkthdr_ring(head)) 
	#endif
		release_buf_to_poll(head, &eth_skbbuf_list, (unsigned int *)&eth_skb_free_num);	
}

//---------------------------------------------------------------------------
static struct sk_buff *dev_alloc_skb_priv_eth(unsigned int size)
{
	struct sk_buff *skb;

	/* first argument is not used */
	unsigned char *data = get_buf_from_poll(&eth_skbbuf_list, (unsigned int *)&eth_skb_free_num);
	if (data == NULL) {
//		DEBUG_ERR("eth_drv: priv skb buffer empty!\n");
		return NULL;
	}
	skb = dev_alloc_8190_skb(data, size);
	if (skb == NULL) {
		free_rtl865x_eth_priv_buf(data);
		return NULL;
	}
    
	return skb;
}

//---------------------------------------------------------------------------
int is_rtl865x_eth_priv_buf(unsigned char *head)
{
	unsigned long offset = (unsigned long)(&((struct priv_skb_buf2 *)0)->buf);
	struct priv_skb_buf2 *priv_buf = (struct priv_skb_buf2 *)(((unsigned long)head) - offset);

	if (!memcmp(priv_buf->magic, ETH_MAGIC_CODE, 4)) {
		return 1;	
	}
	else {
		return 0;
	}
}

//---------------------------------------------------------------------------
#if defined(CONFIG_NET_WIRELESS_AGN) || defined(CONFIG_NET_WIRELESS_AG)
#include <net/dst.h>
static void copy_skb_header(struct sk_buff *new, const struct sk_buff *old)
{
	/*
	 *	Shift between the two data areas in bytes
	 */
	unsigned long offset = new->data - old->data;

	new->list=NULL;
	new->sk=NULL;
	new->dev=old->dev;
	new->priority=old->priority;
	new->protocol=old->protocol;
	new->dst=dst_clone(old->dst);
	new->h.raw=old->h.raw+offset;
	new->nh.raw=old->nh.raw+offset;
	new->mac.raw=old->mac.raw+offset;
	memcpy(new->cb, old->cb, sizeof(old->cb));
	atomic_set(&new->users, 1);
	new->pkt_type=old->pkt_type;
	new->stamp=old->stamp;
	new->destructor = NULL;
	new->security=old->security;
#ifdef CONFIG_NETFILTER
	new->nfmark=old->nfmark;
	new->nfcache=old->nfcache;
	new->nfct=old->nfct;
	nf_conntrack_get(new->nfct);
#ifdef CONFIG_NETFILTER_DEBUG
	new->nf_debug=old->nf_debug;
#endif
#endif
#ifdef CONFIG_NET_SCHED
	new->tc_index = old->tc_index;
#endif
#ifdef CONFIG_RTK_VOIP_VLAN_ID
	new->rx_vlan=old->rx_vlan;
	new->rx_wlan=old->rx_wlan;
#endif
#ifdef CONFIG_RTK_VLAN_SUPPORT	
	new->tag.v = old->tag.v;
#endif
}

//---------------------------------------------------------------------------
struct sk_buff *priv_skb_copy(struct sk_buff *skb)
{
	struct sk_buff *n;	

	if (rx_skb_queue.qlen == 0) {
#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
		n = dev_alloc_skb_priv_eth(CROSS_LAN_MBUF_LEN);
#else        
		n  = dev_alloc_skb(CROSS_LAN_MBUF_LEN);
#endif
	}
	else {
#ifdef RTK_QUE
		n = rtk_dequeue(&rx_skb_queue);
#else
		n = __skb_dequeue(&rx_skb_queue);
#endif
	}
	
	if (n == NULL) 
		return NULL;

	/* Set the tail pointer and length */	
	skb_put(n, skb->len);	
	n->csum = skb->csum;	
	n->ip_summed = skb->ip_summed;	
	memcpy(n->data, skb->data, skb->len);

	copy_skb_header(n, skb);
	return n;
}
#endif // defined(CONFIG_NET_WIRELESS_AGN) || defined(CONFIG_NET_WIRELESS_AG)
#endif // CONFIG_RTL865X_ETH_PRIV_SKB

//---------------------------------------------------------------------------
static void __exit rtl865x_exit (void)
{
}

//---------------------------------------------------------------------------
static int __init rtl865x_init(void)
{ 
#ifdef CONFIG_PROC_FS
	proc_wan_port = create_proc_entry("wan_port", 0, NULL);
	if (proc_wan_port) {
		proc_wan_port->read_proc = read_proc_wan_port;
		proc_wan_port->write_proc = write_proc_wan_port;
	}  

	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	proc_disable_L2 = create_proc_entry("disable_l2_table", 0, NULL);
	if (proc_disable_L2) {
		proc_disable_L2->read_proc = read_proc_disable_L2;
		proc_disable_L2->write_proc = write_proc_disable_L2;
	}  
	#endif

	#ifdef CONFIG_RTL8196B_KLD
	proc_port_speed = create_proc_entry("port_speed", 0, NULL);
	if (proc_port_speed) {
		proc_port_speed->write_proc = write_proc_port_speed;
	}  
	#endif
#endif

/* I suppose that the CONFIG_RTL8214_SUPPORT is undefined under  TR/AC/KLD */
#if defined(CONFIG_RTL8186_TR) || defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD) || defined(CONFIG_RTK_VOIP_DRIVERS_WAN_PORT_4)
	wan_port = 4; 

	/* set LAN/WAN port mask */
	vlanSetting[0].portmask = ALL_PORTS & (~BIT(wan_port));
	vlanSetting[1].portmask=BIT(wan_port);
		
	#ifdef CONFIG_RTL865X_HW_TABLES
	lrConfig[0].memPort = ALL_PORTS & (~BIT(wan_port));
	lrConfig[0].untagSet = lrConfig[0].memPort;
	lrConfig[1].memPort = BIT(wan_port);
	lrConfig[1].untagSet = lrConfig[1].memPort;
	#ifndef CONFIG_RTL865X_HW_PPTPL2TP
	lrConfig[2].memPort = BIT(wan_port);
	lrConfig[2].untagSet = lrConfig[2].memPort;
	#endif
	#endif

#else
	wan_port = 0; 
#endif

#ifdef CONFIG_RTL8196_RTL8366
	
	{
		int ret;
		int i;
		rtl8366rb_phyAbility_t phy;
		REG32(PEFGHCNR_REG) = REG32(PEFGHCNR_REG)& (~(1<<11) ); //set byte F GPIO3 = gpio
	        REG32(PEFGHDIR_REG) = REG32(PEFGHDIR_REG) | (1<<11);  //0 input, 1 output, set F bit 3 output
		REG32(PEFGHDAT_REG) = REG32(PEFGHDAT_REG) |( (1<<11) ); //F3 GPIO
		mdelay(150);
		ret = smi_init(GPIO_PORT_F, 2, 1);
		
		ret = rtl8366rb_initChip();
		ret = rtl8366rb_initChip();
		ret = rtl8366rb_initVlan();
		ret = rtl8366rb_initAcl();
		ret = smi_write(0x0f09, 0x0020);
		ret = smi_write(0x0012, 0xe0ff);

#if 1
		memset(&phy, 0, sizeof(rtl8366rb_phyAbility_t));
		phy.Full_1000 = 1;
		phy.Full_100 = 1;
		phy.Full_10 = 1;
		phy.Half_100 = 1;
		phy.Half_10 = 1;
		phy.FC = 1;
		phy.AsyFC = 1;
		phy.AutoNegotiation = 1;
		for(i=0;i<5;i++)
		{
			ret = rtl8366rb_setEthernetPHY(i,&phy);
		}
		//ret = smi_write(0x0012, 0xe0ff);

		
#endif
	REG32(0xb8010000)=REG32(0xb8010000)&(0x20000000);
	REG32(0xbb80414c)=0x00037d16;
	REG32(0xbb804100)=1;
	REG32(0xbb804104)=0x00E80367;
	}
#endif


	rtl865x_probe(0);
	rtl865x_probe(1);

	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	rtl865x_probe(2); // eth2
	rtl865x_probe(3); // eth3
	rtl865x_probe(4); // eth4
	#endif

#ifdef PREVENT_BCAST_REBOUND
	memset( &bcast , 0,  sizeof(struct bcast_tr_s ));
#endif
	return 0;
}
#ifdef CONFIG_RTL8196_RTL8366
void force_reset_nic_8366()
{
	{
                int ret;
		delay_ms(200);
                ret = rtl8366rb_initChip();
               	ret = rtl8366rb_initChip();
		ret = rtl8366rb_initVlan();

         }
}
#endif
//---------------------------------------------------------------------------
#ifdef CONFIG_RTK_VOIP_WAN_VLAN
void add_WAN_VLAN(unsigned int nvid){
	int ret;
	rtl_vlan_param_t vp;
	bzero((void *) &vp, sizeof(rtl_vlan_param_t));
	
	vp.egressUntag = vlanSetting[1].portmask; //vlanSetting[1].portmask;
	vp.memberPort = vlanSetting[1].portmask;
#ifdef CONFIG_RTL865X_HW_TABLES
	vp.fid = 1;
#else
	vp.fid = 1;
#endif

	ret = swCore_vlanCreate(nvid, &vp);  //P1-P4
	if ( ret != 0 )
		printk("865x-nic: swCore_vlanCreate(%d) failed:%d\n", nvid, ret );
}

void del_WAN_VLAN(unsigned short nvid){
	if( nvid != vlanSetting[1].vid && nvid != vlanSetting[0].vid)
		swCore_vlanDestroy(nvid);
}
#endif

#ifdef CONFIG_RTL8196_RTL8366
int savOP_MODE_value=2;
#endif

#ifndef CONFIG_RTL865X_HW_TABLES
void SoftNAT_OP_Mode(int count)
{
	rtl_netif_param_t np;
    	rtl_vlan_param_t vp;
    	struct  init_vlan_setting *setting;    
    	unsigned int ret;    
    	int ethno;
    	struct re_private *cp;
    	
    	totalVlans = count;
	setting = vlanSetting;
	
#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	if (L2_table_disabled) {

		// eth0(LAN) VLAN, eth2 ~ 4 do not change
		cp = reNet[0]->priv;
#ifdef CONFIG_RTK_GUEST_ZONE		
		REG32(PVCR0) = (LAN_VID << 16) | WAN_VID;
		REG32(PVCR1) = (LAN3_VID << 16) | LAN2_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN4_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;
	
		cp->port = BIT(1);		
		setting[0].portmask = BIT(1);
#endif
	
#ifdef CONFIG_RTK_VLAN_SUPPORT		
		REG32(PVCR0) = (LAN4_VID << 16) | WAN_VID;
		REG32(PVCR1) = (LAN2_VID << 16) | LAN3_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;
		cp->port = BIT(4);		
		setting[0].portmask = BIT(4);
#endif
		// eth1(WAN) VLAN
		cp = reNet[1]->priv;
		cp->port = BIT(0);
		setting[1].portmask = BIT(0);
		
		totalVlans = ETH_INTF_NUM;
	}
	else
#endif
	
#ifdef CONFIG_RTL8196_RTL8366
	{
		int iport;
		int32 retval = 0;
		rtl8366rb_vlanConfig_t vlancfg_8366;

		/* for lan */
		if(count == 2) // gateway
		{
			savOP_MODE_value=count;
			memset(&vlancfg_8366, 0, sizeof(rtl8366rb_vlanConfig_t));
			vlancfg_8366.fid = 0;
			vlancfg_8366.mbrmsk = (RTL8366RB_LANPORT&RTL8366RB_PORTMASK)|RTL8366RB_GMIIPORT;
			vlancfg_8366.untagmsk = vlancfg_8366.mbrmsk&(~RTL8366RB_GMIIPORT);
			vlancfg_8366.vid = RTL_LANVLANID;
			retval = rtl8366rb_setVlan(&vlancfg_8366);
			/*need set pvid??*/
			for(iport=0;iport<8;iport++)
			{
				if  ((1<<iport)&vlancfg_8366.mbrmsk)
				{
					retval = rtl8366rb_setVlanPVID(iport, vlancfg_8366.vid, 0);
				}
			}
			/* for wan */
			iport=0;
			retval = 0;

			vlancfg_8366.fid =0;
			vlancfg_8366.mbrmsk = (RTL8366RB_WANPORT&RTL8366RB_PORTMASK)|RTL8366RB_GMIIPORT;
			vlancfg_8366.untagmsk = vlancfg_8366.mbrmsk&(~RTL8366RB_GMIIPORT);
			vlancfg_8366.vid = RTL_WANVLANID;
			retval = rtl8366rb_setVlan(&vlancfg_8366);
			/*need set pvid??*/
			for(iport=0;iport<8;iport++)
			{
				if  ((1<<iport)&vlancfg_8366.mbrmsk)
				{
					retval = rtl8366rb_setVlanPVID(iport, vlancfg_8366.vid, 0);
				}
			}
		}
		else
		{
			savOP_MODE_value=count;
			{
                        /* for lan */
                        int iport;
                        rtl8366rb_vlanConfig_t vlancfg_8366;

                        memset(&vlancfg_8366, 0, sizeof(rtl8366rb_vlanConfig_t));
                        vlancfg_8366.fid = 0;
                        vlancfg_8366.mbrmsk = ((RTL8366RB_WANPORT|RTL8366RB_LANPORT)&RTL8366RB_PORTMASK)|RTL8366RB_GMIIPORT;
                        vlancfg_8366.untagmsk = vlancfg_8366.mbrmsk&(~RTL8366RB_GMIIPORT);
                        vlancfg_8366.vid = RTL_LANVLANID;
                        retval = rtl8366rb_setVlan(&vlancfg_8366);
                        /*need set pvid??*/
                        for(iport=0;iport<8;iport++)
                                if  ((1<<iport)&vlancfg_8366.mbrmsk)
                                {
                                        retval = rtl8366rb_setVlanPVID(iport, vlancfg_8366.vid, 0);
                                }
                	}
                {
                        /* for wan clear it */
                        int iport;
                        rtl8366rb_vlanConfig_t vlancfg_8366;

                        vlancfg_8366.fid = 0;
                        vlancfg_8366.mbrmsk = 0;
                        vlancfg_8366.untagmsk = 0;
                        vlancfg_8366.vid = RTL_WANVLANID;
                        retval = rtl8366rb_setVlan(&vlancfg_8366);
                }

		}
	}
#elif defined(CONFIG_RTL8196B_GW_MP)
	{
 		REG32(PVCR0) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR1) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;
		cp = reNet[0]->priv;
		setting[0].portmask = ALL_PORTS;
		cp->port = ALL_PORTS;
		cp = reNet[1]->priv;
		setting[1].portmask = 0;
		cp->port = 0;
	}
#else
	{
	if(count == 2) // gateway
	{
		REG32(PVCR0) = cPVCR[wan_port][0];
		REG32(PVCR1) = cPVCR[wan_port][1];
		REG32(PVCR2) = cPVCR[wan_port][2];
		REG32(PVCR3) = cPVCR[wan_port][3];
		// eth0(LAN) VLAN
		cp = reNet[0]->priv;
		cp->port = ALL_PORTS & (~BIT(wan_port));
		setting[0].portmask = ALL_PORTS & (~BIT(wan_port));
		// eth1(WAN) VLAN
		cp = reNet[1]->priv;
		cp->port = BIT(wan_port);
		setting[1].portmask = BIT(wan_port);
	}
	else if(count == 1) // bridge, WISP
	{
		REG32(PVCR0) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR1) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;
		cp = reNet[0]->priv;
		setting[0].portmask = ALL_PORTS;
		cp->port = ALL_PORTS;
		cp = reNet[1]->priv;
		setting[1].portmask = 0;
		cp->port = 0;	
	}
	else
	{
 		return;
	}	
	}
#endif

#ifndef CONFIG_RTL8196_RTL8366
    	swCore_init(-2);

#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	{
	unsigned long flags;
	save_and_cli(flags);
	
	addACLRule(0);

	if (L2_table_disabled)
	  	REG32(MSCR) |= EN_IN_ACL;
	else 
	  	REG32(MSCR) &= (~EN_IN_ACL);

	restore_flags(flags);
	}
#endif
	
    	for (ethno = 0; ethno < totalVlans; ethno++)
    	{        
    	
		// Create NetIF 
		bzero((void *) &np, sizeof(rtl_netif_param_t));
		np.vid = setting[ethno].vid;
		np.valid = 1;
		memcpy(&np.gMac, &setting[ethno].mac[0], 6);
		np.macAddrNumber = 1;
		np.mtu = 1500;
		ret = swCore_netifCreate(ethno, &np);
		if ( ret != 0 )
			printk("865x-nic: swCore_netifCreate() failed:%d\n", ret );
            
		// Create VLAN 
		bzero((void *) &vp, sizeof(rtl_vlan_param_t));
		vp.egressUntag = setting[ethno].portmask;
		vp.memberPort = setting[ethno].portmask;
		vp.fid = ethno;

		ret = swCore_vlanCreate(setting[ethno].vid, &vp);  //P1-P4

#ifdef CONFIG_RTK_VLAN_SUPPORT
		if (L2_table_disabled) 
			REG32(SWTCR0) = REG32(SWTCR0)  | EnUkVIDtoCPU;
		else
			REG32(SWTCR0) = REG32(SWTCR0)  & (~EnUkVIDtoCPU);
#endif

		if ( ret != 0 )
			printk("eth865x: SoftNAT_OP_Mode() failed:%d\n", ret );  
    	}
#endif
}
#endif


#ifndef CONFIG_RTL_KERNEL_MIPS16_DRVETH
void __restore_flags__(unsigned long *x)
{	
	__restore_flags(*x);
}

void __save_and_cli__(unsigned long *x)
{	
	unsigned long flags;		
	__save_and_cli(flags);	
	*x = flags;			
}
#endif

module_init(rtl865x_init);
module_exit(rtl865x_exit);

