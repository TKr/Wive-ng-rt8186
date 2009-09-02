/* -*-linux-c-*-
 * $Id: candela_2.4.21.patch,v 1.4 2003/09/30 21:05:04 greear Exp $
 * pktgen.c: Packet Generator for performance evaluation.
 *
 * See pktgen.c for details of changes, etc.
*/


#ifndef PKTGEN_H_INCLUDE_KERNEL__
#define PKTGEN_H_INCLUDE_KERNEL__


/* The buckets are exponential in 'width' */
#define LAT_BUCKETS_MAX 32

#define IP_NAME_SZ 32

/* Keep information per interface */
struct pktgen_interface_info {
        char ifname[32];
        
        /* Parameters */

        /* If min != max, then we will either do a linear iteration, or
         * we will do a random selection from within the range.
         */
        __u32 flags;     

#define F_IPSRC_RND   (1<<0)  /* IP-Src Random  */
#define F_IPDST_RND   (1<<1)  /* IP-Dst Random  */
#define F_UDPSRC_RND  (1<<2)  /* UDP-Src Random */
#define F_UDPDST_RND  (1<<3)  /* UDP-Dst Random */
#define F_MACSRC_RND  (1<<4)  /* MAC-Src Random */
#define F_MACDST_RND  (1<<5)  /* MAC-Dst Random */
#define F_SET_SRCMAC  (1<<6)  /* Specify-Src-Mac 
				 (default is to use Interface's MAC Addr) */
#define F_SET_SRCIP   (1<<7)  /*  Specify-Src-IP
				  (default is to use Interface's IP Addr) */ 
#define F_TXSIZE_RND  (1<<8)  /* Transmit size is random */

        int min_pkt_size;    /* = ETH_ZLEN; */
        int max_pkt_size;    /* = ETH_ZLEN; */
        int nfrags;
        __u32 ipg;    /* Default Interpacket gap in nsec */
        __u64 count;  /* Default No packets to send */
        __u64 sofar;  /* How many pkts we've sent so far */
        __u64 tx_bytes; /* How many bytes we've transmitted */
        __u64 errors;    /* Errors when trying to transmit, pkts will be re-sent */

        /* runtime counters relating to multiskb */
        __u64 next_tx_ns;          /* timestamp of when to tx next, in nano-seconds */
        
        __u64 fp;
        __u32 fp_tmp;
	int last_ok;           /* Was last skb sent? 
	                        * Or a failed transmit of some sort?  This will keep
                                * sequence numbers in order, for example.
                                */
        /* Fields relating to receiving pkts */
        __u32 last_seq_rcvd;
        __u64 ooo_rcvd;  /* out-of-order packets received */
        __u64 pkts_rcvd; /* packets received */
        __u64 dup_rcvd;  /* duplicate packets received */
        __u64 bytes_rcvd; /* total bytes received, as obtained from the skb */
        __u64 seq_gap_rcvd; /* how many gaps we received.  This coorelates to
                             * dropped pkts, except perhaps in cases where we also
                             * have re-ordered pkts.  In that case, you have to tie-break
                             * by looking at send v/s received pkt totals for the interfaces
                             * involved.
                             */
        __u64 non_pg_pkts_rcvd; /* Count how many non-pktgen skb's we are sent to check. */
        __u64 dup_since_incr; /* How many dumplicates since the last seq number increment,
                               * used to detect gaps when multiskb > 1
                               */
        int avg_latency; /* in micro-seconds */
        int min_latency;
        int max_latency;
        __u64 latency_bkts[LAT_BUCKETS_MAX];
        __u64 pkts_rcvd_since_clear; /* with regard to clearing/resetting the latency logic */
        
        __u64 started_at; /* micro-seconds */
        __u64 stopped_at; /* micro-seconds */
        __u64 idle_acc;
        __u32 seq_num;
        
        int multiskb; /* Use multiple SKBs during packet gen.  If this number
                          * is greater than 1, then that many coppies of the same
                          * packet will be sent before a new packet is allocated.
                          * For instance, if you want to send 1024 identical packets
                          * before creating a new packet, set multiskb to 1024.
                          */
        int peer_multiskb; /* Helps detect drops when multiskb > 1 on peer */
        int do_run_run;  /* if this changes to false, the test will stop */
        
        char dst_min[IP_NAME_SZ]; /* IP, ie 1.2.3.4 */
        char dst_max[IP_NAME_SZ]; /* IP, ie 1.2.3.4 */
        char src_min[IP_NAME_SZ]; /* IP, ie 1.2.3.4 */
        char src_max[IP_NAME_SZ]; /* IP, ie 1.2.3.4 */

        /* If we're doing ranges, random or incremental, then this
         * defines the min/max for those ranges.
         */
        __u32 saddr_min; /* inclusive, source IP address */
        __u32 saddr_max; /* exclusive, source IP address */
        __u32 daddr_min; /* inclusive, dest IP address */
        __u32 daddr_max; /* exclusive, dest IP address */

        __u16 udp_src_min; /* inclusive, source UDP port */
        __u16 udp_src_max; /* exclusive, source UDP port */
        __u16 udp_dst_min; /* inclusive, dest UDP port */
        __u16 udp_dst_max; /* exclusive, dest UDP port */

        __u32 src_mac_count; /* How many MACs to iterate through */
        __u32 dst_mac_count; /* How many MACs to iterate through */
        
        unsigned char dst_mac[6];
        unsigned char src_mac[6];
        
        __u32 cur_dst_mac_offset;
        __u32 cur_src_mac_offset;
        __u32 cur_saddr;
        __u32 cur_daddr;
        __u16 cur_udp_dst;
        __u16 cur_udp_src;
        __u32 cur_pkt_size;
        
        __u8 hh[14];
        /* = { 
           0x00, 0x80, 0xC8, 0x79, 0xB3, 0xCB, 
           
           We fill in SRC address later
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
           0x08, 0x00
           };
        */
        __u16 pad; /* pad out the hh struct to an even 16 bytes */
        char result[512];
        /* proc file names */
        char fname[80];

        /* End of stuff that user-space should care about */

        struct sk_buff* skb; /* skb we are to transmit next, mainly used for when we
                              * are transmitting the same one multiple times
                              */
        struct pktgen_thread_info* pg_thread; /* the owner */

        struct pktgen_interface_info* next_hash; /* Used for chaining in the hash buckets */
        struct pktgen_interface_info* next; /* Used for chaining in the thread's run-queue */

        

        struct net_device* odev; /* The out-going device.  Note that the device should
                                  * have it's pg_info pointer pointing back to this
                                  * device.  This will be set when the user specifies
                                  * the out-going device name (not when the inject is
                                  * started as it used to do.)
                                  */
        
        struct proc_dir_entry *proc_ent;
        
        int (*rcv) (struct sk_buff *skb);
}; /* pktgen_interface_info */


struct pktgen_hdr {
        __u32 pgh_magic;
        __u32 seq_num;
        struct timeval timestamp;
};


/* Define some IOCTLs.  Just picking random numbers, basically. */
#define GET_PKTGEN_INTERFACE_INFO 0x7450

struct pktgen_ioctl_info {
        char thread_name[32];
        char interface_name[32];
        struct pktgen_interface_info info;
};


struct pktgen_thread_info {
        struct pktgen_interface_info* running_if_infos; /* list of running interfaces, current will
                                                         * not be in this list.
                                                         */
        struct pktgen_interface_info* stopped_if_infos; /* list of stopped interfaces. */
        struct pktgen_interface_info* cur_if;           /* Current (running) interface we are servicing in
                                                         * the main thread loop.
                                                         */

        int running_if_sz;
        struct pktgen_thread_info* next;
        char name[32];
        char fname[128]; /* name of proc file */
        struct proc_dir_entry *proc_ent;
        char result[512];
        u32 max_before_softirq; /* We'll call do_softirq to prevent starvation. */
        
        spinlock_t pg_threadlock;
        
        /* Linux task structure of thread */
        struct task_struct *thread;
        
        /* Task queue need to launch thread */
        struct tq_struct tq;
        
        /* function to be started as thread */
        void (*function) (struct pktgen_thread_info *kthread);
        
        /* semaphore needed on start and creation of thread. */
        struct semaphore startstop_sem;

        /* public data */

        /* queue thread is waiting on. Gets initialized by
           init_kthread, can be used by thread itself.
        */
        wait_queue_head_t queue;
        
        /* flag to tell thread whether to die or not.
           When the thread receives a signal, it must check
           the value of terminate and call exit_kthread and terminate
           if set.
        */
        int terminate;

        int in_use; /* if 0, then we can delete or re-use this struct */
        
        /* additional data to pass to kernel thread */
        void *arg;
};/* struct pktgen_thread_info */

/* Defined in dev.c */
extern int (*handle_pktgen_hook)(struct sk_buff *skb);

/* Returns < 0 if the skb is not a pktgen buffer. */
int pktgen_receive(struct sk_buff* skb);


#endif
