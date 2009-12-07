#ifndef	__FASTPATH_CORE_H__
#define	__FASTPATH_CORE_H__

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <net/dst.h>
#include <net/route.h>
#include "./rtl_queue.h"			/* X-Queue Marco Function */
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>

/*
	Virtual Rome Driver API & System (Light Rome Driver Simulator)
*/
#define	__IRAM_GEN	__attribute__	((section(".iram-gen")))
#define	ip_t		__u32
/* ---------------------------------------------------------------------------------------------------- */

#define	IFNAME_LEN_MAX		16
#define	MAC_ADDR_LEN_MAX	18
#define	ARP_TABLE_LIST_MAX	32
#define	ARP_TABLE_ENTRY_MAX	128
#define	ROUTE_TABLE_LIST_MAX	16
#define	ROUTE_TABLE_ENTRY_MAX	64
#define	NAPT_TABLE_LIST_MAX	32
#define	NAPT_TABLE_ENTRY_MAX	32
#define	PATH_TABLE_LIST_MAX	32
#define	PATH_TABLE_ENTRY_MAX NAPT_TABLE_ENTRY_MAX	//(NAPT_TABLE_ENTRY_MAX * 2)  //cathy
#define	INTERFACE_ENTRY_MAX	8

#define	ETHER_ADDR_LEN		6
typedef struct ether_s {
        __u8 octet[ETHER_ADDR_LEN];
} ether_t;

/* ########### API #################################################################################### */
enum LR_RESULT
{
	/* Common error code */
	LR_SUCCESS = 0,						/* Function Success */
	LR_FAILED = -1,						/* General Failure, not recommended to use */
	LR_ERROR_PARAMETER = -2,				/* The given parameter error */
	LR_EXIST = -3,							/* The entry you want to add has been existed, add failed */
	LR_NONEXIST = -4,						/* The specified entry is not found */
	
	LR_NOBUFFER = -1000,					/* Out of Entry Space */
	LR_INVAPARAM = -1001,					/* Invalid parameters */
	LR_NOTFOUND = -1002,					/* Entry not found */
	LR_DUPENTRY = -1003,					/* Duplicate entry found */
};


enum ARP_FLAGS
{
	ARP_NONE = 0,
};

enum RT_FLAGS
{
	RT_NONE = 0,
};

enum SE_TYPE
{
	SE_PPPOE = 1,
	SE_PPTP = 2,
	SE_L2TP = 3,
};
enum SE_FLAGS
{
	SE_NONE = 0,
};

enum NP_FLAGS
{
	NP_NONE = 0,
};

/* ---------------------------------------------------------------------------------------------------- */
enum LR_RESULT rtl865x_addArp( ip_t ip, ether_t* mac, enum ARP_FLAGS flags );
enum LR_RESULT rtl865x_modifyArp( ip_t ip, ether_t* mac, enum ARP_FLAGS flags );
enum LR_RESULT rtl865x_delArp( ip_t ip );
enum LR_RESULT rtl865x_addRoute( ip_t ip, ip_t mask, ip_t gateway, __u8* ifname, enum RT_FLAGS flags, int type);
enum LR_RESULT rtl865x_modifyRoute( ip_t ip, ip_t mask, ip_t gateway, __u8* ifname, enum RT_FLAGS flags, int type );
enum LR_RESULT rtl865x_delRoute( ip_t ip, ip_t mask );
enum LR_RESULT rtl865x_addSession( __u8* ifname, enum SE_TYPE seType, __u32 sessionId, enum SE_FLAGS flags );
enum LR_RESULT rtl865x_delSession( __u8* ifname );

//andrew
void fastpath_notify(int event);

//cathy
enum LR_RESULT rtl865x_addNaptConnection(struct ip_conntrack_tuple ori_tuple,
		struct ip_conntrack_tuple reply_tuple, enum NP_FLAGS flags);

enum LR_RESULT rtl865x_delNaptConnection (struct ip_conntrack_tuple ori_tuple,
		struct ip_conntrack_tuple reply_tuple);


/* [MARCO FUNCTION] ========================================================================= */
#define	MAC2STR(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3], \
	((unsigned char *)&addr)[4], \
	((unsigned char *)&addr)[5]

#define	FASTPATH_MAC2STR(mac, hbuffer) \
	do { \
		int j,k; \
		const char hexbuf[] =  "0123456789ABCDEF"; \
		for (k=0,j=0;k<MAC_ADDR_LEN_MAX && j<6;j++) { \
			hbuffer[k++]=hexbuf[(mac->octet[j]>>4)&15 ]; \
			hbuffer[k++]=hexbuf[mac->octet[j]&15     ]; \
			hbuffer[k++]=':'; \
		} \
		hbuffer[--k]=0; \
	} while(0)	/* Mac Address to String */

#define FASTPATH_ADJUST_CHKSUM_NAT_UDP(ip_mod, ip_org, chksum) \
	do { \
		s32 accumulate = 0; \
		if (chksum == 0) break; \
		if (((ip_mod) != 0) && ((ip_org) != 0)){ \
			accumulate = ((ip_org) & 0xffff); \
			accumulate += (( (ip_org) >> 16 ) & 0xffff); \
			accumulate -= ((ip_mod) & 0xffff); \
			accumulate -= (( (ip_mod) >> 16 ) & 0xffff); \
		} \
		accumulate += ntohs(chksum); \
		if (accumulate < 0) { \
			accumulate = -accumulate; \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) ~accumulate); \
		} else { \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) accumulate); \
		} \
	}while(0)	/* Checksum adjustment */

#define FASTPATH_ADJUST_CHKSUM_NPT_UDP(port_mod, port_org, chksum) \
	do { \
		s32 accumulate = 0; \
		if (chksum == 0) break; \
		if (((port_mod) != 0) && ((port_org) != 0)){ \
			accumulate += (port_org); \
			accumulate -= (port_mod); \
		} \
		accumulate += ntohs(chksum); \
		if (accumulate < 0) { \
			accumulate = -accumulate; \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) ~accumulate); \
		} else { \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) accumulate); \
		} \
	}while(0)	/* Checksum adjustment */


#define FASTPATH_ADJUST_CHKSUM_NAPT_UDP(ip_mod, ip_org, port_mod, port_org, chksum) \
	do { \
		s32 accumulate = 0; \
		if (chksum == 0) break; \
		if (((ip_mod) != 0) && ((ip_org) != 0)){ \
			accumulate = ((ip_org) & 0xffff); \
			accumulate += (( (ip_org) >> 16 ) & 0xffff); \
			accumulate -= ((ip_mod) & 0xffff); \
			accumulate -= (( (ip_mod) >> 16 ) & 0xffff); \
		} \
		if (((port_mod) != 0) && ((port_org) != 0)){ \
			accumulate += (port_org); \
			accumulate -= (port_mod); \
		} \
		accumulate += ntohs(chksum); \
		if (accumulate < 0) { \
			accumulate = -accumulate; \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) ~accumulate); \
		} else { \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) accumulate); \
		} \
	}while(0)	/* Checksum adjustment */

#define FASTPATH_ADJUST_CHKSUM_NAT(ip_mod, ip_org, chksum) \
	do { \
		s32 accumulate = 0; \
		if (((ip_mod) != 0) && ((ip_org) != 0)){ \
			accumulate = ((ip_org) & 0xffff); \
			accumulate += (( (ip_org) >> 16 ) & 0xffff); \
			accumulate -= ((ip_mod) & 0xffff); \
			accumulate -= (( (ip_mod) >> 16 ) & 0xffff); \
		} \
		accumulate += ntohs(chksum); \
		if (accumulate < 0) { \
			accumulate = -accumulate; \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) ~accumulate); \
		} else { \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) accumulate); \
		} \
	}while(0)	/* Checksum adjustment */

#define FASTPATH_ADJUST_CHKSUM_NPT(port_mod, port_org, chksum) \
	do { \
		s32 accumulate = 0; \
		if (((port_mod) != 0) && ((port_org) != 0)){ \
			accumulate += (port_org); \
			accumulate -= (port_mod); \
		} \
		accumulate += ntohs(chksum); \
		if (accumulate < 0) { \
			accumulate = -accumulate; \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) ~accumulate); \
		} else { \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) accumulate); \
		} \
	}while(0)	/* Checksum adjustment */


#define FASTPATH_ADJUST_CHKSUM_NAPT(ip_mod, ip_org, port_mod, port_org, chksum) \
	do { \
		s32 accumulate = 0; \
		if (((ip_mod) != 0) && ((ip_org) != 0)){ \
			accumulate = ((ip_org) & 0xffff); \
			accumulate += (( (ip_org) >> 16 ) & 0xffff); \
			accumulate -= ((ip_mod) & 0xffff); \
			accumulate -= (( (ip_mod) >> 16 ) & 0xffff); \
		} \
		if (((port_mod) != 0) && ((port_org) != 0)){ \
			accumulate += (port_org); \
			accumulate -= (port_mod); \
		} \
		accumulate += ntohs(chksum); \
		if (accumulate < 0) { \
			accumulate = -accumulate; \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) ~accumulate); \
		} else { \
			accumulate = (accumulate >> 16) + (accumulate & 0xffff); \
			accumulate += accumulate >> 16; \
			chksum = htons((__u16) accumulate); \
		} \
	}while(0)	/* Checksum adjustment */

/* ---------------------------------------------------------------------------------------------------- */
__u8 *FastPath_Route(ip_t dIp);
int FastPath_Enter(struct sk_buff *skb);
int FastPath_Track(struct sk_buff *skb);
/* ---------------------------------------------------------------------------------------------------- */

struct Arp_List_Entry
{
	__u8 vaild;
	ip_t ip;
	ether_t mac;
	enum ARP_FLAGS flags;
	CTAILQ_ENTRY(Arp_List_Entry) arp_link;
	CTAILQ_ENTRY(Arp_List_Entry) tqe_link;
};

struct Route_List_Entry
{
	__u8 vaild;
	ip_t ip;
	ip_t mask;
	ip_t gateway;
	__u8 ifname[IFNAME_LEN_MAX];
	enum RT_FLAGS flags;
	CTAILQ_ENTRY(Route_List_Entry) route_link;
	CTAILQ_ENTRY(Route_List_Entry) tqe_link;
};

extern int fp_on;
static inline int FastPath_Enabled(void) {
//	return 1;
	return fp_on;
}
#ifdef CONFIG_PPPOE_PROXY
extern int pppoe_proxy_enabled;
extern void  GetPPPoeInfo(struct sk_buff *skb,int *session, char **remote);
extern int findmacType(char *mac, int  *type);
extern int  pppoe_xmit2(struct sk_buff *skb,char *macaddr,int session ,int type);
#endif
#endif	/* __FASTPATH_CORE_H__ */

