/* -*- linux-c -*-

# (C) Copyright 2001-2003
# Alex Zeffertt, Cambridge Broadband Ltd, ajz@cambridgebroadband.com
# Re-worked by Ben Greear <greearb@candelatech.com>

*/

#ifndef MACVLAN_KERNEL_H_FILE__
#define MACVLAN_KERNEL_H_FILE__


/* NOTE:  If you change this below, you should probably change macvlan_hash_lookup as
 * well.  Especially if you make this bigger.
 */
#define MACVLAN_HASH_LEN 256

#define VLAN_BUCKET(a) a[5] % MACVLAN_HASH_LEN;

/* This can be made as large as desired, and mainly helps keep bad
 * IOCTL arguments from taking down the box.
 */
#define MAX_MACVLANS_PER_PORT 10000

/* Proc file related */
#define MVL_MX_ARG_LEN 80

#ifdef CONFIG_PROC_FS

/* To use or not to use the PROC-FS */
#define MVL_CONFIG_PROC_FS

#endif


/*********************************************************/
/*		       types				 */
/*********************************************************/
/* a macvlan_vlan represents an upper layer interface */
struct macvlan_vlan {
	struct net_device* dev;
	struct net_device_stats	statistics;
	struct macvlan_vlan *next;
	struct macvlan_port *port;
	struct net_device *lowerdev;
	atomic_t nmacs;  /* the number of mac addresses bound to this vlan */

#ifdef MVL_CONFIG_PROC_FS
        struct proc_dir_entry* proc_ent;
#endif        

};

struct macvlan_hash_entry {
	unsigned char mac[ETH_ALEN];    /* the eth hdr source to match.  Can
					 * match as destination too, see flags in
					 * macvlan_port.  Cannot match on both. */
	struct macvlan_vlan *vlan;	/* the vlan target */
	struct macvlan_hash_entry *next;/* next entry in list (same hash, any dev) */
};


/*
 * a macvlan_port represents a mux/demux between a mac-
 * based-vlan enabled ethernet device and vlans
 * layered on top of it
 */
struct macvlan_port {
	/* MAC to vlan lookup */
	struct macvlan_hash_entry *hash_table[MACVLAN_HASH_LEN];
	struct net_device *dev;  /* the mac-based-vlan enabled ethernet device */
	atomic_t ndevs;    /* number of vlans layered over dev */
	struct macvlan_vlan *vlan_list; /* list of vlans layered over this port */
	struct macvlan_port *next;    /* next port */
	
#define MVL_FILTER_ON_DEST 0x1	/* 0x1	filter-on-destination (instead of source) */
	int flags;

#ifdef MVL_CONFIG_PROC_FS
        struct proc_dir_entry* proc_dir;
        struct proc_dir_entry* proc_ent;
#endif        

};


#ifdef MVL_CONFIG_PROC_FS
static int read_mvl_glbl(char *page, char **start, off_t off,
			 int count, int *eof, void *data);
static int write_mvl_glbl(struct file *file, const char *buffer,
			  unsigned long count, void *data);
static int read_mvl(char *page, char **start, off_t off,
		    int count, int *eof, void *data);
static int write_mvl(struct file *file, const char *buffer,
		     unsigned long count, void *data);
static int read_mvl_port(char *page, char **start, off_t off,
			 int count, int *eof, void *data);
static int write_mvl_port(struct file *file, const char *buffer,
			  unsigned long count, void *data);
#endif


#endif

