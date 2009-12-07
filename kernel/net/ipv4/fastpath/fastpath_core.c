/*
Linux Kernel Hacking:
	net/core/neighbour.c				// ARP
	net/ipv4/fib_hash.c				// ROUTE
	net/ipv4/netfilter/ip_conntrack_core.c		// NAPT (PATH*2)
	net/ipv4/netfilter/ip_nat_core.c		// NAPT (PATH*2)
	net/ipv4/ip_input.c				// FastPath_Enter()
	net/ipv4/ip_output.c				// FastPath_Track()
*/

#include "fastpath_core.h"
#define	MODULE_NAME		"UNIVERSAL_FAST_PATCH"
#define	MODULE_VERSION_FP	"v.0.0.1"
#define __SWAP

//andrew
extern struct neigh_table arp_tbl; 
enum {
	ST_INIT =  0,
	ST_OPERATIONAL, 
	ST_ADMIN_DOWN
};

enum {
	EVT_SAR_UP = 1,
	EVT_SAR_DOWN,
	EVT_ADMIN_DOWN,
 	EVT_ADMIN_UP
};

int fp_on=1;

#define DEBUGP_API(format, args...)

#define DEBUGP_PKT(format, args...)

#define DEBUGP_SYS(format, args...)

#define	DEBUG_PROCFILE	/* Create ProcFile for debug */

/* ==================================================================================================== */
static __u8 fastpath_forward_flag = 1;		/* default: On */

/* --- ARP Table Structures --- */
struct Arp_Table
{
	CTAILQ_HEAD(Arp_list_entry_head, Arp_List_Entry) list[ARP_TABLE_LIST_MAX];
};

CTAILQ_HEAD(Arp_list_inuse_head, Arp_List_Entry) arp_list_inuse;
CTAILQ_HEAD(Arp_list_free_head, Arp_List_Entry) arp_list_free;

struct Arp_Table *table_arp;

/* --- Route Table Structures --- */
struct Route_Table
{
	CTAILQ_HEAD(Route_list_entry_head, Route_List_Entry) list[ROUTE_TABLE_LIST_MAX];
};

CTAILQ_HEAD(Route_list_inuse_head, Route_List_Entry) route_list_inuse;
CTAILQ_HEAD(Route_list_free_head, Route_List_Entry) route_list_free;

struct Route_Table *table_route;

/* --- NAPT Table Structures --- */
struct Napt_List_Entry
{
	__u8 vaild;
	//cathy
        //enum NP_PROTOCOL protocol;
	__u16 protocol;
	ip_t intIp;
	__u32 intPort;
	ip_t extIp;
	__u32 extPort;
	ip_t remIp;
	__u32 remPort;
	enum NP_FLAGS flags;
	CTAILQ_ENTRY(Napt_List_Entry) napt_link;
	CTAILQ_ENTRY(Napt_List_Entry) tqe_link;
};

struct Napt_Table
{
	CTAILQ_HEAD(Napt_list_entry_head, Napt_List_Entry) list[NAPT_TABLE_LIST_MAX];
};

CTAILQ_HEAD(Napt_list_inuse_head, Napt_List_Entry) napt_list_inuse;
CTAILQ_HEAD(Napt_list_free_head, Napt_List_Entry) napt_list_free;

struct Napt_Table *table_napt;

/* --- PATH Table Structures --- */
struct Path_List_Entry
{
	__u8			vaild;
	//cathy
	__u16			*protocol;
	ip_t		*in_sIp;
	__u32			*in_sPort;
	ip_t		*in_dIp;
	__u32			*in_dPort;
	ip_t		*out_sIp;
	__u32			*out_sPort;
	ip_t		*out_dIp;
	__u32			*out_dPort;
	__u8			*out_ifname;
	struct Arp_List_Entry	*arp_entry;		/* for Out-dMac */
	__u8			course;					/* 1:In-Bonud 2:Out-Bound */
	struct dst_entry *dst;
	__u8			type;
	CTAILQ_ENTRY(Path_List_Entry) path_link;
	CTAILQ_ENTRY(Path_List_Entry) tqe_link;
};

struct Path_Table
{
	CTAILQ_HEAD(Path_list_entry_head, Path_List_Entry) list[PATH_TABLE_LIST_MAX];
};

CTAILQ_HEAD(Path_list_inuse_head, Path_List_Entry) path_list_inuse;
CTAILQ_HEAD(Path_list_free_head, Path_List_Entry) path_list_free;

struct Path_Table *table_path;


/* ==================================================================================================== */

static __u32 __SWAP FastPath_Hash_ARP_Entry(ip_t ip)
{
	return (ip % 16);
}

static __u32 __SWAP 
FastPath_Hash_ROUTE_Entry(ip_t ip, ip_t mask)
{
	int i;
	ip_t tmp = (ip & mask);
	
	for(i=0; i<32; i++) {
		if (tmp & 0x00000001) {
			return (tmp + (__u32)i) % ROUTE_TABLE_LIST_MAX;
		}
		tmp = tmp >> 1;
	}
	
	return 0;
}

static __u32 __SWAP 
FastPath_Hash_NAPT_Entry(ip_t intIp,__u32 intPort,
			ip_t extIp, __u32 extPort,
			ip_t remIp, __u32 remPort)
{
	__u32 hash;

	hash = (0xff000000 & intIp) >> 24;
	hash ^= (0x00ff0000 & intIp) >> 16;
	hash ^= (0x0000ff00 & intIp) >> 8;
	hash ^= (0x000000ff & intIp);
	hash ^= (0x0000ff00 & intPort) >> 8;
	hash ^= (0x000000ff & intPort);
	
	hash ^= (0xff000000 & extIp) >> 24;
	hash ^= (0x00ff0000 & extIp) >> 16;
	hash ^= (0x0000ff00 & extIp) >> 8;
	hash ^= (0x000000ff & extIp);
	hash ^= (0x0000ff00 & extPort) >> 8;
	hash ^= (0x000000ff & extPort);

	hash ^= (0xff000000 & remIp) >> 24;
	hash ^= (0x00ff0000 & remIp) >> 16;
	hash ^= (0x0000ff00 & remIp) >> 8;
	hash ^= (0x000000ff & remIp);
	hash ^= (0x0000ff00 & remPort) >> 8;
	hash ^= (0x000000ff & remPort);
	
	// Kaohj
	//return 0x000003ff & (hash ^ (hash >> 12));
	return (NAPT_TABLE_LIST_MAX-1) & (hash ^ (hash >> 12));
}

__IRAM_GEN inline static __u32
FastPath_Hash_PATH_Entry(ip_t sip, __u32 sport, ip_t dip, __u32 dport, __u16 proto)
{
	register __u32 hash;
	
	hash = ((sip>>16)^sip);
	hash ^= ((dip>>16)^dip);
	hash ^= sport;
	hash ^= dport;
//cathy
	hash ^= proto;
	// Kaohj
	//return 0x000003ff & (hash ^ (hash >> 12));
	return (PATH_TABLE_LIST_MAX-1) & (hash ^ (hash >> 12));
}


/* ==================================================================================================== */


enum LR_RESULT __SWAP 
rtl865x_addArp(ip_t ip,
		ether_t* mac,
		enum ARP_FLAGS flags)
{
	__u32 hash = FastPath_Hash_ARP_Entry(ip);
	struct Arp_List_Entry *ep;
	
	DEBUGP_API("addArp: ip=0x%08X mac=%02X:%02X:%02X:%02X:%02X:%02X flags=0x%08X Hash=%u \n", ip, MAC2STR(*mac), flags, hash);
	
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
		if (ep->ip == ip) {
			DEBUGP_SYS("addArp: ERROR - arp(ip=0x%08X) already exist! \n", ip);
			return LR_EXIST;
		}
	}
	
	/* Create */
	if(!CTAILQ_EMPTY(&arp_list_free)) {
		struct Arp_List_Entry *entry_arp;
		entry_arp = CTAILQ_FIRST(&arp_list_free);
		entry_arp->ip = ip;
		entry_arp->mac = *mac;
		entry_arp->flags = flags;
		entry_arp->vaild = 0xff;
		CTAILQ_REMOVE(&arp_list_free, entry_arp, tqe_link);
		CTAILQ_INSERT_TAIL(&arp_list_inuse, entry_arp, tqe_link);
		CTAILQ_INSERT_TAIL(&table_arp->list[hash], entry_arp, arp_link);
	} else {
		DEBUGP_SYS("addArp: ERROR - arp_list_free is empty! \n");
		return LR_FAILED;
	}
	
	return LR_SUCCESS;
}

enum LR_RESULT __SWAP 
rtl865x_modifyArp(ip_t ip,
		ether_t* mac,
		enum ARP_FLAGS flags)
{
	__u32 hash = FastPath_Hash_ARP_Entry(ip);
	struct Arp_List_Entry *ep;
	
	DEBUGP_API("modifyArp: ip=0x%08X mac=%02X:%02X:%02X:%02X:%02X:%02X flags=0x%08X \n", ip, MAC2STR(*mac), flags);
	
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
		if (ep->ip == ip) {
			ep->mac = *mac;
			ep->flags = flags;
			
			return LR_SUCCESS;
		}
	}
	
	return LR_SUCCESS;
}

/*
	delArp() - Delete an entry of Arp Table.
*/
enum LR_RESULT __SWAP 
rtl865x_delArp(ip_t ip)
{
	__u32 hash = FastPath_Hash_ARP_Entry(ip);
	struct Arp_List_Entry *ep;
	
	DEBUGP_API("delArp: ip=0x%08X \n", ip);
	
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
		if (ep->ip == ip) {
			ep->vaild = 0x00;
			CTAILQ_REMOVE(&table_arp->list[hash], ep, arp_link);
			CTAILQ_REMOVE(&arp_list_inuse, ep, tqe_link);
			CTAILQ_INSERT_TAIL(&arp_list_free, ep, tqe_link);
			
			return LR_SUCCESS;
		}
	}
	
	return LR_NONEXIST;
}

//cathy
u32 LANsub[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
u32 LANmask[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
u32 routeIndex = 0;

__u32 ExistInLAN(ip_t ip, ip_t mask){
	__u32 i;
	if(ip){
		for( i=0; i<routeIndex&& routeIndex<=8; i++ ) 
			if( LANsub[i] == ip && LANmask[i] == mask ) 		
				return 1;	//found it, return 1
	}
	return 0; //doesnt exist, return 0
}


enum LR_RESULT __SWAP 
rtl865x_addRoute(ip_t ip,
		ip_t mask,
		ip_t gateway,
		__u8* ifname,
		enum RT_FLAGS flags,
		int type)
{
	__u32 hash = FastPath_Hash_ROUTE_Entry(ip, mask);
	
	/* For fixing the bug that system will be hang if keep changing secondary IP&netmask . */
        // Just skip the addition if node already existed in array 
	struct Route_List_Entry *ep;		
	/* Looking for the matched ip & netmask node in list */
	CTAILQ_FOREACH(ep, &route_list_inuse, tqe_link) {
		if(ep->ip==ip && ep->mask == mask)			
			return LR_SUCCESS;	
        }
	DEBUGP_API("addRoute: ip=0x%08X mask=0x%08X gateway=0x%08X ifname=%s flags=0x%08X Hash=%u type=%d\n", 
		ip, mask, gateway, ifname, flags, hash, type);
	
	if(!CTAILQ_EMPTY(&route_list_free)) {
		struct Route_List_Entry *entry_route;
		entry_route = CTAILQ_FIRST(&route_list_free);
		entry_route->ip = ip;
		entry_route->mask = mask;
		entry_route->gateway = gateway;
		memcpy(&entry_route->ifname, ifname, IFNAME_LEN_MAX - 1);
		entry_route->flags = flags;
		entry_route->vaild = 0xff;
		CTAILQ_REMOVE(&route_list_free, entry_route, tqe_link);
		CTAILQ_INSERT_TAIL(&route_list_inuse, entry_route, tqe_link);
		CTAILQ_INSERT_TAIL(&table_route->list[hash], entry_route, route_link);
		//cathy, for multi-subnet
		if( type == RTN_UNICAST && strstr(ifname, "br") ) {
                     	/* For fixing the bug that system will be hang if keep changing secondary IP&netmask . */
 			//Add it if ip&mask we dont know
			if(ExistInLAN(ip, mask)==0){
			LANsub[routeIndex] = ip & mask;
			LANmask[routeIndex] = mask;
			routeIndex++;
		}
		}
	} else {
		DEBUGP_SYS("addRoute: ERROR - Route_list_free is empty! \n");
		
		return LR_FAILED;
	}
	
	return LR_SUCCESS;
}

enum LR_RESULT __SWAP 
rtl865x_modifyRoute(ip_t ip,
		ip_t mask,
		ip_t gateway,
		__u8* ifname,
		enum RT_FLAGS flags,
		int type)
{
	__u32 hash = FastPath_Hash_ROUTE_Entry(ip, mask);
	struct Route_List_Entry *ep;
	
	DEBUGP_API("modifyRoute: ip=0x%08X mask=0x%08X gateway=0x%08X ifname=%s flags=0x%08X \n", 
		ip, mask, gateway, ifname, flags);
		
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_route->list[hash], route_link) {
		if (ep->ip == ip && ep->mask == mask) {
			ep->gateway = gateway;
			memcpy(&ep->ifname, ifname, IFNAME_LEN_MAX - 1);
			ep->flags = flags;
			CTAILQ_REMOVE(&table_route->list[hash], ep, route_link);
			CTAILQ_REMOVE(&route_list_inuse, ep, tqe_link);
			CTAILQ_INSERT_TAIL(&route_list_free, ep, tqe_link);
			return LR_SUCCESS;
		}
	}
	
	return LR_SUCCESS;
}

enum LR_RESULT __SWAP 
rtl865x_delRoute(ip_t ip, ip_t mask)
{
	__u32 hash = FastPath_Hash_ROUTE_Entry(ip, mask);
	struct Route_List_Entry *ep;
	int i;
	DEBUGP_API("delRoute: ip=0x%08X mask=0x%08X \n", ip, mask);
	
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_route->list[hash], route_link) {
		if (ep->ip == ip && ep->mask == mask) {
			ep->vaild = 0x00;
			CTAILQ_REMOVE(&table_route->list[hash], ep, route_link);
			CTAILQ_REMOVE(&route_list_inuse, ep, tqe_link);
			CTAILQ_INSERT_TAIL(&route_list_free, ep, tqe_link);
			//cathy, for multi-subnet
			for( i=0; i<routeIndex&& routeIndex<=8; i++ ) {
				if( (LANsub[i] == (ip&mask))  &&  (LANmask[i] == mask) ) {
					LANsub[i] = 0;
					LANmask[i] = 0;
					routeIndex--;
					break;
				}
			}
			for( ; i<routeIndex&&routeIndex<=8; i++ ) {
				LANsub[i] = LANsub[i+1];
				LANmask[i] = LANmask[i+1];
			}									
			return LR_SUCCESS;
		}
	}
	
	return LR_NONEXIST;
}

enum LR_RESULT __SWAP 
rtl865x_addSession(__u8* ifname,
		enum SE_TYPE seType,
		__u32 sessionId,
		enum SE_FLAGS flags )
{
	return LR_SUCCESS;
}

enum LR_RESULT __SWAP 
rtl865x_delSession(__u8* ifname)
{
	return LR_SUCCESS;
}
struct Arp_List_Entry *FastPath_Find_ARP_Entry(ip_t ipaddr)
{
	struct Arp_List_Entry *ep;
	__u32 hash = FastPath_Hash_ARP_Entry(ipaddr);
	
	CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
		if (ep->ip == ipaddr) {
			return ep;			
		}
	}
	return NULL;
}

struct Route_List_Entry *
rtl867x_lookupRoute(ip_t ip)		
{
	struct Route_List_Entry *ep,*default_route=NULL;
		
	/* Lookup */
	CTAILQ_FOREACH(ep, &route_list_inuse, tqe_link) 
	{
			if ((ep->vaild==0xff)&&((ip & ep->mask) == (ep->ip & ep->mask)))
			{
				if(ep->mask!=0)
				{
					return ep;
				}
				else
				{
					default_route=ep;
				}
			}	

	}
	
	return default_route;
}

enum LR_RESULT  __SWAP 
rtl865x_addNaptConnection(struct ip_conntrack_tuple ori_tuple,
		struct ip_conntrack_tuple reply_tuple, enum NP_FLAGS flags)
{
	__u32 hash;
	struct Napt_List_Entry *ep;
	//cathy
	__u8 *proto;
	__u16 protocol;
	struct ip_conntrack_tuple tpdir1, tpdir2;
	ip_t intIp, extIp, remIp;
	__u32 intPort, extPort, remPort;
	int i;

	// for GCC 4.x.0 warning
	tpdir1.src.ip = 0;
	tpdir2.src.ip = 0;
	tpdir2.dst.ip = 0;
	
	protocol = ori_tuple.dst.protonum;	

	if( protocol == IPPROTO_TCP ) {
		proto = "TCP";
	}
	else if( protocol == IPPROTO_UDP ) {
		proto = "UDP";
	}
	else {
		proto = "unknow";
	}

	//cathy, for multi-subnet
	for(i=0; i< routeIndex; i++) {
		if ( (ori_tuple.src.ip & LANmask[i]) == LANsub[i] ) {
			tpdir1 = ori_tuple;
			tpdir2 = reply_tuple;
			break;
		}
		else {
			tpdir1 = reply_tuple;
			tpdir2 = ori_tuple;
		}
	}	

	intIp = tpdir1.src.ip;
	intPort = ntohs(tpdir1.src.u.all);
	extIp = tpdir2.dst.ip;
	extPort = ntohs(tpdir2.dst.u.all);
	remIp = tpdir2.src.ip;
	remPort = ntohs(tpdir2.src.u.all);
	
	hash = FastPath_Hash_NAPT_Entry(intIp, intPort, extIp, extPort, remIp, remPort);
		
	
	DEBUGP_API("addNaptConnection: P=%s int=%u.%u.%u.%u:%u ext=%u.%u.%u.%u:%u rem=%u.%u.%u.%u:%u F=%d (H=%u, Ha=%u, Hb=%u)\n", 
		proto, NIPQUAD(intIp), intPort, NIPQUAD(extIp), extPort, NIPQUAD(remIp), remPort, flags, hash,
		FastPath_Hash_PATH_Entry(intIp, intPort, remIp, remPort, protocol), FastPath_Hash_PATH_Entry(remIp, remPort, extIp, extPort, protocol));
//cathy
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_napt->list[hash], napt_link) {
		if ((ep->protocol == protocol) &&
			(ep->intIp == intIp) &&
			(ep->intPort == intPort) &&
			(ep->extIp == extIp) &&
			(ep->extPort == extPort) &&
			(ep->remIp == remIp) &&
			(ep->remPort == remPort)) {
			DEBUGP_SYS("addNaptConnection: ERROR - the entry already exist! \n");
			
			return LR_SUCCESS;
		}
	}
	
	if(!CTAILQ_EMPTY(&napt_list_free)) {
		struct Napt_List_Entry *entry_napt;
		entry_napt = CTAILQ_FIRST(&napt_list_free);
		entry_napt->protocol = protocol;
		entry_napt->intIp = intIp;
		entry_napt->intPort = intPort;
		entry_napt->extIp = extIp;
		entry_napt->extPort = extPort;
		entry_napt->remIp = remIp;
		entry_napt->remPort = remPort;
		entry_napt->flags = flags;
		entry_napt->vaild = 0xff;
		CTAILQ_REMOVE(&napt_list_free, entry_napt, tqe_link);
		CTAILQ_INSERT_TAIL(&napt_list_inuse, entry_napt, tqe_link);
		CTAILQ_INSERT_TAIL(&table_napt->list[hash], entry_napt, napt_link);
		
		/* add Path Table Entry */
		if (1) {
			__u32	hash;
			struct Path_List_Entry *entry_path;
		//downstream
			/* course = 2 (Inbound) */
			hash = FastPath_Hash_PATH_Entry(remIp, remPort, extIp, extPort, protocol);  //cathy
			entry_path = CTAILQ_FIRST(&path_list_free);
			entry_path->protocol	= &entry_napt->protocol;
			entry_path->in_sIp		= &entry_napt->remIp;
			entry_path->in_sPort	= &entry_napt->remPort;
			entry_path->in_dIp		= &entry_napt->extIp;
			entry_path->in_dPort	= &entry_napt->extPort;
			entry_path->out_sIp		= &entry_napt->remIp;
			entry_path->out_sPort	= &entry_napt->remPort;
			entry_path->out_dIp		= &entry_napt->intIp;
			entry_path->out_dPort	= &entry_napt->intPort;
			entry_path->out_ifname	= FastPath_Route(*entry_path->out_dIp);
			entry_path->arp_entry	= NULL;
			entry_path->course		= 2;
			entry_path->vaild		= 0xff;
			entry_path->dst			= NULL;
			entry_path->type		= 0;	/* Init: Normal (Only Routing) */
			if (*entry_path->in_dIp != *entry_path->out_dIp) {
				entry_path->type |= 4;	/* DNAT */
			}
			if (*entry_path->in_dPort != *entry_path->out_dPort) {
				entry_path->type |= 8;	/* DNPT */
			}
			CTAILQ_REMOVE(&path_list_free, entry_path, tqe_link);
			CTAILQ_INSERT_TAIL(&path_list_inuse, entry_path, tqe_link);
			CTAILQ_INSERT_TAIL(&table_path->list[hash], entry_path, path_link);
			
		}
	} else {
		DEBUGP_SYS("addNaptConnection: ERROR - Napt_list_free is empty! \n");
		
		return LR_FAILED;
	}
	
	return LR_SUCCESS;
}

enum LR_RESULT __SWAP 
rtl865x_delNaptConnection (struct ip_conntrack_tuple ori_tuple,
		struct ip_conntrack_tuple reply_tuple)
{
	__u32 hash;
	struct Napt_List_Entry *ep;
	//cathy
	__u8 *proto;
	__u16 protocol;
	struct ip_conntrack_tuple tpdir1, tpdir2;
	ip_t intIp, extIp, remIp;
	__u32 intPort, extPort, remPort;
	int i;
	protocol = ori_tuple.dst.protonum;

	// for GCC 4.x.0 warning
	tpdir1.src.ip = 0;
	tpdir2.src.ip = 0;
	tpdir2.dst.ip = 0;
	
	if( protocol == IPPROTO_TCP ) {
		proto = "TCP";
	}
	else if( protocol == IPPROTO_UDP ) {
		proto = "UDP";
	}
	else {
		proto = "unknow";
	}

	//cathy, for multi-subnet
	for(i=0; i< routeIndex; i++) {
		if ( (ori_tuple.src.ip & LANmask[i]) == LANsub[i] ) {
			tpdir1 = ori_tuple;
			tpdir2 = reply_tuple;
			break;
		}
		else {
			tpdir1 = reply_tuple;
			tpdir2 = ori_tuple;
		}
	}	
	

	intIp = tpdir1.src.ip;
	intPort = ntohs(tpdir1.src.u.all);
	extIp = tpdir2.dst.ip;
	extPort = ntohs(tpdir2.dst.u.all);
	remIp = tpdir2.src.ip;
	remPort = ntohs(tpdir2.src.u.all);
	
	hash = FastPath_Hash_NAPT_Entry(intIp, intPort, extIp, extPort, remIp, remPort);
	
	DEBUGP_API("delNaptConnection: P=%s int=%u.%u.%u.%u:%u ext=%u.%u.%u.%u:%u rem=%u.%u.%u.%u:%u \n", 
		proto, NIPQUAD(intIp), intPort, NIPQUAD(extIp), extPort, NIPQUAD(remIp), remPort);
	
	/* Lookup */
	CTAILQ_FOREACH(ep, &table_napt->list[hash], napt_link) {
		if ((ep->protocol == protocol) &&
			(ep->intIp == intIp) &&
			(ep->intPort == intPort) &&
			(ep->extIp == extIp) &&
			(ep->extPort == extPort) &&
			(ep->remIp == remIp) &&
			(ep->remPort == remPort)) {
			ep->vaild = 0x00;
			CTAILQ_REMOVE(&table_napt->list[hash], ep, napt_link);
			CTAILQ_REMOVE(&napt_list_inuse, ep, tqe_link);
			CTAILQ_INSERT_TAIL(&napt_list_free, ep, tqe_link);
			
			/* del Path Table Entry */
			if (1) {
				__u32	hash;
				struct Path_List_Entry *entry_path;
		//downstream
				/* course = 2 (Inbound) */
				hash = FastPath_Hash_PATH_Entry(remIp, remPort, extIp, extPort, protocol);  //cathy
				CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
					if ((entry_path->protocol == &ep->protocol) && (entry_path->course == 2)) {
						entry_path->vaild = 0x00;
						//cathy, fix dst destroyed before fastpath entry releases it
						if( entry_path->dst ) {
							dst_release(entry_path->dst);							
						}
						CTAILQ_REMOVE(&table_path->list[hash], entry_path, path_link);
						CTAILQ_REMOVE(&path_list_inuse, entry_path, tqe_link);
						CTAILQ_INSERT_TAIL(&path_list_free, entry_path, tqe_link);
						break;
					}
				}
			}
					
			return LR_SUCCESS;
		}
	}
	
	return LR_NONEXIST;
}

/* ==================================================================================================== */
__u8 *
FastPath_Route(ip_t dIp)
{
	__u8 *ifname = NULL;
	__u32 mask_max = 0x0;
	struct Route_List_Entry *ep;
	
	/* Lookup */
	CTAILQ_FOREACH(ep, &route_list_inuse, tqe_link) {
		if ((ep->mask >= mask_max) && ((dIp & ep->mask) == ep->ip)) {
			ifname = &ep->ifname[0];
			mask_max = ep->mask;
		}
	}
	
	return ifname;
}

/* ==================================================================================================== */
        /* cached hardware header; allow for machine alignment needs.        */
#define HH_DATA_MOD     16
#define HH_DATA_ALIGN(__len) \
        (((__len)+(HH_DATA_MOD-1))&~(HH_DATA_MOD - 1))
#include "../../bridge/br_private.h"

extern void nic_tx2(struct sk_buff* skb);
static inline int __SWAP ip_finish_output3(struct sk_buff *skb)
{
	struct dst_entry *dst = skb->dst;
	struct hh_cache *hh = dst->hh;
	struct net_bridge_fdb_entry *fpdst;
	struct net_bridge *br;	//suppose skb->dev is bridge
	unsigned char *dest;
	
	if (hh) {
		int hh_alen;
		
		read_lock_bh(&hh->hh_lock);
		hh_alen = HH_DATA_ALIGN(hh->hh_len);
  		memcpy(skb->data - hh_alen, hh->hh_data, hh_alen);
		read_unlock_bh(&hh->hh_lock);
	       skb_push(skb, hh->hh_len);
	       dest = skb->mac.raw;
	       br = skb->dev->priv;
	       if( !br ) {
			kfree_skb(skb);
			return 1;
	       }
		if ((fpdst = br_fdb_get(br, dest)) != NULL) {
			if(!strncmp("eth0",fpdst->dst->dev->name, 4)){			
				if(netif_running(fpdst->dst->dev)){
					fpdst->dst->dev->hard_start_xmit(skb, fpdst->dst->dev);		
				}
				else
					kfree_skb(skb);
			}else if(!strncmp("eth1",fpdst->dst->dev->name, 4)){			
				if(netif_running(fpdst->dst->dev)){
					fpdst->dst->dev->hard_start_xmit(skb, fpdst->dst->dev);		
				}
				else
					kfree_skb(skb);
			}else if(!strncmp("wlan0",fpdst->dst->dev->name, 5)){			
				if(netif_running(fpdst->dst->dev)){
					fpdst->dst->dev->hard_start_xmit(skb, fpdst->dst->dev);		
				}
				else
					kfree_skb(skb);
			}else if(!strcmp("usb0",fpdst->dst->dev->name)) {
				if(netif_running(fpdst->dst->dev)){
                                        fpdst->dst->dev->hard_start_xmit(skb, fpdst->dst->dev);
                                }
                                else
                                        kfree_skb(skb);
			}
			else{
				//printk("not eth0:%x %x %x %x %x %x\n",dest[0],dest[1],dest[2],dest[3],dest[4],dest[5]);
				kfree_skb(skb);
			}
			return 1;
		}
		else{
			if(netif_running(fpdst->dst->dev))
				fpdst->dst->dev->hard_start_xmit(skb, fpdst->dst->dev);		
			return 1; 
			//printk("can not found in fdb:%x %x %x %x %x %x\n",dest[0],dest[1],dest[2],dest[3],dest[4],dest[5]);
		}
		kfree_skb(skb);
		return 1;
	}
	//printk(KERN_DEBUG "ip_finish_output3: No header cache and no neighbour!\n");
	kfree_skb(skb);
	return -EINVAL;
}
#ifdef CONFIG_PPPOE_PROXY_FASTPATH
static inline int ppp_proxy_output(struct sk_buff *skb){

	 
      char *peer =NULL;
	  int  session ;
	  int type;
      
      GetPPPoeInfo(skb,&session,&peer);
//	 printk("peer addr %02x:%02x:%02x:%02x:%02x:%02x \n ",(peer)[0],(peer)[1],(peer)[2],(peer)[3],(peer)[4],(peer)[5]);
 
       if(peer!=NULL){
	   	if(!findmacType(peer,&type))
			return 0;
       }
	  
	 if(!pppoe_xmit2(skb,peer,session,type))
	 	return 0;
       return 1;

}
#endif
#ifdef CONFIG_PPPOE_PROXY_FASTPATH
__IRAM_GEN int
FastPath_Enter(struct sk_buff *skb)	/* Ethertype = 0x0800 (IP Packet) */
{
	struct iphdr *iph;
	
	__u32 sIp,dIp;
	__u16 sPort=0,dPort=0;
	
	int pppoe_proxy_data =0;
       unsigned char check_arpentry =0;
	   
	if (!fastpath_forward_flag) return 0;
	
	
	iph = skb->nh.iph;
	sIp = iph->saddr;
	dIp = iph->daddr;
	if(iph->frag_off & htons(0x3fff)){
		return 0;
	}

	switch (iph->protocol) {
	case IPPROTO_TCP: {
		struct tcphdr *tcph;
		__u32 hash;
		struct Path_List_Entry *entry_path;
		
		tcph = (struct tcphdr*)((__u32 *)iph + iph->ihl);
		sPort = tcph->source;
		dPort = tcph->dest;
		
		DEBUGP_PKT("==>> [%08X, %08X] SIP: %u.%u.%u.%u:%u  (%s) -> DIP: %u.%u.%u.%u:%u \n", 
			skb->csum, tcph->check,
			NIPQUAD(iph->saddr), tcph->source, skb->dev->name,
			NIPQUAD(iph->daddr), tcph->dest);

		/* DONT care now! */
		if (!strcmp(skb->dev->name, "lo")) return 0;
		if (tcph->fin || tcph->rst || tcph->syn) return 0;
		
		hash = FastPath_Hash_PATH_Entry(sIp, sPort, dIp, dPort, iph->protocol); //cathy
		CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
			if ((*entry_path->in_sPort == sPort) && 
				(*entry_path->in_dPort == dPort) && 
				(*entry_path->in_sIp == sIp) &&
				(*entry_path->in_dIp == dIp) &&
				(*entry_path->protocol == IPPROTO_TCP)) {  //cathy
				check_arpentry =entry_path->arp_entry && entry_path->arp_entry->vaild && (entry_path->arp_entry->ip == *entry_path->out_dIp);
				/* ARP Cache check */
				if (check_arpentry||pppoe_proxy_enabled) {
					/* ARP Cache valid */
					if (entry_path->dst == NULL) {
						if(ip_route_input(skb, *entry_path->out_dIp, iph->saddr, iph->tos, skb->dev))
							return 0;
						entry_path->dst = skb->dst;
					} else {
						skb->dst = entry_path->dst;
					}

					if(!check_arpentry){				   	
					    if(strncmp(skb->dst->dev->name, "ppp",3))
						 return 0; 
					    else
						 pppoe_proxy_data=1; 
                                 	}	
					
					skb->dst->lastuse = jiffies;
					dst_hold(skb->dst); //cathy, fix dst cache full problem, dst should be held when it is referenced						
					skb->dst->__use++;
					
					DEBUGP_PKT("FORWARD to [%s] \n", entry_path->out_ifname);
					switch(entry_path->type) {
					case 0:	{	/* Only Routing */
						break;
					}
					case 4: {	/* DNAT */
						int org_tcp=0;
						org_tcp=tcph->check;
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, tcph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						if(tcph->check==0){
							int idx1; 
							printk("err org %x\n",org_tcp);
							for(idx1=0;idx1<0x50;idx1++){
								if(idx1%0x10==0 && idx1!=0)
									printk("\n");
								printk("%2x ",skb->data[idx1]);
							}
						}
						break;
					}
					case 8: /* DNPT */
					case 12: {	/* DNAPT */						
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, tcph->dest, tcph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						tcph->dest			= *entry_path->out_dPort;
						break;
					}
					default: {
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_sIp, skb->nh.iph->saddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT(*entry_path->out_sIp, skb->nh.iph->saddr, *entry_path->out_sPort, tcph->source, tcph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, tcph->dest, tcph->check);
						skb->nh.iph->saddr	= *entry_path->out_sIp;
						tcph->source		= *entry_path->out_sPort;
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						tcph->dest			= *entry_path->out_dPort;
						break;
					}
					}
					
					skb->ip_summed = 0x0;
					//skb->dst->output(skb);	/* ip_output() */
					skb->dev = skb->dst->dev;
					//alex for pppoe proxy
					if (!pppoe_proxy_data)
					   ip_finish_output3(skb);
					else
					{
					  if(!ppp_proxy_output(skb))
					  	return 0;					 
					}
					return NET_RX_DROP;
				} else {
					/* Arp Cache update */
					struct Arp_List_Entry *ep;
					__u32 hash = FastPath_Hash_ARP_Entry(*entry_path->out_dIp);
					
					CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
						if (ep->ip == *entry_path->out_dIp) {
							entry_path->arp_entry = ep;
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}
	case IPPROTO_UDP: {
		struct udphdr *udph;
		__u32 hash;
		struct Path_List_Entry *entry_path;
		
		udph = (struct udphdr*)((__u32 *)iph + iph->ihl);
		sPort = udph->source;
		dPort = udph->dest;
		
		DEBUGP_PKT("==>> [%08X, %08X] SIP: %u.%u.%u.%u:%u  (%s) -> DIP: %u.%u.%u.%u:%u <UDP> #0x%x\n", 
			skb->csum, udph->check,
			NIPQUAD(iph->saddr), udph->source, skb->dev->name,
			NIPQUAD(iph->daddr), udph->dest, iph->frag_off);
		
		/* DONT care now! */
		if (!strcmp(skb->dev->name, "lo")) return 0;
		if (iph->frag_off & 0x3fff) return 0;	/* Ignore fragment */
		
		hash = FastPath_Hash_PATH_Entry(sIp, sPort, dIp, dPort, iph->protocol);  //cathy
		CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
			if ((*entry_path->in_sPort == sPort) && 
				(*entry_path->in_dPort == dPort) && 
				(*entry_path->in_sIp == sIp) &&
				(*entry_path->in_dIp == dIp) &&
				(*entry_path->protocol == IPPROTO_UDP)) {  //cathy
				check_arpentry=entry_path->arp_entry && entry_path->arp_entry->vaild && (entry_path->arp_entry->ip == *entry_path->out_dIp);
				/* ARP Cache check */
				if (check_arpentry||pppoe_proxy_enabled) {
					/* ARP Cache valid */
					if (entry_path->dst == NULL) {
						if(ip_route_input(skb, *entry_path->out_dIp, iph->saddr, iph->tos, skb->dev))
							return 0;
						entry_path->dst = skb->dst;						
					} else {
						skb->dst = entry_path->dst;
					}
					
                                  if(!check_arpentry){				   	
					    if(strncmp(skb->dst->dev->name, "ppp",3))
						 return 0; 
					    else
						 pppoe_proxy_data=1; 
                                 	}	
					
					skb->dst->lastuse = jiffies;
					dst_hold(skb->dst); //cathy, fix dst cache full problem						
					skb->dst->__use++;
					
					DEBUGP_PKT("FORWARD to [%s] \n", entry_path->out_ifname);
					switch(entry_path->type) {
					case 0:	{	/* Only Routing */
						break;
					}
					case 4: {	/* DNAT */
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, udph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					case 8: /* DNPT */
					case 12: {	/* DNAPT */
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, udph->dest, udph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						udph->dest			= *entry_path->out_dPort;
						break;
					}
					default: {
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_sIp, skb->nh.iph->saddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT_UDP(*entry_path->out_sIp, skb->nh.iph->saddr, *entry_path->out_sPort, udph->source, udph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, udph->dest, udph->check);
						skb->nh.iph->saddr	= *entry_path->out_sIp;
						udph->source		= *entry_path->out_sPort;
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						udph->dest			= *entry_path->out_dPort;
						break;
					}
					}
					
					skb->ip_summed = 0x0;
					//skb->dst->output(skb);	/* ip_output() */
					skb->dev = skb->dst->dev;
					
					if (!pppoe_proxy_data)
					   ip_finish_output3(skb);
					else
					{
					  if(!ppp_proxy_output(skb))
					  	return 0;					 
					}
					
					return NET_RX_DROP;
				} else {
					/* Arp Cache update */
					struct Arp_List_Entry *ep;
					__u32 hash = FastPath_Hash_ARP_Entry(*entry_path->out_dIp);
					
					CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
						if (ep->ip == *entry_path->out_dIp) {
							entry_path->arp_entry = ep;
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}
	case IPPROTO_ICMP:{  //cathy
		break;	
	}
	default: {  //cathy
		__u32 hash;
		struct Path_List_Entry *entry_path;
		
		sPort = 0;
		dPort = 0;
		
		/* DONT care now! */
		if (!strcmp(skb->dev->name, "lo")) return 0;
		if (iph->frag_off & 0x3fff) return 0;	/* Ignore fragment */
		
		hash = FastPath_Hash_PATH_Entry(sIp, sPort, dIp, dPort, iph->protocol);
		CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
			if ((*entry_path->protocol == iph->protocol) && 
				(*entry_path->in_sIp == sIp) &&
				(*entry_path->in_dIp == dIp) ) {
			check_arpentry=entry_path->arp_entry && entry_path->arp_entry->vaild && (entry_path->arp_entry->ip == *entry_path->out_dIp);	
				/* ARP Cache check */
				if (check_arpentry||pppoe_proxy_enabled) {
					/* ARP Cache valid */
					if (entry_path->dst == NULL) {
						if(ip_route_input(skb, *entry_path->out_dIp, iph->saddr, iph->tos, skb->dev))
							return 0;
						entry_path->dst = skb->dst;						
					} else {
						skb->dst = entry_path->dst;						
					}
					skb->dst->lastuse = jiffies;
					dst_hold(skb->dst); //cathy, fix dst cache full problem						
					skb->dst->__use++;
					
					DEBUGP_PKT("FORWARD to [%s] \n", entry_path->out_ifname);
					switch(entry_path->type) {
					case 0:	{	/* Only Routing */
						break;
					}					
					case 4: {	/* DNAT */
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					default: {
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_sIp, skb->nh.iph->saddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						skb->nh.iph->saddr	= *entry_path->out_sIp;
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					}
					
					skb->ip_summed = 0x0;
					skb->dev = skb->dst->dev;
					if (!pppoe_proxy_data)
					   ip_finish_output3(skb);
					else
					{
					  if(!ppp_proxy_output(skb))
					  	return 0;					 
					}
					return NET_RX_DROP;
				} else {
					/* Arp Cache update */
					struct Arp_List_Entry *ep;
					__u32 hash = FastPath_Hash_ARP_Entry(*entry_path->out_dIp);
					
					CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
						if (ep->ip == *entry_path->out_dIp) {
							entry_path->arp_entry = ep;
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}
	}
	
	return 0;
}
#else
__IRAM_GEN int
FastPath_Enter(struct sk_buff *skb)	/* Ethertype = 0x0800 (IP Packet) */
{
	struct iphdr *iph;
	
	__u32 sIp,dIp;
	__u16 sPort=0,dPort=0;
	
	if (!fastpath_forward_flag) return 0;
	
	iph = skb->nh.iph;
	sIp = iph->saddr;
	dIp = iph->daddr;
	if(iph->frag_off & htons(0x3fff)){
		return 0;
	}

	switch (iph->protocol) {
	case IPPROTO_TCP: {
		struct tcphdr *tcph;
		__u32 hash;
		struct Path_List_Entry *entry_path;
		
		tcph = (struct tcphdr*)((__u32 *)iph + iph->ihl);
		sPort = tcph->source;
		dPort = tcph->dest;
		
		DEBUGP_PKT("==>> [%08X, %08X] SIP: %u.%u.%u.%u:%u  (%s) -> DIP: %u.%u.%u.%u:%u \n", 
			skb->csum, tcph->check,
			NIPQUAD(iph->saddr), tcph->source, skb->dev->name,
			NIPQUAD(iph->daddr), tcph->dest);

		/* DONT care now! */
		if (!strcmp(skb->dev->name, "lo")) return 0;
		if (tcph->fin || tcph->rst || tcph->syn) return 0;
		
		hash = FastPath_Hash_PATH_Entry(sIp, sPort, dIp, dPort, iph->protocol); //cathy
		CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
			if ((*entry_path->in_sPort == sPort) && 
				(*entry_path->in_dPort == dPort) && 
				(*entry_path->in_sIp == sIp) &&
				(*entry_path->in_dIp == dIp) &&
				(*entry_path->protocol == IPPROTO_TCP)) {  //cathy
				/* ARP Cache check */
				if (entry_path->arp_entry && entry_path->arp_entry->vaild && (entry_path->arp_entry->ip == *entry_path->out_dIp)) {
					/* ARP Cache valid */
					if (entry_path->dst == NULL) {
						if(ip_route_input(skb, *entry_path->out_dIp, iph->saddr, iph->tos, skb->dev))
							return 0;
						entry_path->dst = skb->dst;
					} else {
						skb->dst = entry_path->dst;
					}
					skb->dst->lastuse = jiffies;
					dst_hold(skb->dst); //cathy, fix dst cache full problem, dst should be held when it is referenced						
					skb->dst->__use++;
					
					DEBUGP_PKT("FORWARD to [%s] \n", entry_path->out_ifname);
					switch(entry_path->type) {
					case 0:	{	/* Only Routing */
						break;
					}
					case 4: {	/* DNAT */
						int org_tcp=0;
						org_tcp=tcph->check;
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, tcph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					case 8: /* DNPT */
					case 12: {	/* DNAPT */						
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, tcph->dest, tcph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						tcph->dest			= *entry_path->out_dPort;
						break;
					}
					default: {
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_sIp, skb->nh.iph->saddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT(*entry_path->out_sIp, skb->nh.iph->saddr, *entry_path->out_sPort, tcph->source, tcph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, tcph->dest, tcph->check);
						skb->nh.iph->saddr	= *entry_path->out_sIp;
						tcph->source		= *entry_path->out_sPort;
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						tcph->dest			= *entry_path->out_dPort;
						break;
					}
					}
					
					skb->ip_summed = 0x0;
					skb->dev = skb->dst->dev;
					ip_finish_output3(skb);
					return NET_RX_DROP;
				} else {
					/* Arp Cache update */
					struct Arp_List_Entry *ep;
					__u32 hash = FastPath_Hash_ARP_Entry(*entry_path->out_dIp);
					
					CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
						if (ep->ip == *entry_path->out_dIp) {
							entry_path->arp_entry = ep;
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}
	case IPPROTO_UDP: {
		struct udphdr *udph;
		__u32 hash;
		struct Path_List_Entry *entry_path;
		
		udph = (struct udphdr*)((__u32 *)iph + iph->ihl);
		sPort = udph->source;
		dPort = udph->dest;
		
		DEBUGP_PKT("==>> [%08X, %08X] SIP: %u.%u.%u.%u:%u  (%s) -> DIP: %u.%u.%u.%u:%u <UDP> #0x%x\n", 
			skb->csum, udph->check,
			NIPQUAD(iph->saddr), udph->source, skb->dev->name,
			NIPQUAD(iph->daddr), udph->dest, iph->frag_off);
		
		/* DONT care now! */
		if (!strcmp(skb->dev->name, "lo")) return 0;
		if (iph->frag_off & 0x3fff) return 0;	/* Ignore fragment */
		
		hash = FastPath_Hash_PATH_Entry(sIp, sPort, dIp, dPort, iph->protocol);  //cathy
		CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
			if ((*entry_path->in_sPort == sPort) && 
				(*entry_path->in_dPort == dPort) && 
				(*entry_path->in_sIp == sIp) &&
				(*entry_path->in_dIp == dIp) &&
				(*entry_path->protocol == IPPROTO_UDP)) {  //cathy
				/* ARP Cache check */
				if (entry_path->arp_entry && entry_path->arp_entry->vaild && (entry_path->arp_entry->ip == *entry_path->out_dIp)) {
					/* ARP Cache valid */
					if (entry_path->dst == NULL) {
						if(ip_route_input(skb, *entry_path->out_dIp, iph->saddr, iph->tos, skb->dev))
							return 0;
						entry_path->dst = skb->dst;						
					} else {
						skb->dst = entry_path->dst;
					}
					skb->dst->lastuse = jiffies;
					dst_hold(skb->dst); //cathy, fix dst cache full problem						
					skb->dst->__use++;
					
					DEBUGP_PKT("FORWARD to [%s] \n", entry_path->out_ifname);
					switch(entry_path->type) {
					case 0:	{	/* Only Routing */
						break;
					}
					case 4: {	/* DNAT */
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, udph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					case 8: /* DNPT */
					case 12: {	/* DNAPT */
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, udph->dest, udph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						udph->dest			= *entry_path->out_dPort;
						break;
					}
					default: {
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_sIp, skb->nh.iph->saddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT_UDP(*entry_path->out_sIp, skb->nh.iph->saddr, *entry_path->out_sPort, udph->source, udph->check);
						FASTPATH_ADJUST_CHKSUM_NAPT_UDP(*entry_path->out_dIp, skb->nh.iph->daddr, *entry_path->out_dPort, udph->dest, udph->check);
						skb->nh.iph->saddr	= *entry_path->out_sIp;
						udph->source		= *entry_path->out_sPort;
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						udph->dest			= *entry_path->out_dPort;
						break;
					}
					}
					
					skb->ip_summed = 0x0;
					skb->dev = skb->dst->dev;
					ip_finish_output3(skb);
					return NET_RX_DROP;
				} else {
					/* Arp Cache update */
					struct Arp_List_Entry *ep;
					__u32 hash = FastPath_Hash_ARP_Entry(*entry_path->out_dIp);
					
					CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
						if (ep->ip == *entry_path->out_dIp) {
							entry_path->arp_entry = ep;
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}
	case IPPROTO_ICMP:{  //cathy
		break;	
	}
	default: {  //cathy
		__u32 hash;
		struct Path_List_Entry *entry_path;
		
		sPort = 0;
		dPort = 0;
		
		/* DONT care now! */
		if (!strcmp(skb->dev->name, "lo")) return 0;
		if (iph->frag_off & 0x3fff) return 0;	/* Ignore fragment */
		
		hash = FastPath_Hash_PATH_Entry(sIp, sPort, dIp, dPort, iph->protocol);
		CTAILQ_FOREACH(entry_path, &table_path->list[hash], path_link) {
			if ((*entry_path->protocol == iph->protocol) && 
				(*entry_path->in_sIp == sIp) &&
				(*entry_path->in_dIp == dIp) ) {
				/* ARP Cache check */
				if (entry_path->arp_entry && entry_path->arp_entry->vaild && (entry_path->arp_entry->ip == *entry_path->out_dIp)) {
					/* ARP Cache valid */
					if (entry_path->dst == NULL) {
						if(ip_route_input(skb, *entry_path->out_dIp, iph->saddr, iph->tos, skb->dev))
							return 0;
						entry_path->dst = skb->dst;						
					} else {
						skb->dst = entry_path->dst;						
					}
					skb->dst->lastuse = jiffies;
					dst_hold(skb->dst); //cathy, fix dst cache full problem						
					skb->dst->__use++;
					
					DEBUGP_PKT("FORWARD to [%s] \n", entry_path->out_ifname);
					switch(entry_path->type) {
					case 0:	{	/* Only Routing */
						break;
					}					
					case 4: {	/* DNAT */
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					default: {
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_sIp, skb->nh.iph->saddr, iph->check);
						FASTPATH_ADJUST_CHKSUM_NAT(*entry_path->out_dIp, skb->nh.iph->daddr, iph->check);
						skb->nh.iph->saddr	= *entry_path->out_sIp;
						skb->nh.iph->daddr	= *entry_path->out_dIp;
						break;
					}
					}
					
					skb->ip_summed = 0x0;
					skb->dev = skb->dst->dev;
					ip_finish_output3(skb);
					return NET_RX_DROP;
				} else {
					/* Arp Cache update */
					struct Arp_List_Entry *ep;
					__u32 hash = FastPath_Hash_ARP_Entry(*entry_path->out_dIp);
					
					CTAILQ_FOREACH(ep, &table_arp->list[hash], arp_link) {
						if (ep->ip == *entry_path->out_dIp) {
							entry_path->arp_entry = ep;
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}
	}
	
	return 0;
}

#endif
int __SWAP
FastPath_Track(struct sk_buff *skb)
{
	struct iphdr *iph;
	__u32 sip,dip;
	__u16 sport=0,dport=0;
	
	if (skb->nh.iph) {
		iph = skb->nh.iph;
		sip = iph->saddr;
		dip = iph->daddr;
		
		switch (iph->protocol) {
		case IPPROTO_TCP: { /* TCP */
			struct tcphdr *tcph;
			tcph = (struct tcphdr*)((__u32 *)iph + iph->ihl);
			sport = tcph->source;
			dport = tcph->dest;
			DEBUGP_PKT("<<== [%08X, %08X] SIP: %u.%u.%u.%u:%u -> DIP: %u.%u.%u.%u:%u (%s) \n", 
				skb->csum, tcph->check,
				NIPQUAD(sip), tcph->source,
				NIPQUAD(dip), tcph->dest, skb->dst->dev->name);
			break;
		}
		case IPPROTO_UDP: { /* UDP */
			struct udphdr *udph;
			udph = (struct udphdr*)((__u32 *)iph + iph->ihl);
			sport = udph->source;
			dport = udph->dest;
			DEBUGP_PKT("<<== [%08X, %08X] SIP: %u.%u.%u.%u:%u -> DIP: %u.%u.%u.%u:%u (%s) \n", 
				skb->csum, udph->check,
				NIPQUAD(sip), udph->source,
				NIPQUAD(dip), udph->dest, skb->dst->dev->name);
			break;
		}
		default:
			break;
		}
		
	}
	
	return 0;
}

#ifdef	DEBUG_PROCFILE
/*
static int fastpath_forward_proc(char *buffer, char **start, off_t offset, int length)
{
	int len=0;
	len += sprnitf(buffer + len, "%d\n", fastpath_forward_flag);
	return len;
}
*/

static int __SWAP fastpath_table_arp(char *buffer, char **start, off_t offset, int length)
{
	struct Arp_List_Entry *ep;
	int len=0;
	
	CTAILQ_FOREACH(ep, &arp_list_inuse, tqe_link) {
		len += sprintf(buffer + len, "~Arp: ip=0x%08X mac=%02X:%02X:%02X:%02X:%02X:%02X flags=0x%08X \n", ep->ip, MAC2STR(ep->mac), ep->flags);
	}
	
	return len;
}

static int __SWAP fastpath_table_route(char *buffer, char **start, off_t offset, int length)
{
	struct Route_List_Entry *ep;
	int len=0;
	
	CTAILQ_FOREACH(ep, &route_list_inuse, tqe_link) {
		len += sprintf(buffer + len, "~Route: ip=0x%08X mask=0x%08X gateway=0x%08X ifname=%-5s flags=0x%08X \n", 
			ep->ip, ep->mask, ep->gateway, ep->ifname, ep->flags);
	}
	
	return len;
}

static int __SWAP fastpath_table_napt(char *buffer, char **start, off_t offset, int length)
{
	struct Napt_List_Entry *ep;
	unsigned int len=0, newlen=0;
	//cathy
	__u8 *proto;
	off_t upto = 0;	
	CTAILQ_FOREACH(ep, &napt_list_inuse, tqe_link) {
		if( upto++ < offset ) 
			continue;
		if( ep->protocol  == IPPROTO_TCP ) {
			proto = "TCP";
		}
		else if( ep->protocol  == IPPROTO_UDP ) {
			proto = "UDP";
		}
		else {
			proto = "unknow";
		}
		newlen = sprintf(buffer + len, "~Napt: [%s] int=0x%08X:%-5u ext=0x%08X:%-5u rem=0x%08X:%-5u flags=0x%08X \n", 
			proto,
			ep->intIp, ep->intPort, ep->extIp, ep->extPort, ep->remIp, ep->remPort,
			ep->flags);
		if ( (len + newlen) > length) {
			goto finished;			
		}
		else {
			len += newlen;
		}
	}
finished:
	/* `start' hack - see fs/proc/generic.c line ~165 */
	*start = (char *)((unsigned int)upto - offset);	
	return len;
}

static int __SWAP fastpath_table_path(char *buffer, char **start, off_t offset, int length)
{
	struct Path_List_Entry *ep;
	unsigned int len=0, newlen=0;
	//cathy
	__u8 *proto;
	off_t upto = 0;	
	CTAILQ_FOREACH(ep, &path_list_inuse, tqe_link) {
		if( upto++ < offset ) 
			continue;
		if( *ep->protocol  == IPPROTO_TCP ) {
			proto = "TCP";
		}
		else if( *ep->protocol  == IPPROTO_UDP ) {
			proto = "UDP";
		}
		else {
			proto = "unknow";
		}
		newlen = sprintf(buffer + len, "~Path: [%s] in-S=0x%08X:%-5u in-D=0x%08X:%-5u out-S=0x%08X:%-5u out-D=0x%08X:%-5u out-ifname=%-5s <%u> {%d}\n", 
			proto,
			*ep->in_sIp, *ep->in_sPort, *ep->in_dIp, *ep->in_dPort,
			*ep->out_sIp, *ep->out_sPort, *ep->out_dIp, *ep->out_dPort,
			ep->out_ifname, ep->course, ep->type);
		if ( (len + newlen) > length) {
			goto finished;			
		}
		else {
			len += newlen;
		}
	}
finished:
	/* `start' hack - see fs/proc/generic.c line ~165 */
	*start = (char *)((unsigned int)upto - offset);		
	return len;
}

static int __SWAP fastpath_hash_path(char *buffer, char **start, off_t offset, int length)
{
	int i, len=0;
	
	for (i=0; i<PATH_TABLE_LIST_MAX; i++) {
		len += sprintf(buffer + len, "%5d ", CTAILQ_TOTAL(&table_path->list[i]));
		if (i%12 == 11) len += sprintf(buffer + len, "\n");
	}
	len += sprintf(buffer + len, "\n");	
	
	return len;
}

#endif	/* DEBUG_PROCFILE */

static struct proc_dir_entry *FP_Proc_File;
#define PROCFS_NAME 		"FastPath"
#include <asm/uaccess.h>
int fp_proc_read(struct file *file, const char *buffer,
                      unsigned long count, void *data)
{
	if(fp_on==1)
		printk("fastpath ON!\n");
	if(fp_on==0)
		printk("fastpath OFF!\n");
	return -1;
}
int fp_proc_write(struct file *file, const char *buffer,
                      unsigned long count, void *data)
{
	char proc_buffer[count];
	
	/* write data to the buffer */
	memset(proc_buffer, 0, sizeof(proc_buffer));
	if ( copy_from_user(proc_buffer, buffer, count) ) {
		return -EFAULT;
	}

	DEBUGP_API("Old_State:%d ", fp_state);
	switch(proc_buffer[0]) {
	case '0': fp_on = 0;break;
	case '1': fp_on = 1;break;
	default:
		printk("Error setting!\n");
	}
	DEBUGP_API("New_State:%d\n", fp_state);

	return -1;
}

static int __SWAP fastpath_memory_init(void) 
{
	int i;	
	/* Arp-Table Init */
	table_arp = (struct Arp_Table *)kmalloc(sizeof(struct Arp_Table), GFP_ATOMIC);
	if (table_arp == NULL) {
		DEBUGP_SYS("MALLOC Failed! (Arp Table) \n");
		return -1;
	}
	CTAILQ_INIT(&arp_list_inuse);
	CTAILQ_INIT(&arp_list_free);
	for (i=0; i<ARP_TABLE_LIST_MAX; i++) {
		CTAILQ_INIT(&table_arp->list[i]);
	}
	/* Arp-List Init */
	for (i=0; i<ARP_TABLE_ENTRY_MAX; i++) {
		struct Arp_List_Entry *entry_arp = (struct Arp_List_Entry *)kmalloc(sizeof(struct Arp_List_Entry), GFP_ATOMIC);
		if (entry_arp == NULL) {
			DEBUGP_SYS("MALLOC Failed! (Arp Table Entry) \n");
			return -2;
		}
		CTAILQ_INSERT_TAIL(&arp_list_free, entry_arp, tqe_link);
	}
	
	/* Route-Table Init */
	table_route = (struct Route_Table *)kmalloc(sizeof(struct Route_Table), GFP_ATOMIC);
	if (table_route == NULL) {
		DEBUGP_SYS("MALLOC Failed! (Route Table) \n");
		return -1;
	}
	CTAILQ_INIT(&route_list_inuse);
	CTAILQ_INIT(&route_list_free);
	for (i=0; i<ROUTE_TABLE_LIST_MAX; i++) {
		CTAILQ_INIT(&table_route->list[i]);
	}
	/* Route-List Init */
	for (i=0; i<ROUTE_TABLE_ENTRY_MAX; i++) {
		struct Route_List_Entry *entry_route = (struct Route_List_Entry *)kmalloc(sizeof(struct Route_List_Entry), GFP_ATOMIC);
		if (entry_route == NULL) {
			DEBUGP_SYS("MALLOC Failed! (Route Table Entry) \n");
			return -2;
		}
		CTAILQ_INSERT_TAIL(&route_list_free, entry_route, tqe_link);
	}
	
	/* Napt-Table Init */
	table_napt = (struct Napt_Table *)kmalloc(sizeof(struct Napt_Table), GFP_ATOMIC);
	if (table_napt == NULL) {
		DEBUGP_SYS("MALLOC Failed! (Napt Table) \n");
		return -1;
	}
	CTAILQ_INIT(&napt_list_inuse);
	CTAILQ_INIT(&napt_list_free);
	for (i=0; i<NAPT_TABLE_LIST_MAX; i++) {
		CTAILQ_INIT(&table_napt->list[i]);
	}
	/* Napt-List Init */
	for (i=0; i<NAPT_TABLE_ENTRY_MAX; i++) {
		struct Napt_List_Entry *entry_napt = (struct Napt_List_Entry *)kmalloc(sizeof(struct Napt_List_Entry), GFP_ATOMIC);
		if (entry_napt == NULL) {
			DEBUGP_SYS("MALLOC Failed! (Napt Table Entry) \n");
			return -2;
		}
		CTAILQ_INSERT_TAIL(&napt_list_free, entry_napt, tqe_link);
	}
	
	/* Path-Table Init */
	table_path = (struct Path_Table *)kmalloc(sizeof(struct Path_Table), GFP_ATOMIC);
	if (table_path == NULL) {
		DEBUGP_SYS("MALLOC Failed! (Path Table) \n");
		return -1;
	}
	CTAILQ_INIT(&path_list_inuse);
	CTAILQ_INIT(&path_list_free);
	for (i=0; i<PATH_TABLE_LIST_MAX; i++) {
		CTAILQ_INIT(&table_path->list[i]);
	}
	/* Path-List Init */
	for (i=0; i<PATH_TABLE_ENTRY_MAX; i++) {
		struct Path_List_Entry *entry_path = (struct Path_List_Entry *)kmalloc(sizeof(struct Path_List_Entry), GFP_ATOMIC);
		if (entry_path == NULL) {
			DEBUGP_SYS("MALLOC Failed! (Path Table Entry) \n");
			return -2;
		}
		CTAILQ_INSERT_TAIL(&path_list_free, entry_path, tqe_link);
	}

	return 0;
}


static int __init fastpath_init(void)
{
	//int i;
#ifdef	DEBUG_PROCFILE
	/* proc file for debug */
	proc_net_create("fp_arp", 0, fastpath_table_arp);
	proc_net_create("fp_route", 0, fastpath_table_route);
	proc_net_create("fp_napt", 0, fastpath_table_napt);
	proc_net_create("fp_path", 0, fastpath_table_path);
	proc_net_create("fp_hash_path", 0, fastpath_hash_path);
#endif	/* DEBUG_PROCFILE */
	
	printk("%s %s\n",MODULE_NAME, MODULE_VERSION_FP);
	
	//create proc
	FP_Proc_File= create_proc_entry(PROCFS_NAME, 0644, NULL);
	if (FP_Proc_File == NULL) {
		remove_proc_entry(PROCFS_NAME, &proc_root);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	FP_Proc_File->read_proc  = (read_proc_t *)fp_proc_read;
	FP_Proc_File->write_proc  = (write_proc_t *)fp_proc_write;
	FP_Proc_File->owner 	  = THIS_MODULE;
	FP_Proc_File->mode 	  = S_IFREG | S_IRUGO;
	FP_Proc_File->uid 	  = 0;
	FP_Proc_File->gid 	  = 0;
	FP_Proc_File->size 	  = 37;

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
	fastpath_memory_init();

	return 0;
}

static void __exit fastpath_exit(void)
{
	printk("%s %s removed!\n", MODULE_NAME, MODULE_VERSION_FP);
}

module_init(fastpath_init);
module_exit(fastpath_exit);
MODULE_LICENSE("GPL");
