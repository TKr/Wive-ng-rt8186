// fragmented packet/* 8186nic.c: A Linux Ethernet driver for the RealTek 8186 chips. */

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

#if defined(CONFIG_RE8305) || defined(CONFIG_RE8306)
#include "re830x.h"
#endif

#define DRV_NAME		"8186NIC"
#define DRV_VERSION		"0.1.2-NG"
#define DRV_RELDATE		"02.09.2009"

#define DYNAMIC_ADJUST_TASKLET

#undef MII_EXTENTION 	//for future
#undef CP_VLAN_TAG_USED //for portmap (conflict with VLAN)

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
#define CP_VLAN_TX_TAG(tx_desc,vlan_tag_value) \
	do { (tx_desc)->opts2 = (vlan_tag_value); } while (0)
#else
#define CP_VLAN_TX_TAG(tx_desc,vlan_tag_value) \
	do { (tx_desc)->opts2 = 0; } while (0)
#endif

#define cpw16(reg,val)  writew((val), (u32)cp->regs + (reg))
#define cpw16_f(reg,val) do {                   \
        writew((val), (u32)cp->regs + (reg));   \
        readw(cp->regs + (reg));                \
        } while (0)
                        
/* proc entry for rtl8186 */
static struct proc_dir_entry * root_ethX_dir;

/* These identify the driver base version and may not be removed. */
static char version[] __devinitdata =
KERN_INFO DRV_NAME " Ethernet driver v" DRV_VERSION " (" DRV_RELDATE ")\n";

MODULE_AUTHOR("Arthur Yang <ysy@realtek.com.tw>");
MODULE_DESCRIPTION("RealTek RTL8186 series 10/100 Ethernet driver");
MODULE_LICENSE("GPL");

#ifdef CONFIG_RTL8186_ETH_DEBUG
static int debug = -1;
MODULE_PARM (debug, "i");
MODULE_PARM_DESC (debug, "RTL8186NIC bitmapped message enable number");
#endif

/* Maximum number of multicast addresses to filter (vs. Rx-all-multicast).
   The RTL chips use a 64 element hash table based on the Ethernet CRC.  */
static int multicast_filter_limit = 32;
MODULE_PARM (multicast_filter_limit, "i");
MODULE_PARM_DESC (multicast_filter_limit, "RTL8186NIC maximum number of filtered multicast addresses");

#define PFX						DRV_NAME ": "
#define CP_DEF_MSG_ENABLE		(NETIF_MSG_DRV		| \
								 NETIF_MSG_PROBE 	| \
								 NETIF_MSG_LINK)
#define CP_REGS_SIZE			(0xff + 1)
#define RTL8186_RX_RING_SIZE	32
#define RTL8186_TX_RING_SIZE	64
#define DESC_ALIGN		0x100
#define UNCACHE_MASK		0xa0000000

#define RTL8186_RXRING_BYTES	((sizeof(struct dma_desc) * (RTL8186_RX_RING_SIZE+1)) + DESC_ALIGN)

#define RTL8186_TXRING_BYTES	((sizeof(struct dma_desc) * (RTL8186_TX_RING_SIZE+1)) + DESC_ALIGN)

#define NEXT_TX(N)		(((N) + 1) & (RTL8186_TX_RING_SIZE - 1))
#define NEXT_RX(N)		(((N) + 1) & (RTL8186_RX_RING_SIZE - 1))
#define TX_HQBUFFS_AVAIL(CP)					\
	(((CP)->tx_hqtail <= (CP)->tx_hqhead) ?			\
	  (CP)->tx_hqtail + (RTL8186_TX_RING_SIZE - 1) - (CP)->tx_hqhead : \
	  (CP)->tx_hqtail - (CP)->tx_hqhead - 1)

#define PKT_BUF_SZ		1536	/* Size of each temporary Rx buffer.*/
#define RX_OFFSET		2

/* The following settings are log_2(bytes)-4:  0 == 16 bytes .. 6==1024, 7==end of packet. */

/* hardware minimum and maximum for a single frame's data payload */
#define CP_MIN_MTU      96              /* RFC2625 */
#define CP_MAX_MTU      65280           /* RFC2625 */

#define RTL_W32(reg, value)	(*(volatile u32*)(cp->regs+reg)) = (u32)value
#define RTL_W16(reg, value)	(*(volatile u16*)(cp->regs+reg)) = (u16)value
#define RTL_W8(reg, value)	(*(volatile u8*)(cp->regs+reg)) = (u8)value
#define RTL_R32(reg)		(*(volatile u32*)(cp->regs+reg))
#define RTL_R16(reg)		(*(volatile u16*)(cp->regs+reg))
#define RTL_R8(reg)		(*(volatile u8*)(cp->regs+reg))

enum PHY_REGS {
	TXFCE	= BIT(7),
	RXFCE	= BIT(6),
	SP100	= BIT(3),
	LINK	= BIT(2),
	TXPF	= BIT(1),
	RXPF	= BIT(0),
	FORCE_TX = BIT(5),
};

enum {
	NIC0,
	NIC1,
	NICEND,
};

enum {
	NIC0IRQ=4,
	NIC1IRQ=5,
};

enum {
	NIC0BASE=0xbd200000,
	NIC1BASE=0xbd300000,
};

enum {
	/* NIC register offsets */
	IDR0 			=0,	/* Ethernet ID */
	IDR1 			=0x1,	/* Ethernet ID */
	IDR2 			=0x2,	/* Ethernet ID */
	IDR3 			=0x3,	/* Ethernet ID */
	IDR4 			=0x4,	/* Ethernet ID */
	IDR5 			=0x5,	/* Ethernet ID */
	MAR0 			=0x8,	/* Multicast register */
	MAR1 			=0x9,
	MAR2 			=0xa,
	MAR3 			=0xb,
	MAR4 			=0xc,
	MAR5 			=0xd,
	MAR6 			=0xe,
	MAR7 			=0xf,
	TXOKCNT			=0x10,
	RXOKCNT			=0x12,
	TXERR 			=0x14,
	RXERRR 			=0x16,
	MISSPKT 		=0x18,
	FAE			=0x1A,
	TX1COL 			=0x1c,
	TXMCOL			=0x1e,
	RXOKPHY			=0x20,
	RXOKBRD			=0x22,
	RXOKMUL			=0x24,
	TXABT			=0x26,
	TXUNDRN			=0x28,
	TRSR			=0x34,
	CMD			=0x3B,
	IMR			=0x3C,
	ISR			=0x3E,
	TCR			=0x40,
	RCR			=0x44,
	MSR			=0x58,
	MIIAR			=0x5C,
	CpCmd           	=0xE0,	  /* C+ Command register (C+ mode only) */
	TxFDP1			=0x1300,
	TxCDO1			=0x1304,
	TXCPO1			=0x1308,
	TXPSA1			=0x130A,
	TXCPA1			=0x130C,
	LastTxCDO1		=0x1310,
	TXPAGECNT1		=0x1312,
	Tx1ScratchDes		=0x1350,
	TxFDP2			=0x1380,
	TxCDO2			=0x1384,
	TXCPO2			=0x1388,
	TXPSA2			=0x138A,
	TXCPA2			=0x138C,
	LASTTXCDO2		=0x1390,
	TXPAGECNT2		=0x1392,
	Tx2ScratchDes		=0x13A0,
	RxFDP			=0x13F0,
	RxCDO			=0x13F4,
	RxRingSize		=0x13F6,
	RxCPO			=0x13F8,
	RxPSA			=0x13FA,
	RXPLEN			=0x1403,
	//LASTRXCDO		=0x1402,
	RXPFDP			=0x1404,
	RXPAGECNT		=0x1408,
	RXSCRATCHDES		=0x1410,
	EthrntRxCPU_Des_Num	=0x1430,
	EthrntRxCPU_Des_Wrap 	=0x1431,
	Rx_Pse_Des_Thres 	=0x1432,

	IO_CMD 			=0x1434,
	MII_RE 			=BIT(3),
	MII_TE 			=BIT(2),
	TX_FNL 			=BIT(1),
	TX_FNH 			=BIT(0),
};

enum RTL8186_STATUS_REGS {
	/* Tx and Rx status descriptors */
	DescOwn		= (1 << 31), /* Descriptor is owned by NIC */
	RingEnd		= (1 << 30), /* End of descriptor ring */
	FirstFrag	= (1 << 29), /* First segment of a packet */
	LastFrag	= (1 << 28), /* Final segment of a packet */
	TxError         = (1 << 23), /* Tx error summary */
	RxError		= (1 << 20), /* Rx error summary */
	IPCS		= (1 << 18), /* Calculate IP checksum */
	UDPCS		= (1 << 17), /* Calculate UDP/IP checksum */
	TCPCS		= (1 << 16), /* Calculate TCP/IP checksum */
	TxVlanTag	= (1 << 16), /* Add VLAN tag */
	RxVlanTagged	= (1 << 16), /* Rx VLAN tag available */
	IPFail		= (1 << 15), /* IP checksum failed */
	UDPFail		= (1 << 14), /* UDP/IP checksum failed */
	TCPFail		= (1 << 13), /* TCP/IP checksum failed */
	NormalTxPoll	= (1 << 6),  /* One or more normal Tx packets to send */
	PID1		= (1 << 17), /* 2 protocol id bits:  0==non-IP, */
	PID0		= (1 << 16), /* 1==UDP/IP, 2==TCP/IP, 3==IP */
	RxProtoTCP	= 1,
	RxProtoUDP	= 2,
	RxProtoIP	= 3,
	RxErrFrame	= (1 << 27), /* Rx frame alignment error */
	RxMcast		= (1 << 26), /* Rx multicast packet rcv'd */
	RxErrCRC	= (1 << 18), /* Rx CRC error */
	RxErrRunt	= (1 << 19), /* Rx error, packet < 64 bytes */
	RWT		= (1 << 21), /* Rx  */
	E8023		= (1 << 22), /* Receive Ethernet 802.3 packet  */
	TxCRC		= (1 << 23),
	PPPOE		= (1 << 23),
	RxVlanOn	= (1 << 2),  /* Rx VLAN de-tagging enable */
	RxChkSum	= (1 << 1),  /* Rx checksum offload enable */
};

enum RTL8186_THRESHOLD_REGS {
	THVAL		= 2,
	RINGSIZE	= 2,

	LOOPBACK	= (0x3 << 8),
 	AcceptErr	= 0x20,	     /* Accept packets with CRC errors */
	AcceptRunt	= 0x10,	     /* Accept runt (<64 bytes) packets */
	AcceptBroadcast	= 0x08,	     /* Accept broadcast packets */
	AcceptMulticast	= 0x04,	     /* Accept multicast packets */
	AcceptMyPhys	= 0x02,	     /* Accept pkts with our MAC as dest */
	AcceptAllPhys	= 0x01,	     /* Accept all pkts w/ physical dest */
	AcceptAll		= AcceptBroadcast | AcceptMulticast | AcceptMyPhys |
					  AcceptAllPhys | AcceptErr | AcceptRunt,
	AcceptNoBroad	= AcceptMulticast |AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	AcceptNoMulti	= AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	NoErrAccept		= AcceptBroadcast | AcceptMulticast | AcceptMyPhys |  AcceptAllPhys,
};

enum RTL8186_ISR_REGS {
	SW_INT 		= BIT(10),
	TX_EMPTY	= BIT(9),
	LINK_CHG	= BIT(8),
	TX_ERR		= BIT(7),
	TX_OK		= BIT(6),
	RX_EMPTY	= BIT(5),
	RX_FIFOOVR	= BIT(4),
	RX_ERR		= BIT(2),
	RX_OK		= BIT(0),
};

enum RTL8186_IOCMD_REG {
#ifdef DELAY_RX
	RX_MIT 		= 1,
#else
	RX_MIT		= 3,
#endif
	RX_TIMER 	= 1,
	RX_FIFO 	= 2,
	TX_FIFO		= 1,
#ifdef DELAY_RX
	TX_MIT		= 1,
#else
	TX_MIT		= 3,
#endif
	TX_POLL		= 1 << 0,
	CMD_CONFIG	= 0x3c | RX_MIT << 8 | RX_FIFO << 11 |  RX_TIMER << 13 |
				  TX_MIT << 16 | TX_FIFO<<19,
};

static const u32 rtl8186_intr_mask =
	SW_INT | LINK_CHG |
	RX_OK | RX_ERR | RX_EMPTY | RX_FIFOOVR |
	TX_OK | TX_ERR | TX_EMPTY;


typedef struct dma_desc {
	u32		opts1;		// status
	u32		addr;		// buffer address
	u32		opts2;		// vlan stuff
	u32		opts3;		// partial checksum
} DMA_DESC;


struct ring_info {
	struct sk_buff	*skb;
	dma_addr_t		mapping;
	unsigned		frag;
};

struct cp_extra_stats {
	unsigned long	rx_frags;
	unsigned long 	tx_timeouts;
};

#ifdef CONFIG_VLAN_QOS
struct qos_node {
	unsigned char	valid;
	unsigned char	addr[6];
	unsigned char	vlan_tagged;
	unsigned char	priority;
	unsigned char	vlan_id_h;
	unsigned char	vlan_id_l;
};
#endif

struct re_private {
	unsigned		tx_hqhead;
	unsigned		tx_hqtail;
	unsigned		tx_lqhead;
	unsigned		tx_lqtail;
	unsigned		rx_tail;
	void			*regs;
	struct net_device	*dev;
#ifdef DELAY_RX
	struct tasklet_struct 	rx_tasklet;
#endif
	spinlock_t		lock;
	DMA_DESC		*rx_ring;
	DMA_DESC		*tx_hqring;
	DMA_DESC		*tx_lqring;
	struct ring_info	tx_skb[RTL8186_TX_RING_SIZE];
	struct ring_info	rx_skb[RTL8186_RX_RING_SIZE];
	unsigned		rx_buf_sz;
	dma_addr_t		ring_dma;

#if CP_VLAN_TAG_USED
	struct vlan_group	*vlgrp;
#endif

	u32			msg_enable;
	struct net_device_stats net_stats;
	struct cp_extra_stats	cp_stats;
	struct pci_dev		*pdev;
	u32			rx_config;
	u16 			cpcmd;
	struct sk_buff		*frag_skb;
	unsigned		dropping_frag : 1;
	char*			rxdesc_buf;
	char*			txdesc_buf;
	struct mii_if_info	mii_if;
#ifdef CONFIG_VLAN_QOS
	struct qos_node	qosnode_table[8][8];
	unsigned char	qosnode_index[8];
#endif

#ifdef DYNAMIC_ADJUST_TASKLET
	struct timer_list expire_timer; 
#endif
	unsigned int	tx_avarage;
	unsigned int	tx_peak;
	unsigned int	rx_avarage;
	unsigned int	rx_peak;
	unsigned int	tx_byte_cnt;
	unsigned int	rx_byte_cnt;	

};
struct re_private *reDev[2]={NULL, NULL};

extern int r3k_flush_dcache_range(int, int);
static void __rtl8186_set_rx_mode(struct net_device *dev);
static inline void rtl8186_tx(struct re_private *cp);
static void rtl8186_clean_rings(struct re_private *cp);
static void rtl8186_tx_timeout(struct net_device *dev);
static int rtl8186_set_hwaddr(struct net_device *dev, void *addr);

static int eth_flag=0;	// bit0 - disable DELAY_RX, bit1 - disable BR_SHORTCUT

#ifdef DYNAMIC_ADJUST_TASKLET
static int rx_pkt_thres=0;
static int rx_cnt;
#endif

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
#define RTL8306_MIB_CNT1			0
#define RTL8306_MIB_CNT2			1
#define RTL8306_MIB_CNT3			2
#define RTL8306_MIB_CNT4			3
#define RTL8306_MIB_CNT5			4
#define RTL8306_MIB_RESET			0
#define RTL8306_MIB_START			1
#define RTL8306_MIB_BYTE			0
#define RTL8306_MIB_PKT				1

#define SUCCESS 0
#define FAILED -1

#define LED_MAX_PACKET_CNT			5000
#define LED_MAX_SCALE				100
#define LED_NOBLINK_TIME			120	// time more than 1-sec timer interval
#define LED_INTERVAL_TIME			50	// 500ms
#define LED_ON_TIME				4	// 40ms
#define LED_ON					0
#define LED_OFF					1
#define LINK_STATUS				1
#define LINK_STATE_CHANGE 0x80000000

#define UINT32_DIFF(a, b)		((a >= b)? (a - b):(0xffffffff - b + a + 1))

typedef int int32;

struct ctrl_led {
	struct timer_list	LED_Timer;	
	unsigned int		LED_Interval;
	unsigned char		LED_Toggle;
	unsigned char		LED_ToggleStart;
	unsigned int		LED_tx_cnt_log;
	unsigned int		LED_rx_cnt_log;
	unsigned int		LED_tx_cnt;
	unsigned int		LED_rx_cnt;
	unsigned int		link_status;
} led_cb[5];

static unsigned short int MII_read(unsigned short int phyaddr, unsigned short int regaddr, unsigned char eth);
static void MII_write(unsigned short int phyaddr, unsigned short int regaddr, unsigned short int data, unsigned char eth);
static int32 rtl8306_getAsicMibCounter(uint32 port, uint32 counter, uint32 *value) ;

static void rtl8306_page_select(unsigned int pagenumber)
{	
	pagenumber = (pagenumber + 2) & 0x03;
	MII_write(0,16,(MII_read(0,16,0)&0x7ffd)|(pagenumber&0x02)|((pagenumber&0x01)<<15),0);	
}

static void set_led(int port, int flag)
{
	unsigned short val;
	
	rtl8306_page_select(3);

	if (port == 0)
		port = 3;
	else if (port == 1)
		port = 2;
	else if (port == 2)
		port = 1;
	else if (port == 3)
		port = 0;

	val = (1 << port) | (1 << (port + 5));
	if (flag == LED_OFF)	
		MII_write(3, 24, MII_read(3 ,24, 0)&~val, 0); 
	else
		MII_write(3, 24, MII_read(3 ,24, 0)|val, 0); 

	MII_read(4, 0, 0); // switch to MII to wan port to fix Eth auto link issue
}

static void LED_Interval_timeout(unsigned long port)
{
	struct ctrl_led *cb	= &led_cb[port];
	unsigned long flags;

	save_flags(flags);cli();

	if (cb->link_status & LINK_STATE_CHANGE) {
		cb->link_status &= ~LINK_STATE_CHANGE;	
		if (cb->link_status & LINK_STATUS)	 
			cb->LED_ToggleStart = LED_ON;
		else
			cb->LED_ToggleStart = LED_OFF;
		cb->LED_Toggle = cb->LED_ToggleStart;
		set_led(port, cb->LED_Toggle);			
	}
	else {
		if (cb->link_status & LINK_STATUS)	 
			set_led(port, cb->LED_Toggle);	
	}

	if (cb->link_status & LINK_STATUS)	 {
		if (cb->LED_Toggle == cb->LED_ToggleStart)
			mod_timer(&cb->LED_Timer, jiffies + cb->LED_Interval);
		else
			mod_timer(&cb->LED_Timer, jiffies + LED_ON_TIME);
		cb->LED_Toggle = (cb->LED_Toggle + 1) % 2;				
	}
	restore_flags(flags);
}

void calculate_led_interval(int port)
{
	struct ctrl_led *cb	= &led_cb[port];

	unsigned int delta = 0;
	int i, scale_num=0;

	// calculate counter delta
	delta += UINT32_DIFF(cb->LED_tx_cnt, cb->LED_tx_cnt_log);
	delta += UINT32_DIFF(cb->LED_rx_cnt, cb->LED_rx_cnt_log);
	cb->LED_tx_cnt_log = cb->LED_tx_cnt;
	cb->LED_rx_cnt_log = cb->LED_rx_cnt;

	// update interval according to delta
	if (delta == 0) {
		if (cb->LED_Interval == LED_NOBLINK_TIME)
			mod_timer(&cb->LED_Timer, jiffies + cb->LED_Interval);
		else
			cb->LED_Interval = LED_NOBLINK_TIME;
	}
	else
	{	
		scale_num = LED_MAX_PACKET_CNT / LED_MAX_SCALE;
		for (i=1; i<=LED_MAX_SCALE; i++) {
			if (delta < i*scale_num)
				break;
		}

		cb->LED_Interval = ((LED_MAX_SCALE-i+1)*LED_INTERVAL_TIME)/LED_MAX_SCALE;

		if (cb->LED_Interval < LED_ON_TIME)
			cb->LED_Interval = LED_ON_TIME;
	}
}

void update_mib_counter(int port)
{
	struct ctrl_led *cb	= &led_cb[port];

	if	(MII_read(port , 1, 0) & 4) {
		if (!(cb->link_status & LINK_STATUS)) {
			cb->link_status = LINK_STATE_CHANGE | 1;
		}
		if (port == 4) {
			if (reDev[1]) {
				cb->LED_tx_cnt =	reDev[1]->net_stats.tx_packets;
				cb->LED_rx_cnt =	reDev[1]->net_stats.rx_packets;		
			}
		}
		else {
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT1, (uint32 *)&cb->LED_tx_cnt);
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT2, (uint32 *)&cb->LED_rx_cnt);
		}	
	}
	else {
		if (cb->link_status & LINK_STATUS) {
			cb->link_status = LINK_STATE_CHANGE;
		}				
	}	
}

void init_led_ctrl(int port)
{
	struct ctrl_led *cb	= &led_cb[port];

	memset(cb, '\0', sizeof(struct ctrl_led));

	update_mib_counter(port);		
	calculate_led_interval(port);	
	cb->link_status |= LINK_STATE_CHANGE;

	init_timer(&cb->LED_Timer);
	cb->LED_Timer.expires = jiffies + cb->LED_Interval;
	cb->LED_Timer.data = (unsigned long)port;
	cb->LED_Timer.function = &LED_Interval_timeout;	
	mod_timer(&cb->LED_Timer, jiffies + cb->LED_Interval);

	LED_Interval_timeout(port);		
}

void disable_led_ctrl(int port)
{
	struct ctrl_led *cb	= &led_cb[port];
	set_led(port, LED_OFF);

	if (timer_pending(&cb->LED_Timer))	
		del_timer_sync(&cb->LED_Timer);
}
#endif // CONFIG_PATCH_8306_CTRL_LED_BY_CPU


#ifdef CONFIG_VLAN_QOS
static __inline__ int mac_hash(unsigned char *mac)
{
	unsigned long x;

	x = mac[0];
	x = (x << 2) ^ mac[1];
	x = (x << 2) ^ mac[2];
	x = (x << 2) ^ mac[3];
	x = (x << 2) ^ mac[4];
	x = (x << 2) ^ mac[5];

	x ^= x >> 8;

	return x & (8 - 1);
}

void qosnode_insert(struct re_private *cp, unsigned char *sa, unsigned char vlan_tagged,
				unsigned char priority, unsigned char vlan_id_h, unsigned char vlan_id_l)
{
	unsigned int index, i;
	struct qos_node *pbuf;

	index = mac_hash(sa);
	for (i=0; i<8; i++)
	{
		if ((cp->qosnode_table[index][i].valid) && !(memcmp(cp->qosnode_table[index][i].addr, sa, 6))) {
#ifdef CONFIG_RTL8186_QOS_DEBUG
			printk("qosnode: entry %02x%02x%02x%02x%02x%02x found\n",
				sa[0],sa[1],sa[2],sa[3],sa[4],sa[5]);
#endif
			cp->qosnode_table[index][i].vlan_tagged = vlan_tagged;
			if (vlan_tagged) {
				cp->qosnode_table[index][i].priority = priority;
				cp->qosnode_table[index][i].vlan_id_h = vlan_id_h;
				cp->qosnode_table[index][i].vlan_id_l = vlan_id_l;
			}
			return;
		}
	}

	// not found
	pbuf = &cp->qosnode_table[index][cp->qosnode_index[index]];
	cp->qosnode_index[index] = (cp->qosnode_index[index] + 1) & (8 - 1);
	pbuf->valid = 1;
	memcpy(pbuf->addr, sa, 6);
	pbuf->vlan_tagged = vlan_tagged;
	if (vlan_tagged) {
		pbuf->priority = priority;
		pbuf->vlan_id_h = vlan_id_h;
		pbuf->vlan_id_l = vlan_id_l;
	}
#ifdef CONFIG_RTL8186_QOS_DEBUG
	printk("qosnode: entry %02x%02x%02x%02x%02x%02x insert\n",
		sa[0],sa[1],sa[2],sa[3],sa[4],sa[5]);
#endif
}

struct qos_node *qosnode_lookup(struct re_private *cp, unsigned char *da)
{
	unsigned int index, i;

	index = mac_hash(da);
	for (i=0; i<8; i++)
	{
		if ((cp->qosnode_table[index][i].valid) && !(memcmp(cp->qosnode_table[index][i].addr, da, 6))) {
#ifdef CONFIG_RTL8186_QOS_DEBUG
			printk("qosnode: entry %02x%02x%02x%02x%02x%02x lookup\n",
				da[0],da[1],da[2],da[3],da[4],da[5]);
#endif
			return &cp->qosnode_table[index][i];
		}
	}
	return 0;
}
#endif // VLAN_QOS

static int write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	unsigned char tmp;

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(&tmp, buffer, 1)) {
	    eth_flag = tmp - '0';
	    return count;
    }
    return -EFAULT;
}

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
	    	eth_flag |= 1;	// disable tasklet
		}		
	    return count;
    }
    return -EFAULT;
}
#endif


#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
static int page, phy, reg;
static int write_proc_mii(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	unsigned char tmpbuf[100];
	unsigned int val;

	if (count < 2)
		return -EFAULT;

    if (buffer && !copy_from_user(tmpbuf, buffer, count)) {
		sscanf(tmpbuf, "%d %d %d %d", &page, &phy, &reg, &val);

		printk("page=%d, phy=%d, register=%d, val=0x%x\n", page, phy, reg, val);

		if (val != 0) {
			rtl8306_page_select(page);
			MII_write(phy, reg, val, 0); 
		}
	    return count;
    }
    return -EFAULT;
}

static int read_proc_mii(char *pa, char **start, off_t off,
		     int count, int *eof, void *data)
{
		int len;

		rtl8306_page_select(page);	
		len = sprintf(pa, "page=%d, phy=%d, register=%d, val=0x%x\n", page, phy, reg, MII_read(phy, reg, 0));

      if (len <= off+count) *eof = 1;
      *start = pa + off;
      len -= off;
      if (len>count) len = count;
      if (len<0) len = 0;
      return len;
}

static int read_proc_mib(char *pa, char **start, off_t off,
		     int count, int *eof, void *data)
{
		int len=0;
		uint32 port, unit, value;
	
		for (port = 0; port <5; port ++) {
			len += sprintf(pa+len, "Port%d:\n", port);
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT1, &value);												
			rtl8306_getAsicMibCounterUnit(port, RTL8306_MIB_CNT1, &unit);
			len += sprintf(pa+len, "TX COUNT = %d (%s; ", value, unit == RTL8306_MIB_BYTE ? "byte)" : "pkt)");			
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT2, &value);
			rtl8306_getAsicMibCounterUnit(port, RTL8306_MIB_CNT2, &unit);
			len += sprintf(pa+len, "RX COUNT = %d (%s; ", value, unit == RTL8306_MIB_BYTE ? "byte)" : "pkt)");			
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT3, &value);
			len += sprintf(pa+len, "RX Drop = %d(pkt);\n", value);				
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT4, &value);
			len += sprintf(pa+len, "RX CRCERR =%d(pkt); ", value);
			rtl8306_getAsicMibCounter(port, RTL8306_MIB_CNT5, &value);
			len += sprintf(pa+len, "RX ERR =%d(pkt)\n", value);				
			len += sprintf(pa+len, "Link =%s\n", ((MII_read(port , 1, 0) & 4) ? "ON" : "OFF"));			
		}	

		if (len <= off+count) *eof = 1;
		*start = pa + off;
		len -= off;
		if (len>count) len = count;
		if (len<0) len = 0;
		return len;
}
#endif // CONFIG_PATCH_8306_CTRL_LED_BY_CPU

static inline void rtl8186_set_rxbufsize(struct re_private *cp)
{
	unsigned int mtu = cp->dev->mtu;

	if (mtu > ETH_DATA_LEN)
		/* MTU + ethernet header + FCS + optional VLAN tag */
		cp->rx_buf_sz = mtu + ETH_HLEN + 8;
	else
		cp->rx_buf_sz = PKT_BUF_SZ;
}

static inline void rtl8186_rx_skb(struct re_private *cp, struct sk_buff *skb,
			      DMA_DESC *desc)
{
#ifdef BR_SHORTCUT
	extern struct net_device *get_shortcut_dev(unsigned char *da);
	struct net_device *dev;
#endif

	skb->protocol = eth_type_trans (skb, cp->dev);

	cp->net_stats.rx_packets++;
	cp->net_stats.rx_bytes += skb->len;
	cp->dev->last_rx = jiffies;
	cp->rx_byte_cnt += skb->len;
	
#ifdef CONFIG_VLAN_QOS
	if (desc->opts2 & RxVlanTagged) {
		unsigned char priority  = (desc->opts2 & 0x000000e0) >> 5;
		unsigned char vlan_id_h = (desc->opts2 & 0x0000000f);
		unsigned char vlan_id_l = (desc->opts2 & 0x0000ff00) >> 8;
		qosnode_insert(cp, skb->mac.ethernet->h_source, 1, priority, vlan_id_h, vlan_id_l);
		skb->cb[0] = priority;
	}
#endif

#if CP_VLAN_TAG_USED
	if (cp->vlgrp && (desc->opts2 & RxVlanTagged)) {
		vlan_hwaccel_rx(skb, cp->vlgrp, desc->opts2 & 0xffff);
	}
	else
#endif
	{
#ifdef BR_SHORTCUT
		if (!(skb->mac.ethernet->h_dest[0] & 0x01) &&
			!(eth_flag & 2) &&
			(cp->dev->br_port) &&
			((dev = get_shortcut_dev(skb->mac.ethernet->h_dest)) != NULL)) {
			skb_push(skb, ETH_HLEN);
			memcpy(skb->data, skb->mac.ethernet, ETH_HLEN);
			dev->hard_start_xmit(skb, dev);
		}
		else
#endif
		{

#ifdef DYNAMIC_ADJUST_TASKLET
			if (rx_pkt_thres > 0 && cp->dev->name[3] == '0') // eth0
				rx_cnt++;
#endif			
			netif_rx(skb);
		}
	}
}

static void rtl8186_rx_err_acct(struct re_private *cp, unsigned rx_tail,
			    u32 status, u32 len)
{
#ifdef CONFIG_RTL8186_ETH_DEBUG
	if (netif_msg_rx_err (cp))
		printk (KERN_DEBUG
			"%s: rx err, slot %d status 0x%x len %d\n",
			cp->dev->name, rx_tail, status, len);
#endif
	cp->net_stats.rx_errors++;
	if (status & RxErrFrame)
		cp->net_stats.rx_frame_errors++;
	if (status & RxErrCRC)
		cp->net_stats.rx_crc_errors++;
	if (status & RxErrRunt)
		cp->net_stats.rx_length_errors++;
}

static void rtl8186_rx_frag(struct re_private *cp, unsigned rx_tail,
			struct sk_buff *skb, u32 status, u32 len)
{
	struct sk_buff *copy_skb, *frag_skb = cp->frag_skb;
	unsigned orig_len = frag_skb ? frag_skb->len : 0;
	unsigned target_len = orig_len + len;
	unsigned first_frag = status & FirstFrag;
	unsigned last_frag = status & LastFrag;

	if (netif_msg_rx_status (cp))
		printk (KERN_DEBUG "%s: rx %s%sfrag, slot %d status 0x%x len %d\n",
			cp->dev->name,
			cp->dropping_frag ? "dropping " : "",
			first_frag ? "first " :
			last_frag ? "last " : "",
			rx_tail, status, len);

	cp->cp_stats.rx_frags++;

	if (!frag_skb && !first_frag)
		cp->dropping_frag = 1;
	if (cp->dropping_frag)
		goto drop_frag;

	copy_skb = dev_alloc_skb (target_len + RX_OFFSET);
	if (!copy_skb) {
		printk(KERN_WARNING "%s: rx slot %d alloc failed\n",
		       cp->dev->name, rx_tail);

		cp->dropping_frag = 1;
drop_frag:
		if (frag_skb) {
			dev_kfree_skb_irq(frag_skb);
			cp->frag_skb = NULL;
		}
		if (last_frag) {
			cp->net_stats.rx_dropped++;
			cp->dropping_frag = 0;
		}
		return;
	}

	copy_skb->dev = cp->dev;
	skb_reserve(copy_skb, RX_OFFSET);
	skb_put(copy_skb, target_len);
	if (frag_skb) {
		memcpy(copy_skb->data, frag_skb->data, orig_len);
		dev_kfree_skb_irq(frag_skb);
	}
	memcpy(copy_skb->data + orig_len, skb->data, len);

	copy_skb->ip_summed = CHECKSUM_NONE;

	if (last_frag) {
		if (status & (RxError)) {
			rtl8186_rx_err_acct(cp, rx_tail, status, len);
			dev_kfree_skb_irq(copy_skb);
		} else
			rtl8186_rx_skb(cp, copy_skb, &cp->rx_ring[rx_tail]);
		cp->frag_skb = NULL;
	} else {
		cp->frag_skb = copy_skb;
	}
}

static inline unsigned int rtl8186_rx_csum_ok(u32 status)
{
	unsigned int protocol = (status >> 16) & 0x3;

	if (likely((protocol == RxProtoTCP) && (!(status & TCPFail))))
		return 1;
	else if ((protocol == RxProtoUDP) && (!(status & UDPFail)))
		return 1;
	else if ((protocol == RxProtoIP) && (!(status & IPFail)))
		return 1;

	return 0;
}

static inline void rtl8186_rx(unsigned long task_priv)
{
	struct re_private *cp = (struct re_private *)task_priv;
	unsigned rx_tail = cp->rx_tail;
	unsigned rx_work = 100;

	u32 status, len;
	dma_addr_t mapping;
	struct sk_buff *skb, *new_skb;
	DMA_DESC *desc;
	unsigned buflen;

	while (rx_work--) {
		skb = cp->rx_skb[rx_tail].skb;
		if (!skb)
			BUG();

		desc = &cp->rx_ring[rx_tail];
		status = desc->opts1;
		if (status & DescOwn)
			break;

		// ethernet CRC-4 bytes- are padding after the payload
		len = (status & 0x0fff) - 4;

		mapping = cp->rx_skb[rx_tail].mapping;

		if ((status & (FirstFrag | LastFrag)) != (FirstFrag | LastFrag)) {
			rtl8186_rx_frag(cp, rx_tail, skb, status, len);
			goto rx_next;
		}

		if (status & (RxError)) {
			rtl8186_rx_err_acct(cp, rx_tail, status, len);
			goto rx_next;
		}

		if (netif_msg_rx_status(cp))
			printk(KERN_DEBUG "%s: rx slot %d status 0x%x len %d\n",
			       cp->dev->name, rx_tail, status, len);

		buflen = cp->rx_buf_sz + RX_OFFSET;
		new_skb = dev_alloc_skb (buflen);
		if (!new_skb) {
			cp->net_stats.rx_dropped++;
			goto rx_next;
		}
		if ((u32)new_skb->data &0x3)
			printk(KERN_DEBUG "new_skb->data unaligment %8x\n",(u32)new_skb->data);

		/*skb_reserve(new_skb, RX_OFFSET);*/
		new_skb->dev = cp->dev;

		/* Handle checksum offloading for incoming packets. */
		if (rtl8186_rx_csum_ok(status))
			skb->ip_summed = CHECKSUM_UNNECESSARY;
		else
			skb->ip_summed = CHECKSUM_NONE;

		skb_reserve(skb, RX_OFFSET);
		skb_put(skb, len);
		mapping =
		cp->rx_skb[rx_tail].mapping =
				(u32)new_skb->tail | UNCACHE_MASK;
		rtl8186_rx_skb(cp, skb, desc);

		cp->rx_skb[rx_tail].skb = new_skb;
		cp->rx_ring[rx_tail].addr = virt_to_bus((void *)mapping);

rx_next:
		if (rx_tail == (RTL8186_RX_RING_SIZE - 1))
			desc->opts1 = (DescOwn | RingEnd |
						  cp->rx_buf_sz);
		else
			desc->opts1 = (DescOwn | cp->rx_buf_sz);
		cp->rx_ring[rx_tail].opts2 = 0;
		rx_tail = NEXT_RX(rx_tail);
	}
	r3k_flush_dcache_range(0,0);
	if (!rx_work)
		printk(KERN_WARNING "%s: rx work limit reached\n", cp->dev->name);

	cp->rx_tail = rx_tail;

#ifdef DELAY_RX
	if (RTL_R16(IMR) == 0) {
		RTL_W16(IMR, rtl8186_intr_mask); // re-accept all interrupt
		RTL_R16(IMR);
	}
#endif
}

static inline void rtl8186_interrupt(int irq, void *dev_instance, struct pt_regs *regs)
{
	struct net_device *dev = dev_instance;
	struct re_private *cp = dev->priv;
	u16 status;

	spin_lock(&cp->lock);

	status = RTL_R16(ISR);
	RTL_W16(ISR,status);
	
#ifdef CONFIG_RTL8186_ETH_DEBUG
	printk("%s: intr, status %04x cmd %02x \n",dev->name,status,RTL_R8(CMD));
#endif
	if (!status || (status == 0xFFFF))
		return;

#ifdef CONFIG_RTL8186_ETH_DEBUG
	if (status & LINK_CHG)
		printk("%s: Link change\n",dev->name);
#endif
	if (netif_msg_intr(cp))
		printk(KERN_DEBUG "%s: intr, status %04x cmd %02x \n",
		        dev->name, status, RTL_R8(CMD));

	if (status & (RX_OK | RX_ERR | RX_EMPTY | RX_FIFOOVR))
	{
#ifdef CONFIG_RTL8186_ETH_DEBUG
		if(!(status & RX_OK))
			printk("%s rx error =%x\n",dev->name,status);
		if(status & RX_EMPTY)
			printk("eRDU ");
		if(status & RX_FIFOOVR)
			printk("eFIFO ");
#endif

#ifndef DELAY_RX
		rtl8186_rx((unsigned long)cp);
#else
		if (eth_flag & 1)
			rtl8186_rx((unsigned long)cp);
		else {
			RTL_W16(IMR, 0); // disable ISR
			tasklet_hi_schedule(&cp->rx_tasklet);
		}
#endif
	}

	if (status & (TX_OK | TX_ERR | TX_EMPTY | SW_INT))
	{
		if (!(status & TX_OK) && !(status & TX_EMPTY))
			printk("%s tx error =%x\n", dev->name, status);
		rtl8186_tx(cp);
	}

	spin_unlock(&cp->lock);
}

static inline void rtl8186_tx(struct re_private *cp)
{
	unsigned tx_head = cp->tx_hqhead;
	unsigned tx_tail = cp->tx_hqtail;

	while (tx_tail != tx_head) {
		struct sk_buff *skb;
		u32 status;

		status = (cp->tx_hqring[tx_tail].opts1);
		if (status & DescOwn)
			break;
		skb = cp->tx_skb[tx_tail].skb;
		cp->tx_hqring[tx_tail].addr = 0x0;
		cp->tx_hqring[tx_tail].opts1 = 0x0;

		if (!skb)
			BUG();

		cp->net_stats.tx_packets++;
		cp->net_stats.tx_bytes += skb->len;
		cp->tx_byte_cnt += skb->len;

		if (netif_msg_tx_done(cp))
			printk(KERN_DEBUG "%s: tx done, slot %d\n", cp->dev->name, tx_tail);
		dev_kfree_skb_irq(skb);
		cp->tx_skb[tx_tail].skb = NULL;
		tx_tail = NEXT_TX(tx_tail);
	}

	cp->tx_hqtail = tx_tail;

	if (netif_queue_stopped(cp->dev) && (TX_HQBUFFS_AVAIL(cp) > (MAX_SKB_FRAGS + 1)))
		netif_wake_queue(cp->dev);
}

static int rtl8186_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	unsigned entry;

	u32 eor;
#if CP_VLAN_TAG_USED
	u32 vlan_tag = 0;
#endif

#ifdef CONFIG_VLAN_QOS
	struct qos_node *pbuf;
#endif

#ifdef DELAY_RX
	struct Qdisc *q=dev->qdisc;
	int	     deq_loop=0;
	unsigned int flags;

dequeue_label:

	if (deq_loop)
	{
		save_flags(flags);cli();
		// fetch a new skb for tx
		if ((skb = q->dequeue(q)) == NULL)
		{
			// polling TX
			RTL_W32(IO_CMD, CMD_CONFIG | TX_POLL);
			restore_flags(flags);
			return 0;
		}
		restore_flags(flags);
#ifdef CONFIG_RTL8186_ETH_DEBUG
		printk("next tx skb\n");
#endif
	}
#endif

	spin_lock_irq(&cp->lock);

	/* This is a hard error, log it. */
	if (TX_HQBUFFS_AVAIL(cp) <= (skb_shinfo(skb)->nr_frags + 1)) {
		netif_stop_queue(dev);
		spin_unlock_irq(&cp->lock);
#ifdef CONFIG_RTL8186_ETH_DEBUG
		printk(KERN_ERR PFX "%s: Tx Ring full when queue awake!\n",dev->name);
#endif
#ifdef DELAY_RX
	 	if (deq_loop)
		{
			q->ops->requeue(skb, q);
			netif_schedule(dev);
		}
		RTL_W32(IO_CMD, CMD_CONFIG | TX_POLL);
#endif
		return 0;
	}

#if CP_VLAN_TAG_USED
	if (cp->vlgrp && vlan_tx_tag_present(skb))
		vlan_tag = TxVlanTag | vlan_tx_tag_get(skb);
#endif

#ifdef CONFIG_RTL8186_ETH_DEBUG
	if ( (skb->len>ETH_FRAME_LEN) )
		for(;;)
		{
			printk("error tx len = %d \n",skb->len);
		}
#endif

	entry = cp->tx_hqhead;
	eor = (entry == (RTL8186_TX_RING_SIZE - 1)) ? RingEnd : 0;
	// if skb contains a full/single packet
	if (skb_shinfo(skb)->nr_frags == 0) {
		DMA_DESC  *txd = &cp->tx_hqring[entry];
		u32 	  mapping, len;

		len = ETH_ZLEN < skb->len ? skb->len : ETH_ZLEN;
		eor = (entry == (RTL8186_TX_RING_SIZE - 1)) ? RingEnd : 0;
#if CP_VLAN_TAG_USED
		CP_VLAN_TX_TAG(txd, vlan_tag);
#endif
		mapping = (u32)skb->data|UNCACHE_MASK;
		txd->addr = virt_to_bus((void *)mapping);
		cp->tx_skb[entry].skb = skb;
		cp->tx_skb[entry].mapping = mapping;
		cp->tx_skb[entry].frag = 0;

		txd->opts1 = (eor | len | DescOwn | FirstFrag |
			LastFrag | TxCRC);

#ifdef CONFIG_VLAN_QOS
		pbuf = qosnode_lookup(cp, skb->data);
		if ((pbuf == 0) || (pbuf->vlan_tagged == 0))
			txd->opts2 = 0;
		else
			txd->opts2 = TxVlanTag | ((skb->cb[0] & 0x07) << 5) |
				pbuf->vlan_id_h | (pbuf->vlan_id_l << 8);
#endif

		entry = NEXT_TX(entry);
	} else {	// fragmented packet
		DMA_DESC *txd;
		u32 first_len, first_mapping;
		int frag, first_entry = entry;

		/* We must give this initial chunk to the device last.
		 * Otherwise we could race with the device.
		 */
		first_len = skb->len - skb->data_len;
		first_mapping = (u32)skb->data|UNCACHE_MASK;
		cp->tx_skb[entry].skb = skb;
		cp->tx_skb[entry].mapping = first_mapping;
		cp->tx_skb[entry].frag = 1;
		entry = NEXT_TX(entry);

		for (frag = 0; frag < skb_shinfo(skb)->nr_frags; frag++) {
			skb_frag_t *this_frag = &skb_shinfo(skb)->frags[frag];
			u32 len, mapping;
			u32 ctrl;

			len = this_frag->size;
			mapping = (u32)this_frag->page_offset|UNCACHE_MASK;

			eor = (entry == (RTL8186_TX_RING_SIZE - 1)) ? RingEnd : 0;
			ctrl = eor | len | DescOwn | TxCRC;
			if (frag == skb_shinfo(skb)->nr_frags - 1)
				ctrl |= LastFrag;

			txd = &cp->tx_hqring[entry];
#if CP_VLAN_TAG_USED
			CP_VLAN_TX_TAG(txd, vlan_tag);
#endif
			txd->addr = virt_to_bus((void *)mapping);
			txd->opts1 = (ctrl);
			cp->tx_skb[entry].skb = skb;
			cp->tx_skb[entry].mapping = mapping;
			cp->tx_skb[entry].frag = frag + 2;
			entry = NEXT_TX(entry);
		}

		txd = &cp->tx_hqring[first_entry];
#if CP_VLAN_TAG_USED
		CP_VLAN_TX_TAG(txd, vlan_tag);
#endif
		txd->addr = virt_to_bus((void *)first_mapping);
		eor = (first_entry == (RTL8186_TX_RING_SIZE - 1)) ? RingEnd : 0;
		txd->opts1 = (first_len | FirstFrag | DescOwn|TxCRC|eor);

	}
	cp->tx_hqhead = entry;
	if (netif_msg_tx_queued(cp))
		printk(KERN_DEBUG "%s: tx queued, slot %d, skblen %d\n",
		       dev->name, entry, skb->len);
	if (TX_HQBUFFS_AVAIL(cp) <= (MAX_SKB_FRAGS + 1))
		netif_stop_queue(dev);

	spin_unlock_irq(&cp->lock);
#ifndef DELAY_RX
	RTL_W32(IO_CMD, CMD_CONFIG | TX_POLL);
#else
	deq_loop++;
	goto dequeue_label;
#endif
	dev->trans_start = jiffies;

	return 0;
}

/* Set or clear the multicast filter for this adaptor.
   This routine is not state sensitive and need not be SMP locked. */

static void __rtl8186_set_rx_mode(struct net_device *dev)
{
	//struct re_private *cp = dev->priv;
	u8 mc_filter[8];	/* Multicast hash filter */
	int i, rx_mode;
	//u32 tmp;
#ifdef CONFIG_RTL8186_ETH_DEBUG
	printk("%s: %s %d Still Unimplemented !!!!!!!\n",__FILE__,__FUNCTION__,__LINE__);
#endif
	return;

	/* Note: do not reorder, GCC is clever about common statements. */
	if (dev->flags & IFF_PROMISC) {
		/* Unconditionally log net taps. */
		printk (KERN_NOTICE "%s: Promiscuous mode enabled.\n", dev->name);
		rx_mode = AcceptBroadcast | AcceptMulticast | AcceptMyPhys | AcceptAllPhys;
		for (i=0;i<8;i++)
			mc_filter[i] = 0xff;
	}
	else if ((dev->mc_count > multicast_filter_limit)
		|| (dev->flags & IFF_ALLMULTI)) {
		/* Too many to filter perfectly -- accept all multicasts. */
		rx_mode = AcceptBroadcast | AcceptMulticast | AcceptMyPhys;
		for (i=0;i<8;i++)
			mc_filter[i] = 0xff;
	}
	else {
		struct dev_mc_list *mclist;
		rx_mode = AcceptBroadcast | AcceptMyPhys;
		for (i=0;i<8;i++)
			mc_filter[i] = 0;
		for (i = 0, mclist = dev->mc_list; mclist && i < dev->mc_count;
			i++, mclist = mclist->next) {
		}
	}
}

static void rtl8186_set_rx_mode(struct net_device *dev)
{
	unsigned long flags;
	struct re_private *cp = dev->priv;

	spin_lock_irqsave (&cp->lock, flags);
	__rtl8186_set_rx_mode(dev);
	spin_unlock_irqrestore (&cp->lock, flags);
}

static void __rtl8186_get_stats(struct re_private *cp)
{
	/* XXX implement */
}

static struct net_device_stats *rtl8186_get_stats(struct net_device *dev)
{
	struct re_private *cp = dev->priv;

	/* The chip only need report frame silently dropped. */
	spin_lock_irq(&cp->lock);
 	if (netif_running(dev) && netif_device_present(dev))
 		__rtl8186_get_stats(cp);
	spin_unlock_irq(&cp->lock);

	return &cp->net_stats;
}

/*
 * Disable TX/RX through IO_CMD register
 */
static void rtl8186_stop_hw(struct net_device *dev, struct re_private *cp)
{
	RTL_W16(IMR, 0);
	RTL_W16(ISR, 0xffff);
	RTL_W32(IO_CMD, 0); /* timer  rx int 1 packets */
	synchronize_irq();
	udelay(10);
	cp->rx_tail = 0;
	cp->tx_hqhead = cp->tx_hqtail = 0;

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
	if (!memcmp(dev->name, "eth0", 4)) {
		int port;
		for (port=0; port<SWITCH_PORT_NUMBER; port++)
			disable_led_ctrl(port);		
	}
#endif	

}

static void rtl8186_reset_hw(struct re_private *cp)
{
	unsigned work = 1000;

   	RTL_W8(CMD, 0x1);	 /* Reset */
	while (work--) {
		if (!(RTL_R8(CMD) & 0x1))
			return;
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_timeout(10);
	}

	printk(KERN_ERR "%s: hardware reset timeout\n", cp->dev->name);
}

static inline void rtl8186_start_hw(struct re_private *cp)
{
	// set TX/RX setting/enable TX/RX
	RTL_W32(IO_CMD, CMD_CONFIG);
}

static void rtl8186_init_hw(struct re_private *cp)
{
	struct net_device *dev = cp->dev;
	u8 status;

	rtl8186_reset_hw(cp);
#ifdef RTL8186_CHECKSUM_OFFLOAD
	RTL_W8(CMD, 0x2);
#endif

#ifdef CONFIG_VLAN_QOS
	RTL_W8(CMD, RTL_R8(CMD)|0x4);
#endif

	RTL_W16(ISR, 0xffff);
	RTL_W16(IMR, rtl8186_intr_mask);
	RTL_W32(RxFDP, virt_to_bus((void *)(cp->rx_ring)));
#if DEBUG
	writecheck = RTL_R32(RxFDP);
	if (writecheck != ((u32)cp->rx_ring))
		for (;;);
#endif

	RTL_W16(RxCDO, 0);
	RTL_W32(TxFDP1, virt_to_bus((void *)(cp->tx_hqring)));
#if DEBUG
	writecheck = RTL_R32(TxFDP1);
	if (writecheck != ((u32)cp->tx_hqring))
		for (;;);
#endif

	RTL_W16(TxCDO1, 0);
	RTL_W32(TxFDP2, (u32)cp->tx_lqring);
	RTL_W16(TxCDO2, 0);
	RTL_W32(RCR, AcceptAll);
	RTL_W32(TCR, (u32)(0xC00));
	RTL_W8(RxRingSize, RINGSIZE);
	status = RTL_R8(MSR);
	status = status & ~(TXFCE | RXFCE);
	RTL_W8(MSR, status);
	mdelay(200);
	RTL_W32(MIIAR, BIT(26)|0x30000);				// read PHY type
	mdelay(200);
	if ((RTL_R32(MIIAR)&0x0000ffff) == 0x8201) {		// if PHY == 8201
		printk("%s:phy is 8201\n", dev->name);
		RTL_W32(MIIAR, BIT(31)|BIT(26)|0x3300);		// Reset/re-auto-nego
	}
	else {							// should be 8305
		printk("%s:phy is 8305\n", dev->name);
		if(dev->base_addr == 0xbd200000)
			RTL_W32(MIIAR, BIT(31)|BIT(30)|0x3300);	// LAN MII is open
		else
			RTL_W32(MIIAR, BIT(31)|BIT(28)|0x1300);	// WAN is port 4 (5th port)
	}
	mdelay(200);
	RTL_W8(IDR0, dev->dev_addr[0]);
	RTL_W8(IDR1, dev->dev_addr[1]);
	RTL_W8(IDR2, dev->dev_addr[2]);
	RTL_W8(IDR3, dev->dev_addr[3]);
	RTL_W8(IDR4, dev->dev_addr[4]);
	RTL_W8(IDR5, dev->dev_addr[5]);

	rtl8186_start_hw(cp);
	__rtl8186_set_rx_mode(dev);

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
	if (!memcmp(dev->name, "eth0", 4)) {
		int port;
		for (port=0; port<SWITCH_PORT_NUMBER; port++)
			init_led_ctrl(port);		
	}
#endif	
}

static int rtl8186_refill_rx(struct re_private *cp)
{
	unsigned i;

	for (i = 0; i < RTL8186_RX_RING_SIZE; i++) {
		struct sk_buff *skb;

		skb = dev_alloc_skb(cp->rx_buf_sz + RX_OFFSET);
		if (!skb)
			goto err_out;

		skb->dev = cp->dev;
		cp->rx_skb[i].skb = skb;
		cp->rx_skb[i].frag = 0;
		if ((u32)skb->data & 0x3)
			printk(KERN_DEBUG "skb->data unaligment %8x\n",(u32)skb->data);
		// set to noncache area
		cp->rx_ring[i].addr = (u32)skb->data|UNCACHE_MASK;
		if (i == (RTL8186_RX_RING_SIZE - 1))
			cp->rx_ring[i].opts1 = (DescOwn | RingEnd | cp->rx_buf_sz);
		else
			cp->rx_ring[i].opts1 =(DescOwn | cp->rx_buf_sz);
		cp->rx_ring[i].opts2 = 0;
	}

	return 0;

err_out:
	rtl8186_clean_rings(cp);
	return -ENOMEM;
}

static void rtl8186_tx_timeout(struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	unsigned tx_head = cp->tx_hqhead;
	unsigned tx_tail = cp->tx_hqtail;

	cp->cp_stats.tx_timeouts++;

	spin_lock_irq(&cp->lock);
	while (tx_tail != tx_head) {
		struct sk_buff *skb;
		u32 status;

		status = (cp->tx_hqring[tx_tail].opts1);
		if (status & DescOwn)
			break;
		skb = cp->tx_skb[tx_tail].skb;
		if (!skb)
			BUG();
		cp->net_stats.tx_packets++;
		cp->net_stats.tx_bytes += skb->len;		
		cp->tx_byte_cnt += skb->len;

		dev_kfree_skb(skb);
		cp->tx_skb[tx_tail].skb = NULL;
		tx_tail = NEXT_TX(tx_tail);
	}

	cp->tx_hqtail = tx_tail;

	spin_unlock_irq(&cp->lock);
	if (netif_queue_stopped(cp->dev))
		netif_wake_queue(cp->dev);

}

static int rtl8186_init_rings(struct re_private *cp)
{
	memset(cp->tx_hqring, 0, sizeof(DMA_DESC) * RTL8186_TX_RING_SIZE);
	memset(cp->rx_ring, 0, sizeof(DMA_DESC) * RTL8186_RX_RING_SIZE);
	cp->rx_tail = 0;
	cp->tx_hqhead = cp->tx_hqtail = 0;

	return rtl8186_refill_rx(cp);
}

static int rtl8186_alloc_rings(struct re_private *cp)
{
	void *pBuf;

	pBuf = kmalloc(RTL8186_RXRING_BYTES, GFP_KERNEL);
	if (!pBuf)
		goto ErrMem;

	cp->rxdesc_buf = pBuf;
	memset(pBuf, 0, RTL8186_RXRING_BYTES);

	pBuf = (void*)((u32)(pBuf + DESC_ALIGN-1) & ~(DESC_ALIGN -1));
	cp->rx_ring = (DMA_DESC*)((u32)(pBuf) | UNCACHE_MASK);

	pBuf= kmalloc(RTL8186_TXRING_BYTES, GFP_KERNEL);
	if (!pBuf)
		goto ErrMem;

	cp->txdesc_buf = pBuf;
	memset(pBuf, 0, RTL8186_TXRING_BYTES);
	pBuf = (void*)((u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1));
	cp->tx_hqring = (DMA_DESC*)((u32)(pBuf) | UNCACHE_MASK);

	return rtl8186_init_rings(cp);

ErrMem:
	if (cp->rxdesc_buf)
		kfree(cp->rxdesc_buf);
	if (cp->txdesc_buf)
		kfree(cp->txdesc_buf);
	return -ENOMEM;
}

static void rtl8186_clean_rings(struct re_private *cp)
{
	unsigned i;

	for (i = 0; i < RTL8186_RX_RING_SIZE; i++) {
		if (cp->rx_skb[i].skb) {
			dev_kfree_skb(cp->rx_skb[i].skb);
		}
	}

	for (i = 0; i < RTL8186_TX_RING_SIZE; i++) {
		if (cp->tx_skb[i].skb) {
			struct sk_buff *skb = cp->tx_skb[i].skb;
			dev_kfree_skb(skb);
			cp->net_stats.tx_dropped++;
		}
	}

	memset(&cp->rx_skb, 0, sizeof(struct ring_info) * RTL8186_RX_RING_SIZE);
	memset(&cp->tx_skb, 0, sizeof(struct ring_info) * RTL8186_TX_RING_SIZE);
}

static void rtl8186_free_rings(struct re_private *cp)
{
	rtl8186_clean_rings(cp);
	kfree(cp->rxdesc_buf);
	kfree(cp->txdesc_buf);

	cp->rx_ring = NULL;
	cp->tx_hqring = NULL;
}

#ifdef DYNAMIC_ADJUST_TASKLET
static void rtk8186_1sec_timer(unsigned long task_priv)
{
	struct re_private *cp = ((struct net_device *)task_priv)->priv;

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
	if (((struct net_device *)task_priv)->name[3] == '0' ) {
		int port;
		unsigned long flags;
		save_flags(flags);cli();

		for (port=0; port<SWITCH_PORT_NUMBER; port++) {
			update_mib_counter(port);		
			calculate_led_interval(port);
			if (led_cb[port].link_status & LINK_STATE_CHANGE) 
				LED_Interval_timeout(port);	
		}
		MII_read(4, 0, 1); // switch to MII to wan port to fix Eth auto link issue		

		restore_flags(flags);
	}	
#endif

	if (((struct net_device *)task_priv)->name[3] == '0' && rx_pkt_thres > 0) {	
		if (rx_cnt > rx_pkt_thres) {
			if (eth_flag & 1)  // tasklet disabled
				eth_flag &= ~1;							
		}				
		else {
			if ((eth_flag & 1) == 0) // tasklet enabled
				eth_flag |= 1;		
		}		
		rx_cnt = 0;		
	}	

	cp->tx_avarage = (cp->tx_avarage/10)*7 + (cp->tx_byte_cnt/10)*3;
	if (cp->tx_avarage > cp->tx_peak)
		cp->tx_peak = cp->tx_avarage;
	cp->tx_byte_cnt = 0;
	
	cp->rx_avarage = (cp->rx_avarage/10)*7 + (cp->rx_byte_cnt/10)*3;
	if (cp->rx_avarage > cp->rx_peak)
		cp->rx_peak = cp->rx_avarage;
	cp->rx_byte_cnt = 0;	
#endif

	mod_timer(&cp->expire_timer, jiffies + 100);
}

static int rtl8186_open(struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	int rc;

	if (netif_msg_ifup(cp))
		printk(KERN_DEBUG "%s: enabling interface\n", dev->name);

	rtl8186_set_rxbufsize(cp);	/* set new rx buf size */
	rc = rtl8186_alloc_rings(cp);
	if (rc)
		return rc;

#ifdef DELAY_RX
	tasklet_init(&cp->rx_tasklet, rtl8186_rx, (unsigned long)cp);
#endif

#ifdef CONFIG_RTL_IPSEC
	rc = request_irq(dev->irq, rtl8186_interrupt, SA_INTERRUPT | SA_SAMPLE_RANDOM, dev->name, dev);
#else
	rc = request_irq(dev->irq, rtl8186_interrupt, SA_INTERRUPT, dev->name, dev);
#endif
	if (rc)
		goto err_out_hw;

	// remember to enable IRQ before enabling TX/RX
	rtl8186_init_hw(cp);

	netif_start_queue(dev);

#ifdef DYNAMIC_ADJUST_TASKLET
	init_timer(&cp->expire_timer);
	cp->expire_timer.expires = jiffies + 100;
	cp->expire_timer.data = (unsigned long)dev;
	cp->expire_timer.function = rtk8186_1sec_timer;
	mod_timer(&cp->expire_timer, jiffies + 100);
	rx_cnt = 0;
#endif
	// power down and up port
	if((RTL_R32(MIIAR)&0x0000ffff) != 0x8201) { // if PHY == 8305
		if(dev->base_addr == 0xbd200000){ // LAN interface
			unsigned long flags;

			save_flags(flags);cli();
			cp->regs = (void *)0xbd300000;

			RTL_W32(MIIAR, BIT(31)|0x800|(0<<26));
			mdelay(10);
			RTL_W32(MIIAR, BIT(31)|0x800|(1<<26));
			mdelay(10);
			RTL_W32(MIIAR, BIT(31)|0x800|(2<<26));
			mdelay(10);
			RTL_W32(MIIAR, BIT(31)|0x800|(3<<26));

			mdelay(400);

			RTL_W32(MIIAR, BIT(31)|0x8000|(0<<26));
			mdelay(10);
			RTL_W32(MIIAR, BIT(31)|0x8000|(1<<26));
			mdelay(10);
			RTL_W32(MIIAR, BIT(31)|0x8000|(2<<26));
			mdelay(10);
			RTL_W32(MIIAR, BIT(31)|0x8000|(3<<26));

			cp->regs = (void *)dev->base_addr;
			restore_flags(flags);
		}
		else {
			RTL_W32(MIIAR, BIT(31)|0x800|(4<<26));
			mdelay(400);
			RTL_W32(MIIAR, BIT(31)|0x8000|(4<<26));
		}
	}
	else
		RTL_W32(MIIAR, BIT(31)|BIT(26)|0x3300);

#ifdef CONFIG_VLAN_QOS
	memset(cp->qosnode_table, 0, sizeof(cp->qosnode_table));
	memset(cp->qosnode_index, 0, sizeof(cp->qosnode_index));
#endif

	return 0;

err_out_hw:
	rtl8186_stop_hw(dev, cp);
	rtl8186_free_rings(cp);
	return rc;
}

static int rtl8186_close(struct net_device *dev)
{
	struct re_private *cp = dev->priv;

	if (netif_msg_ifdown(cp))
		printk(KERN_DEBUG "%s: disabling interface\n", dev->name);
#ifdef DELAY_RX
	tasklet_kill(&cp->rx_tasklet);
#endif
	netif_stop_queue(dev);
	rtl8186_stop_hw(dev, cp);
	free_irq(dev->irq, dev);
	rtl8186_free_rings(cp);

#ifdef DYNAMIC_ADJUST_TASKLET
	if (timer_pending(&cp->expire_timer))
		del_timer_sync(&cp->expire_timer);
#endif
	return 0;
}

static int rtl8186_change_mtu(struct net_device *dev, int new_mtu)
{
	struct re_private *cp = dev->priv;
	int rc;

	/* check for invalid MTU, according to hardware limits */
	if (new_mtu < CP_MIN_MTU || new_mtu > CP_MAX_MTU)
		return -EINVAL;

	/* if network interface not up, no need for complexity */
	if (!netif_running(dev)) {
		dev->mtu = new_mtu;
		rtl8186_set_rxbufsize(cp);	/* set new rx buf size */
		return 0;
	}

	spin_lock_irq(&cp->lock);

	rtl8186_stop_hw(dev, cp);			/* stop h/w and free rings */
	rtl8186_clean_rings(cp);

	dev->mtu = new_mtu;
	rtl8186_set_rxbufsize(cp);		/* set new rx buf size */

	rc = rtl8186_init_rings(cp);		/* realloc and restart h/w */
	rtl8186_start_hw(cp);
	spin_unlock_irq(&cp->lock);

	return rc;
}

#ifdef MII_EXTENTION
static char mii_2_8139_map[8] = {
	BasicModeCtrl,
	BasicModeStatus,
	0,
	0,
	NWayAdvert,
	NWayLPAR,
	NWayExpansion,
	0
};
static int mdio_read(struct net_device *dev, int phy_id, int location)
{
	struct re_private *cp = dev->priv;

	return location < 8 && mii_2_8139_map[location] ?
	       readw(cp->regs + mii_2_8139_map[location]) : 0;
}


static void mdio_write(struct net_device *dev, int phy_id, int location,
		       int value)
{
	struct re_private *cp = dev->priv;

	if (location == 0) {
		cpw8(Cfg9346, Cfg9346_Unlock);
		cpw16(BasicModeCtrl, value);
		cpw8(Cfg9346, Cfg9346_Lock);
	} else if (location < 8 && mii_2_8139_map[location])
		cpw16(mii_2_8139_map[location], value);
}
#endif

static int rtl8186_ethtool_ioctl (struct re_private *cp, void *useraddr)
{
	u32 ethcmd;

	/* dev_ioctl() in ../../net/core/dev.c has already checked
	   capable(CAP_NET_ADMIN), so don't bother with that here.  */

	if (get_user(ethcmd, (u32 *)useraddr))
		return -EFAULT;

	switch (ethcmd) {

	case ETHTOOL_GDRVINFO: {
		struct ethtool_drvinfo info = { ETHTOOL_GDRVINFO };
		strcpy (info.driver, DRV_NAME);
		strcpy (info.version, DRV_VERSION);
		strcpy (info.bus_info, "0");
		if (copy_to_user (useraddr, &info, sizeof (info)))
			return -EFAULT;
		return 0;
	}

	/* get settings */
	case ETHTOOL_GSET: {
		struct ethtool_cmd ecmd = { ETHTOOL_GSET };
		spin_lock_irq(&cp->lock);
		mii_ethtool_gset(&cp->mii_if, &ecmd);
		spin_unlock_irq(&cp->lock);
		if (copy_to_user(useraddr, &ecmd, sizeof(ecmd)))
			return -EFAULT;
		return 0;
	}
	/* set settings */
	case ETHTOOL_SSET: {
		int r;
		struct ethtool_cmd ecmd;
		if (copy_from_user(&ecmd, useraddr, sizeof(ecmd)))
			return -EFAULT;
		spin_lock_irq(&cp->lock);
		r = mii_ethtool_sset(&cp->mii_if, &ecmd);
		spin_unlock_irq(&cp->lock);
		return r;
	}
	/* restart autonegotiation */
	case ETHTOOL_NWAY_RST: {
		return mii_nway_restart(&cp->mii_if);
	}
	/* get link status */
	case ETHTOOL_GLINK: {
		struct ethtool_value edata = {ETHTOOL_GLINK};
		edata.data = mii_link_ok(&cp->mii_if);
		if (copy_to_user(useraddr, &edata, sizeof(edata)))
			return -EFAULT;
		return 0;
	}

	/* get message-level */
	case ETHTOOL_GMSGLVL: {
		struct ethtool_value edata = {ETHTOOL_GMSGLVL};
		edata.data = cp->msg_enable;
		if (copy_to_user(useraddr, &edata, sizeof(edata)))
			return -EFAULT;
		return 0;
	}
	/* set message-level */
	case ETHTOOL_SMSGLVL: {
		struct ethtool_value edata;
		if (copy_from_user(&edata, useraddr, sizeof(edata)))
			return -EFAULT;
		cp->msg_enable = edata.data;
		return 0;
	}

	default:
		break;
	}

	return -EOPNOTSUPP;
}

static int rtl8186_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct re_private *cp = dev->priv;
	int rc = 0;

	if (!netif_running(dev))
		return -EINVAL;

	switch (cmd) {
	case SIOCETHTOOL:
		return rtl8186_ethtool_ioctl(cp, (void *) rq->ifr_data);

    	default:
		rc = -EOPNOTSUPP;
		break;
	}

	return rc;
}

#if CP_VLAN_TAG_USED
static void cp_vlan_rx_register(struct net_device *dev, struct vlan_group *grp)
{
	struct re_private *cp = dev->priv;

	spin_lock_irq(&cp->lock);
	cp->vlgrp = grp;
	cp->cpcmd &= ~RxVlanOn; //enable vlan
	cpw16(CpCmd, cp->cpcmd);//enable vlan
	spin_unlock_irq(&cp->lock);
}

static void cp_vlan_rx_kill_vid(struct net_device *dev, unsigned short vid)
{
	struct re_private *cp = dev->priv;

	spin_lock_irq(&cp->lock);
	cp->cpcmd &= ~RxVlanOn; //enable vlan 
	cpw16(CpCmd, cp->cpcmd);//enable vlan
	if (cp->vlgrp)
		cp->vlgrp->vlan_devices[vid] = NULL;
	spin_unlock_irq(&cp->lock);
}
#endif

static int read_proc_stats(char *page, char **start, off_t off, int count, int *eof, void *data)
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

static int write_proc_stats(struct file *file, const char *buffer, unsigned long count, void *data)
{
	struct net_device *dev = (struct net_device *)data;
	struct re_private *cp = dev->priv;

	cp->tx_avarage = cp->tx_peak = cp->tx_byte_cnt = 0;
	cp->rx_avarage = cp->rx_peak = cp->rx_byte_cnt = 0;

	return count;
}

static int rtl8186_probe(int ethno)
{
#ifdef MODULE
	printk("%s", version);
#endif
	struct net_device *dev;
	struct re_private *cp;
	int rc;
	void *regs;
	unsigned i;
	struct proc_dir_entry *res;

#ifdef DYNAMIC_ADJUST_TASKLET
	struct proc_dir_entry *res1;
#endif	

	struct proc_dir_entry *res_stats;
	regs = (void *)((ethno)?0xbd300000:0xbd200000);

#ifndef MODULE
	static int version_printed;
	if (version_printed++ == 0)
		printk("%s", version);
#endif

	dev = alloc_etherdev(sizeof(struct re_private));
	if (!dev)
		return -ENOMEM;
	SET_MODULE_OWNER(dev);
	cp = dev->priv;
	reDev[ethno] = (struct re_private *)dev->priv;
	cp->dev = dev;
	spin_lock_init(&cp->lock);

#ifdef MII_EXTENTION
	cp->mii_if.dev = dev;
	cp->mii_if.mdio_read = mdio_read;
	cp->mii_if.mdio_write = mdio_write;
	cp->mii_if.phy_id = CP_INTERNAL_PHY;
#endif

	dev->base_addr = (unsigned long) regs;
	cp->regs = regs;

	rtl8186_stop_hw(dev, cp);

	/* Set Default MAC address */
	for (i = 0; i < 6; i++)
		((u8 *)(dev->dev_addr))[i] = i+(ethno<<2);

	dev->open		= rtl8186_open;
	dev->stop		= rtl8186_close;
	dev->set_multicast_list	= rtl8186_set_rx_mode;
	dev->hard_start_xmit	= rtl8186_start_xmit;
	dev->get_stats		= rtl8186_get_stats;
	dev->do_ioctl		= rtl8186_ioctl;
	dev->set_mac_address	= rtl8186_set_hwaddr;
	dev->change_mtu		= rtl8186_change_mtu;
	dev->tx_timeout		= rtl8186_tx_timeout;
	dev->watchdog_timeo	= TX_TIMEOUT;

#ifdef RTL8186_CHECKSUM_OFFLOAD
	dev->features	|= NETIF_F_SG | NETIF_F_IP_CSUM;
#endif

#if CP_VLAN_TAG_USED
	dev->features 	|= NETIF_F_HW_VLAN_TX | NETIF_F_HW_VLAN_RX;
	dev->vlan_rx_register	= cp_vlan_rx_register;
	dev->vlan_rx_kill_vid	= cp_vlan_rx_kill_vid;
#endif
	dev->irq				= (ethno)?5:4;

	rc = register_netdev(dev);
	if (rc)
		goto err_out_iomap;

	printk(KERN_INFO "%s: %s at 0x%lx, "
		"%02x:%02x:%02x:%02x:%02x:%02x, "
		"IRQ %d\n",
		dev->name,
		"RTL8186-NIC",
		dev->base_addr,
		dev->dev_addr[0], dev->dev_addr[1],
		dev->dev_addr[2], dev->dev_addr[3],
		dev->dev_addr[4], dev->dev_addr[5],
		dev->irq);
	
	/* create /proc/ethX */
	root_ethX_dir = proc_mkdir(dev->name, 0);
	res = create_proc_entry("eth_flag", 0, root_ethX_dir);
	
	if (res)
		res->write_proc = write_proc;

#ifdef DYNAMIC_ADJUST_TASKLET
	res1 = create_proc_entry("rx_pkt_thres", 0, root_ethX_dir);
	if (res1)
		res1->write_proc = write_proc_rxthres;
#endif		

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
	{  
		struct proc_dir_entry *res2;
		res2 = create_proc_entry("mii", 0, root_ethX_dir);
		if (res2) {
			res2->write_proc = write_proc_mii;
			res2->read_proc = read_proc_mii;
		}
		struct proc_dir_entry *res3;
		res3 = create_proc_entry("eth_mib", 0, root_ethX_dir);
		if (res3) 
			res3->read_proc = read_proc_mib;	
	}
#endif // CONFIG_PATCH_8306_CTRL_LED_BY_CPU

	if ((res_stats = create_proc_read_entry("stats", 0644, root_ethX_dir,
				read_proc_stats, (void *)dev)) == NULL) {
		printk("create_proc_read_entry failed!\n");
		goto err_out_iomap;
	}
	res_stats->write_proc = write_proc_stats;

	/*
	 * Looks like this is necessary to deal with on all architectures,
	 * even this %$#%$# N440BX Intel based thing doesn't get it right.
	 * Ie. having two NICs in the machine, one will have the cache
	 * line set at boot time, the other will not.
	 */
	return 0;

err_out_iomap:
	printk("in err_out_iomap\n");
	iounmap(regs);
	kfree(dev);
	return -1 ;
}

#ifdef PATCH_8306_SW
#define ETHBASE	0xBD300000   //Brad modify 2007-08-30
#define rtl8305_inl(offset)			(*(volatile unsigned long *)(ETHBASE+offset))
#define rtl8305_outl(offset,val)	(*(volatile unsigned long *)(ETHBASE+offset) = val)
#define RTL8306_S 0
#define RTL8306_SD 1
#define RTL8306_SDM 2
#define RTL8306_CHIPID 0X5988
#define RTL8306_VERNUM 0X0

typedef struct asicVersionPara_s
{
    unsigned short chipid;
    unsigned char vernum;
    unsigned char series;
    unsigned char revision;
} asicVersionPara_t;

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
static void MII_write(unsigned short int phyaddr, unsigned short int regaddr, unsigned short int data, unsigned char eth)
#else
void __init MII_write(unsigned short int phyaddr, unsigned short int regaddr, unsigned short int data, unsigned char eth)
#endif
{
	unsigned int phy_addr_data, MII_reg;
	unsigned long flags;

	save_flags(flags); cli();	
	
	phy_addr_data = (1<<31) | (phyaddr<<26) | (regaddr<<16) | data;
	
	if (eth == 0)
		MII_reg = 0x005c;
	else
		MII_reg = 0x005c;
		
	rtl8305_outl(MII_reg, phy_addr_data);
	while (1) {	
		udelay(300);
		if ((rtl8305_inl(MII_reg) & 0x80000000) == 0) {
#ifdef CONFIG_SWITCH_DEBUG_MODE			
			printk("write phy%d reg%d =0x%x\n",phyaddr, regaddr,rtl8305_inl(MII_reg));
#endif
			break;
		}
	}		
	restore_flags(flags);			
	return;
}

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
static unsigned short int MII_read(unsigned short int phyaddr, unsigned short int regaddr, unsigned char eth)
#else
unsigned short int __init MII_read(unsigned short int phyaddr, unsigned short int regaddr, unsigned char eth)
#endif
{
	unsigned int phy_addr, MII_reg;
	unsigned long flags;

	save_flags(flags); cli();
	
	phy_addr = (0<<31) | (phyaddr<<26) | (regaddr<<16);
	
	if (eth == 0)
		MII_reg = 0x005c;
	else
		MII_reg = 0x005c;
		
	rtl8305_outl(MII_reg, phy_addr);
	while (1) {	
		udelay(300);
		if ((rtl8305_inl(MII_reg) & 0x80000000)) {
#ifdef CONFIG_SWITCH_DEBUG_MODE			
			printk("read phy%d reg%d =0x%x\n",phyaddr, regaddr,rtl8305_inl(MII_reg));
#endif			
			break;
		}
	}		
	restore_flags(flags);			
	return rtl8305_inl(MII_reg)&0x0000ffff;
}

int __init rtl8306_getAsicVersionInfo(asicVersionPara_t *pAsicVer)
{
    unsigned int regval;
    unsigned int temp=0;	
    /*get chip id*/
   // rtl8306_getAsicPhyReg(4, 30, 0, &regval );
    regval = MII_read(4,30,0);
    pAsicVer->chipid = (unsigned short)regval;
    /*get version number*/
   // rtl8306_getAsicPhyReg(4, 31, 0, &regval);
     regval = MII_read(4,31,0);
    
    pAsicVer->vernum = (unsigned char)(regval & 0xFF);
    /* bit[8:9]*/
    //rtl8306_getAsicPhyReg(4, 31, 0, &regval);
     regval = MII_read(4,31,0);
    regval = (regval & 0x300) >> 8;
    if (regval  == 0 || regval ==2){
        pAsicVer->series = RTL8306_S;
    }else if (regval == 1){
        pAsicVer->series = RTL8306_SD;
    }
    else if (regval == 3){
        pAsicVer->series = RTL8306_SDM;
    }else 
        pAsicVer->series = 0xFF;
     /* Set reg 16, bit 11 is 1*/   
    //rtl8306_setAsicPhyRegBit(0, 16, 11, 0, 1);    
    temp   = MII_read(0,16,0);
    temp = temp | 0x800;  
    MII_write(0, 16, temp, 0);       
    mdelay(40);                                           //Ella add
    /* get phy 4 , reg 26 value to regval */
    //rtl8306_getAsicPhyReg(4, 26, 0, &regval); 
     regval = MII_read(4,26,0);
    pAsicVer->revision = (regval & 0xE000) >> 13;
     /* Set reg 16, bit 11 is 0*/   
      //rtl8306_setAsicPhyRegBit(0, 16, 11, 0, 0);
      temp = MII_read(0,16,0);
      temp = temp & ~0x800;
       MII_write(0, 16, temp, 0);       
    mdelay(40);                                           //Ella add
    return 0;
}


#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
int32 rtl8306_getAsicPhyReg(uint32 phyad, uint32 regad, uint32 npage, uint32 *pvalue) 
{
	rtl8306_page_select(npage);	
	MII_read(phyad, regad, 0);
	*pvalue = MII_read(phyad, regad, 0);
	return SUCCESS;		
}

int32 rtl8306_setAsicPhyReg(uint32 phyad, uint32 regad, uint32 npage, uint32 value) 
{
	/* Select PHY Register Page through configuring PHY 0 Register 16 [bit1 bit15] */
	value = value & 0xFFFF;
	rtl8306_page_select(npage);
	MII_write(phyad, regad, value, 0);
	return SUCCESS;
}

int rtl8306_setAsicPhyRegBit(uint32 phyad, uint32 regad, uint32 bit, uint32 npage,  uint32 value) {
	uint32 rdata;
	rtl8306_getAsicPhyReg(phyad, regad,  npage, &rdata);
	if (value) 
		rtl8306_setAsicPhyReg(phyad, regad, npage, rdata | (1 << bit));
	else
		rtl8306_setAsicPhyReg(phyad, regad, npage, rdata & (~(1 << bit)));
	return SUCCESS;
}

int rtl8306_setAsicMibCounterReset(uint32 port, uint32 operation) 
{	
	switch(operation) {	
		case RTL8306_MIB_RESET:		
			rtl8306_setAsicPhyRegBit(port, 17, 2, 2, 1);		
			break;	
		case RTL8306_MIB_START:		
			rtl8306_setAsicPhyRegBit(port, 17, 2, 2, 0);		
			break;			
		default :		
			return FAILED;
	}		
	return SUCCESS;
}

int32 rtl8306_getAsicPhyRegBit(uint32 phyad, uint32 regad, uint32 bit, uint32 npage,  uint32 * pvalue) 
{
	uint32 rdata;

	rtl8306_getAsicPhyReg(phyad, regad, npage, &rdata);
	if (rdata & (1 << bit))
		*pvalue =1;
	else 
		*pvalue =0;
		
	return SUCCESS;
}

int32 rtl8306_getAsicMibCounterUnit(uint32 port, uint32 counter, uint32 *unit) 
{ 
	uint32 bitValue;
	
	switch(counter) {
	case RTL8306_MIB_CNT1:
		rtl8306_getAsicPhyRegBit(port, 17, 3, 2, &bitValue);
		break;
	case RTL8306_MIB_CNT2:
		rtl8306_getAsicPhyRegBit(port, 17, 4, 2, &bitValue);
		break;
	default:
		return FAILED;
	}
	*unit = (bitValue == 1 ? RTL8306_MIB_PKT : RTL8306_MIB_BYTE);
	return SUCCESS;
}

static int32 rtl8306_getAsicMibCounter(uint32 port, uint32 counter, uint32 *value) 
{
	uint32 regValue1, regValue2;

	switch(counter) {
	case RTL8306_MIB_CNT1:
		/*Must read low 16 bit first, then hight 16 bit*/
		rtl8306_getAsicPhyReg(port, 22, 2, &regValue1);
		rtl8306_getAsicPhyReg(port, 23, 2, &regValue2);
		*value = (regValue2 << 16) + regValue1;
		break;
	case RTL8306_MIB_CNT2:
		/*Must read low 16 bit first, then hight 16 bit*/
		rtl8306_getAsicPhyReg(port, 24, 2, &regValue1);
		rtl8306_getAsicPhyReg(port, 25, 2, &regValue2);
		*value = (regValue2 << 16) + regValue1;		
		break;
	case RTL8306_MIB_CNT3:
		/*Must read low 16 bit first, then hight 16 bit*/
		rtl8306_getAsicPhyReg(port, 26, 2, &regValue1);
		rtl8306_getAsicPhyReg(port, 27, 2, &regValue2);
		*value = (regValue2 << 16) + regValue1;		
		break;
	case RTL8306_MIB_CNT4:
		/*Must read low 16 bit first, then hight 16 bit*/
		rtl8306_getAsicPhyReg(port, 28, 2, &regValue1);
		rtl8306_getAsicPhyReg(port, 29, 2, &regValue2);
		*value = (regValue2 << 16) + regValue1;		
		break;
	case RTL8306_MIB_CNT5:
		/*Must read low 16 bit first, then hight 16 bit*/
		rtl8306_getAsicPhyReg(port, 30, 2, &regValue1);
		rtl8306_getAsicPhyReg(port, 31, 2, &regValue2);
		*value = (regValue2 << 16) + regValue1;		
		break;
	default:
		return FAILED;		
	}	
	return SUCCESS;
}
#endif // CONFIG_PATCH_8306_CTRL_LED_BY_CPU
#endif // PATCH_8306_SW

static void __exit rtl8186_exit (void)
{
}

static int __init rtl8186_init(void)
{
#ifdef PATCH_8306_SW
	asicVersionPara_t AsicVer;
#endif
	unsigned int temp=0;
	rtl8186_probe(0);
	rtl8186_probe(1);

#ifdef CONFIG_BICOLOR_LED
	{
		unsigned int val = (19<<16);
		writel(val, NIC1BASE|MIIAR);
		val = readl(NIC1BASE|MIIAR);
		val &= ~0x3400;
		val |= 0x800023ff;
		writel(val, NIC1BASE|MIIAR);
	}
#endif

#ifdef PATCH_8306_SW
	 rtl8306_getAsicVersionInfo(&AsicVer); //brad
    if ((AsicVer.chipid == RTL8306_CHIPID) && 
        (AsicVer.vernum == RTL8306_VERNUM) && 
        (AsicVer.revision == 0x0)  )
    {
#ifdef CONFIG_RTL8186_ETH_DEBUG
       printk("Will Set phy 2, reg 26 ==0x56\n");
#endif
        MII_write(2, 26, 0x0056,0);
    }
	mdelay(40);                                  //brad add 
 	temp   = MII_read(0,16,0);
    	temp = temp | 0x800;  
    	MII_write(0, 16, temp, 0); 
	printk("Check MII:phy 2, reg 31\n");
	printk("0x%08x\n", MII_read(2,31,0));
	printk("Check MII:phy 2, reg 26\n");
	printk("0x%08x\n", MII_read(2,26,0));
	printk("Asic.revision=0x%08x\n",AsicVer.revision);   //brad add
	
#ifdef CONFIG_RTL8186_ETH_DEBUG
	printk("Check MII:phy 0, reg 23\n");
	printk("0x%08x\n", MII_read(0,23,0));
#endif
	temp = MII_read(0,16,0);
      	temp = temp & ~0x800;
       MII_write(0, 16, temp, 0);  
#endif /* PATCH_8306_SW */

#ifdef CONFIG_PATCH_8306_CTRL_LED_BY_CPU
	{
		int i;
		rtl8306_page_select(3);
		MII_write(2, 21, MII_read(2 ,21, 0)|0x780, 0); 

		for (i=0;i<5; i++)
			rtl8306_setAsicMibCounterReset(i, RTL8306_MIB_RESET);

		for (i=0;i<5; i++)
 			rtl8306_setAsicMibCounterReset(i, RTL8306_MIB_START);
	}
#endif // CONFIG_PATCH_8306_CTRL_LED_BY_CPU

	return 0;
}

static int rtl8186_set_hwaddr(struct net_device *dev, void *addr)
{
	unsigned long flags;
	int i;
	struct re_private *cp = (struct re_private *) dev->priv;
	unsigned char *p;

	p = ((struct sockaddr *)addr)->sa_data;

	save_flags(flags); cli();
	for (i = 0; i < 6; ++i) {
		dev->dev_addr[i] = p[i];
#ifdef CONFIG_RTL8186_ETH_DEBUG
		printk("setup %s hw_address %d : %X\n", dev->name, i, p[i]);
#endif
	}
	RTL_W8(IDR0, dev->dev_addr[0]);
	RTL_W8(IDR1, dev->dev_addr[1]);
	RTL_W8(IDR2, dev->dev_addr[2]);
	RTL_W8(IDR3, dev->dev_addr[3]);
	RTL_W8(IDR4, dev->dev_addr[4]);
	RTL_W8(IDR5, dev->dev_addr[5]);
	restore_flags(flags);
	return 0;
}

module_init(rtl8186_init);
module_exit(rtl8186_exit);
