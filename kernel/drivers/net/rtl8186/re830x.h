/*	
 *	re830x.h
*/
#include <linux/skbuff.h>

 //for switch function
extern int enable_IGMP_SNP;
extern int enable_port_mapping;
extern int enable_virtual_port;
extern void check_IGMP_snoop_rx(struct sk_buff *skb, int tag);
extern void process_eth_vlan(struct sk_buff *skb, int *tag);

void internal_miiar_write(unsigned char phyaddr,unsigned char regaddr,unsigned short value);
void miiar_write(unsigned char phyaddr,unsigned char regaddr,unsigned short value);

#ifdef CONFIG_RE8305
#define SWITCH_PORT_NUMBER	5
#else 					//#ifdef CONFIG_RE8306
#define SWITCH_PORT_NUMBER	7
#endif
#define SW_PORT_NUM		4	// virtual ports

// Kaohj -- SWITCH_PORT_NUMBER: used to identify switch ports;
//		5: used for system vlan (0: default group; 1~4: user-defined vlan)
#define VLAN_NUM	SWITCH_PORT_NUMBER+5

// per-port info
struct r8305_phy_struc {
	unsigned egtag;		// egress tag
	unsigned vlanIndex;	// pvid
};

// per vlan info
struct r8305_vlan_struc {
	unsigned vid;		// vlan ID
	unsigned member;		// 5-bit field to specifies which
					// ports are the memers of this vlan.
};

struct r8305_struc {
	unsigned vlan_en;	// vlan enable flag
	unsigned vlan_tag_aware;
	// Kaohj
	//struct r8305_phy_struc	phy[SWITCH_PORT_NUMBER];	// 5-port 8305 switch
	// Kaohj -- SWITCH_PORT_NUMBER: used to identify switch ports
	//		4: system-wise LAN ports info.
	struct r8305_phy_struc	phy[SWITCH_PORT_NUMBER+4];	// 5-port 8305 switch + 4 ports for vlan-grouping
	// Kaohj -- SWITCH_PORT_NUMBER: used to identify switch ports;
	//		5: used for system vlan (0: default group; 1~4: user-defined vlan)
	//struct r8305_vlan_struc	vlan[SWITCH_PORT_NUMBER];	// vlan A,B,C,D,E info.
	struct r8305_vlan_struc	vlan[VLAN_NUM];	// vlan A,B,C,D,E info.
};

extern struct r8305_struc rtl8305_info;

int get_vid(int pvid);
int get_pvid(int vid);

