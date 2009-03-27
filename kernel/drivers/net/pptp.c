/*
 *  Point-to-Point Tunneling Protocol for Linux
 *
 *	Authors: Kozlov D. (xeb@mail.ru)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	20.02.2009 - code cleanup, new call process
 *		     http://ddixlab.ru, Andrey V.Panukov
 *
 *
 *
 */


#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/ppp_channel.h>
#include <linux/ppp_defs.h>
#include <linux/if_pppox.h>
#include <linux/if_ppp.h>
#include <linux/notifier.h>
#include <linux/file.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/tqueue.h>
#include <linux/timer.h>
#include <net/sock.h>
#include <net/protocol.h>
#include <net/ip.h>
#include <net/icmp.h>
#include <net/route.h>
#include <asm/uaccess.h>

#define PPTP_DRIVER_VERSION "0.2"

MODULE_DESCRIPTION("PPTP_DRIVER");
MODULE_AUTHOR("Kozlov D. (xeb@mail.ru)");
MODULE_LICENSE("GPL");
MODULE_PARM(log_level, "i");
MODULE_PARM_DESC(log_level, "Logging level (default=0)");

static int pptp_xmit(struct ppp_channel *chan, struct sk_buff *skb);
static int pptp_ppp_ioctl(struct ppp_channel *chan, unsigned int cmd, unsigned long arg);
static int pptp_rcv_core(struct sock *sk, struct sk_buff *skb);
static int pptp_rcv(struct sk_buff *skb);
static int pptp_create(struct socket *sock);
static int pptp_release(struct socket *sock);
static int pptp_bind(struct socket *sock,struct sockaddr *uservaddr,int sockaddr_len);
static int pptp_getname(struct socket *sock, struct sockaddr *uaddr, int *usockaddr_len, int peer);
static int pptp_connect(struct socket *sock, struct sockaddr *uservaddr, int sockaddr_len, int flags);
static int pptp_ppp_ioctl(struct ppp_channel *chan, unsigned int cmd, unsigned long arg);
static void pptp_sock_destruct(struct sock *sk);

typedef struct _st_callid_list {
    struct _st_callid_list *next;
    __u32 call_id;
    __u32 free_id;
    struct pppox_opt *p_pppox;
} CALLID_LIST, *PCALLID_LIST;

static struct pppox_proto pppox_pptp_proto = {
    .create	= pptp_create
};

static struct inet_protocol net_pptp_protocol = {
    .handler  = pptp_rcv,
    .protocol = IPPROTO_GRE,
    .name     = "PPTP"
};

static struct proto_ops pptp_ops = {
    .family		= AF_PPPOX,
    .release		= pptp_release,
    .bind		= pptp_bind,
    .connect		= pptp_connect,
    .socketpair		= sock_no_socketpair,
    .accept		= sock_no_accept,
    .getname		= pptp_getname,
    .poll		= sock_no_poll,
    .listen		= sock_no_listen,
    .shutdown		= sock_no_shutdown,
    .setsockopt		= sock_no_setsockopt,
    .getsockopt		= sock_no_getsockopt,
    .sendmsg		= sock_no_sendmsg,
    .recvmsg		= sock_no_recvmsg,
    .mmap		= sock_no_mmap
};

static struct ppp_channel_ops pptp_chan_ops= {
    .start_xmit = pptp_xmit,
    .ioctl 	= pptp_ppp_ioctl
};

struct pptp_gre_header {
    u_int8_t flags;		/* bitfield */
    u_int8_t ver;		/* should be PPTP_GRE_VER (enhanced GRE) */
    u_int16_t protocol;		/* should be PPTP_GRE_PROTO (ppp-encaps) */
    u_int16_t payload_len;	/* size of ppp payload, not inc. gre header */
    u_int16_t call_id;		/* peer's call_id for this session */
    u_int32_t seq;		/* sequence number.  Present if S==1 */
    u_int32_t ack;		/* seq number of highest packet recieved by */
  				/*  sender in this session */
};


#define MAX_CALL_ID	0xFFFF
#define MISSING_WINDOW 300
#define SC_RCV_BITS	(SC_RCV_B7_1|SC_RCV_B7_0|SC_RCV_ODDP|SC_RCV_EVNP)

#define WRAPPED(curseq, lastseq) \
    ((((curseq) & 0xffffff00) == 0) && \
     (((lastseq) & 0xffffff00 ) == 0xffffff00))

#define PPTP_GRE_PROTO  0x880B
#define PPTP_GRE_VER    0x1
#define PPTP_GRE_FLAG_C	0x80
#define PPTP_GRE_FLAG_R	0x40
#define PPTP_GRE_FLAG_K	0x20
#define PPTP_GRE_FLAG_S	0x10
#define PPTP_GRE_FLAG_A	0x80
#define PPTP_GRE_IS_C(f) ((f)&PPTP_GRE_FLAG_C)
#define PPTP_GRE_IS_R(f) ((f)&PPTP_GRE_FLAG_R)
#define PPTP_GRE_IS_K(f) ((f)&PPTP_GRE_FLAG_K)
#define PPTP_GRE_IS_S(f) ((f)&PPTP_GRE_FLAG_S)
#define PPTP_GRE_IS_A(f) ((f)&PPTP_GRE_FLAG_A)
#define PPTP_HEADER_OVERHEAD (2 + sizeof(struct pptp_gre_header))

int gIsFirewallEnabled = 1;
static int log_level = 0;
static rwlock_t chan_lock = RW_LOCK_UNLOCKED;
static PCALLID_LIST call_listhead = NULL, call_listtail = NULL;

#define PPTP_LOG(l, args...) if( log_level >= l ) printk(args)

#define PPPOX_SK(sk) sk->protinfo.pppox
#define PPTP_SK(sk) ((struct pptp_opt *)&sk->protinfo.pppox->proto.pptp)
#define PPTP_PPPOX(px) ((struct pptp_opt *)&px->proto.pptp)
#define SK_PPPOX(px) px->sk

static inline struct pppox_opt *find_call(__u32 call_id, __u32 s_addr, __u32 d_addr) {
    PCALLID_LIST p_call;
	
    read_lock(&chan_lock);
    p_call = call_listhead;
    while( p_call ) 
	if( p_call->call_id == call_id &&
    	    ((s_addr != (__u32)-1 && PPTP_PPPOX(p_call->p_pppox)->dst_addr.sin_addr.s_addr == s_addr) ||
	     (d_addr != (__u32)-1 && PPTP_PPPOX(p_call->p_pppox)->dst_addr.sin_addr.s_addr == d_addr))
          ) {
		sock_hold(SK_PPPOX(p_call->p_pppox));
		break;
	} else p_call = p_call->next;
    read_unlock(&chan_lock);
    return p_call ? p_call->p_pppox : NULL;	
}

static inline __u32 add_call(struct pppox_opt *p_pppox) {
    PCALLID_LIST p_call;
    __u32 free_id = (__u32)-1;
	
    write_lock(&chan_lock);
    if( (p_call = call_listhead) ) {
        while( p_call ) 
    	    if( p_call->free_id != (__u32)-1 ) break;
	    else p_call = p_call->next;
	
	if( !p_call && call_listtail->call_id >= MAX_CALL_ID ) {
	    write_unlock(&chan_lock);
	    return (__u32)-1;
	} else if( !p_call ) {
	    free_id = call_listtail->call_id + 1;
	} else {
	    free_id = p_call->free_id;
	    p_call->free_id = (__u32)-1;
	}
    } else free_id = 1;
	    
    if( (p_call = kmalloc(sizeof(CALLID_LIST), GFP_KERNEL)) ) {
        p_call->call_id = free_id;
        p_call->free_id = (__u32)-1;
        p_call->p_pppox = p_pppox;
        p_call->next = NULL;
        PPTP_PPPOX(p_pppox)->src_addr.call_id = free_id;
	    
        if( !call_listtail ) call_listhead = call_listtail = p_call;
        else call_listtail = call_listtail->next = p_call;
    }
    write_unlock(&chan_lock);
    return p_call ? free_id : (__u32)-1;
}

static inline void delete_call(struct pppox_opt *p_pppox) {
    PCALLID_LIST p_call, *pp_call;
	
    write_lock(&chan_lock);
    pp_call = &call_listhead;
    while( (p_call = *pp_call) ) {
	if( p_call->call_id == (__u32)PPTP_PPPOX(p_pppox)->src_addr.call_id ) {
	    if( p_call == call_listtail ) {
		if( p_call == call_listhead ) call_listhead = call_listtail = NULL;
		else call_listtail = (PCALLID_LIST)pp_call;
	    }
	    *pp_call = p_call->next;
	    kfree(p_call);
	    break;
	} else pp_call = &p_call->next;
    }
    
    p_call = call_listhead;
    while( p_call ) {
	if( p_call->free_id == (__u32)-1 ) {
	    p_call->free_id = (__u32)PPTP_PPPOX(p_pppox)->src_addr.call_id;
	    break;
	} else p_call = p_call->next;
    }
    write_unlock(&chan_lock);
}

static int pptp_xmit(struct ppp_channel *chan, struct sk_buff *skb) {
    struct pptp_gre_header *hdr;
    int islcp;
    int len;
    unsigned char *data;
    __u32 seq_recv;
    struct rtable *rt;
    struct net_device *tdev;
    struct iphdr  *iph;	
    int    max_headroom;
    struct pppox_opt *po;
    struct pptp_opt *opt;
    
    struct sock *sk 		= (struct sock *)chan->private;
    unsigned int header_len 	= sizeof(*hdr);

    if( sk->dead || !(sk->state & PPPOX_CONNECTED) ) goto tx_error;
    
    po 	= PPPOX_SK(sk);
    opt	= PPTP_SK(sk);

    {   struct rt_key key = {
    	    .dst=opt->dst_addr.sin_addr.s_addr,
	    .src=opt->src_addr.sin_addr.s_addr,
	    .tos=RT_TOS(0),
	};
	if( ip_route_output_key(&rt, &key) ) goto tx_error; 
    }

    tdev = rt->u.dst.dev;
    max_headroom = ((tdev->hard_header_len + 15) & ~15) + sizeof(*iph) + sizeof(*hdr) + 2;
    
    if( skb_headroom(skb) < max_headroom || skb_cloned(skb) || skb_shared(skb) ) {
	struct sk_buff *new_skb;
	if( !(new_skb = skb_realloc_headroom(skb, max_headroom)) ) {
	    ip_rt_put(rt);
	    goto tx_error;
	}
	
	if( skb->sk ) skb_set_owner_w(new_skb, skb->sk);
	kfree_skb(skb);
	skb = new_skb;
    }

    data = skb->data;
    islcp=((data[0] << 8) + data[1]) == PPP_LCP && 1 <= data[2] && data[2] <= 7;
    if( (opt->ppp_flags & SC_COMP_PROT) && !data[0] && !islcp ) skb_pull(skb,1);
	
    if( !(opt->ppp_flags & SC_COMP_AC) || islcp ) {
	data = skb_push(skb, 2);
	data[0] = 0xff;
	data[1] = 0x03;
    }
	
    len = skb->len;
    seq_recv = opt->seq_recv;
    if( opt->ack_sent == seq_recv ) header_len -= sizeof(hdr->ack);

    skb_push(skb, header_len);
    hdr = (struct pptp_gre_header*)(skb->data);

    hdr->flags       	= PPTP_GRE_FLAG_K;
    hdr->ver         	= PPTP_GRE_VER;
    hdr->protocol    	= htons(PPTP_GRE_PROTO);
    hdr->call_id     	= htons(opt->dst_addr.call_id);
    hdr->flags 	       |= PPTP_GRE_FLAG_S;
    hdr->seq    	= htonl(++opt->seq_sent);

    if( opt->ack_sent != seq_recv ) {
	hdr->ver |= PPTP_GRE_FLAG_A;
	hdr->ack  = htonl(seq_recv);
	opt->ack_sent = seq_recv;
    }

    hdr->payload_len = htons(len);
    
    skb->h.raw = skb->nh.raw;
    skb->nh.raw = skb_push(skb, sizeof(*iph));

    memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
    iph 		= skb->nh.iph;
    iph->version	= 4;
    iph->ihl		= sizeof(struct iphdr) >> 2;
    iph->frag_off	= 0;
    iph->protocol	= IPPROTO_GRE;
    iph->tos		= 0;
    iph->daddr		= rt->rt_dst;
    iph->saddr		= rt->rt_src;
    iph->ttl 		= sysctl_ip_default_ttl - 1;
    iph->tot_len 	= htons(skb->len);

    dst_release(skb->dst);
    skb->dst = &rt->u.dst;

#ifdef CONFIG_NETFILTER
    nf_conntrack_put(skb->nfct);
    skb->nfct = NULL;
#ifdef CONFIG_NETFILTER_DEBUG
    skb->nf_debug = 0;
#endif
#endif /* CONFIG_NETFILTER */

    skb->ip_summed = CHECKSUM_NONE;
    ip_select_ident(iph, &rt->u.dst, NULL);
    ip_send_check(iph);
	
    //NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, skb, NULL, rt->u.dst.dev, ip_send);
    ip_send(skb);

tx_error:
    return 1;
}

static int pptp_rcv_core(struct sock *sk,struct sk_buff *skb) {
    int headersize, payload_len, seq;
    __u8 *payload;
    struct pptp_gre_header *header;
    struct pppox_opt *po;
    struct pptp_opt *opt;

    if( !(sk->state & PPPOX_CONNECTED) ) {
	if (sock_queue_rcv_skb(sk, skb)) goto drop;
	return NET_RX_SUCCESS;
    }
    
    po = PPPOX_SK(sk);
    opt = PPTP_SK(sk);
	
    header = (struct pptp_gre_header *)(skb->data);

    if( PPTP_GRE_IS_A(header->ver) ) { 
	__u32 ack = (PPTP_GRE_IS_S(header->flags)) ? header->ack : header->seq;
	ack = ntohl( ack);
	if( ack > opt->ack_recv ) opt->ack_recv = ack;
	if( WRAPPED(ack, opt->ack_recv) ) opt->ack_recv = ack;
    }

    if( !PPTP_GRE_IS_S(header->flags) ) goto drop;

    headersize  = sizeof(*header);
    payload_len = ntohs(header->payload_len);
    seq         = ntohl(header->seq);

    if( !PPTP_GRE_IS_A(header->ver) ) headersize -= sizeof(header->ack);

    if( skb->len - headersize < payload_len )  goto drop;
    if( seq - opt->seq_recv > MISSING_WINDOW ) goto drop;
	
    payload = skb->data + headersize;
    if( !opt->first_seq && (seq < opt->seq_recv + 1 || WRAPPED(opt->seq_recv, seq)) ) {
	PPTP_LOG(1, KERN_INFO"PPTP: Discarding packet: seq: %X seq_recv: %X\n", seq, opt->seq_recv);
    } else {
	opt->first_seq = 0;
	opt->seq_recv = seq;
	skb_pull(skb, headersize);

	if( payload[0] == PPP_ALLSTATIONS && payload[1] == PPP_UI ){
    	    if( skb->len < 3 ) goto drop;
	    skb_pull(skb, 2);
	}

	if( (*skb->data) & 1) skb_push(skb, 1)[0] = 0;

	skb->ip_summed = CHECKSUM_NONE;
	ppp_input(&po->chan, skb);
	return NET_RX_SUCCESS;
    }
drop:
    kfree_skb(skb);
    return NET_RX_DROP;
}

static int pptp_rcv(struct sk_buff *skb) {
    struct pppox_opt *po;
    struct pptp_gre_header *header;
    struct iphdr *iph;
    int error;
    struct sock *sk;

    if( skb->pkt_type != PACKET_HOST ) 	goto drop;
    if( !pskb_may_pull(skb, 12) ) 	goto drop;

    iph = skb->nh.iph;
    header = (struct pptp_gre_header *)skb->data;

    if( ((header->ver & 0x7F) != PPTP_GRE_VER ) ||
	( ntohs(header->protocol) != PPTP_GRE_PROTO ) ||
	  PPTP_GRE_IS_C(header->flags) ||
	  PPTP_GRE_IS_R(header->flags) ||
	 !PPTP_GRE_IS_K(header->flags) ||
	( header->flags & 0xF ) ) {
	    PPTP_LOG(1, KERN_INFO"PPTP: Discarding GRE: %X %X %X %X %X %X\n", \
		header->ver & 0x7F, ntohs(header->protocol), \
		PPTP_GRE_IS_C(header->flags), \
		PPTP_GRE_IS_R(header->flags), \
		PPTP_GRE_IS_K(header->flags), \
		header->flags & 0xF); 
	    goto drop;
    }

    if( (po = find_call(htons(header->call_id), iph->saddr, (__u32)-1)) ) {
	dst_release(skb->dst);
	skb->dst = NULL;

#ifdef CONFIG_NETFILTER
        nf_conntrack_put(skb->nfct);
	skb->nfct = NULL;
#ifdef CONFIG_NETFILTER_DEBUG
	skb->nf_debug = 0;
#endif
#endif /* CONFIG_NETFILTER */

	sk = SK_PPPOX(po);
    	    
    	bh_lock_sock(sk);
	if( sk->lock.users != 0 ) {
	    sk_add_backlog(sk, skb);
	    error = NET_RX_SUCCESS;
	} else error = pptp_rcv_core(sk, skb);
	bh_unlock_sock(sk);

	sock_put(sk);
	return error;
    } else icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);
drop:
    kfree_skb(skb);
    return NET_RX_DROP;
}

static int pptp_bind(struct socket *sock,struct sockaddr *uservaddr, int sockaddr_len) {
    struct sock *sk 		= sock->sk;
    struct sockaddr_pppox *sp 	= (struct sockaddr_pppox*)uservaddr;
    int error;

    lock_sock(sk);

    PPTP_SK(sk)->src_addr = sp->sa_addr.pptp;
    if( add_call(PPPOX_SK(sk)) == (__u32)-1 ) error = -EBUSY;
    else error = 0;

    release_sock(sk);

    return error;
}

static int pptp_connect(struct socket *sock, struct sockaddr *uservaddr, int sockaddr_len, int flags) {
    struct sock *sk 		= sock->sk;
    struct sockaddr_pppox *sp 	= (struct sockaddr_pppox*)uservaddr;
    struct pppox_opt *po 	= PPPOX_SK(sk);
    struct pptp_opt *opt 	= PPTP_SK(sk);
    struct rtable *rt;     			
    int error;

    if( sp->sa_protocol != PX_PROTO_PPTP ) return -EINVAL;
    if( find_call((__u32)sp->sa_addr.pptp.call_id, (__u32)-1, sp->sa_addr.pptp.sin_addr.s_addr) ) return -EALREADY;

    lock_sock(sk);
    if( sk->state & (PPPOX_CONNECTED | PPPOX_DEAD) ) {
	error = sk->state & PPPOX_DEAD ? -EALREADY : -EBUSY;
	goto end;
    }

    if( !opt->src_addr.sin_addr.s_addr || !sp->sa_addr.pptp.sin_addr.s_addr ){
	error = -EINVAL;
	goto end;
    }

    po->chan.private 	= sk;
    po->chan.ops	= &pptp_chan_ops;

    { 	struct rt_key key = {
	    .dst=opt->dst_addr.sin_addr.s_addr,
	    .src=opt->src_addr.sin_addr.s_addr,
	    .tos=RT_TOS(0),
	};
	
	if( ip_route_output_key(&rt, &key) ) {
    	    error = -EHOSTUNREACH;
	    goto end;
	}
    }

    if( !(po->chan.mtu = rt->u.dst.pmtu) ) po->chan.mtu = PPP_MTU;
    po->chan.mtu -= PPTP_HEADER_OVERHEAD;

    po->chan.hdrlen = 2 + sizeof(struct pptp_gre_header);
    if( (error = ppp_register_channel(&po->chan)) ) {
	PPTP_LOG(0, KERN_ERR "PPTP: failed to register PPP channel (%d)\n",error);
	goto end;
    }

    opt->dst_addr = sp->sa_addr.pptp;
    sk->state  = PPPOX_CONNECTED;

    error = 0;
end:
    release_sock(sk);
    return error;
}

static int pptp_getname(struct socket *sock, struct sockaddr *uaddr, int *usockaddr_len, int peer) {
    struct sockaddr_pppox sp;
    
    sp.sa_family	= AF_PPPOX;
    sp.sa_protocol	= PX_PROTO_PPTP;
    sp.sa_addr.pptp	= PPTP_SK(sock->sk)->src_addr;

    memcpy(uaddr, &sp, sizeof(struct sockaddr_pppox));
    *usockaddr_len = sizeof(struct sockaddr_pppox);
    return 0;
}

static int pptp_release(struct socket *sock) {
    struct sock *sk = sock->sk;

    if( !sk ) return 0;
    
    lock_sock(sk);
    if( !sk->dead ) {
        delete_call(PPPOX_SK(sk));
    
	if( sk->state & PPPOX_CONNECTED ) pppox_unbind_sock(sk);	
	sk->state = PPPOX_DEAD;

        sock_orphan(sk);
	sock->sk = NULL;

        skb_queue_purge(&sk->receive_queue);
        
        release_sock(sk);
        sock_put(sk);
        return 0;
    } else {
	release_sock(sk);
	return -EBADF;
    }
}

static void pptp_sock_destruct(struct sock *sk) {
    if( sk->protinfo.destruct_hook ) kfree(sk->protinfo.destruct_hook);
    MOD_DEC_USE_COUNT;
}

static int pptp_create(struct socket *sock) {
    struct sock *sk;

    MOD_INC_USE_COUNT;

    if( !(sk = sk_alloc(PF_PPPOX, GFP_KERNEL, 1)) ) return -ENOMEM;

    sock_init_data(sock, sk);

    sock->state = SS_UNCONNECTED;
    sock->ops   = &pptp_ops;

    sk->state	   = PPPOX_NONE;
    sk->type	   = SOCK_STREAM;
    sk->family	   = PF_PPPOX;
    sk->protocol   = PX_PROTO_PPTP;

    if( !(sk->protinfo.pppox = (struct pppox_opt*)kmalloc(sizeof(struct pppox_opt), GFP_KERNEL)) ) {
	sk_free(sk);
	return -ENOMEM;
    }  

    memset(sk->protinfo.pppox, 0, sizeof(struct pppox_opt));

    sk->destruct = pptp_sock_destruct;
    sk->protinfo.destruct_hook = sk->protinfo.pppox;
	
    PPPOX_SK(sk)->sk = sk;
    PPTP_SK(sk)->first_seq = 1;

    return 0;
}

static int pptp_ppp_ioctl(struct ppp_channel *chan, unsigned int cmd, unsigned long arg) {
    struct sock *sk = (struct sock*)chan->private;
    struct pptp_opt *opt = PPTP_SK(sk);
    int error, val;

    error = -EFAULT;
    switch( cmd ) {
    case PPPIOCGFLAGS:
	if( put_user(opt->ppp_flags, (int*)arg) ) break;
	error = 0;
	break;
    case PPPIOCSFLAGS:
	if( get_user(val, (int*)arg) ) break;
	opt->ppp_flags = val & ~SC_RCV_BITS;
	error = 0;
	break;
    default:
	error = -ENOTTY;
    }

    return error;
}

static int pptp_init_module(void) {
    int error;
    printk(KERN_INFO "PPTP driver, v."PPTP_DRIVER_VERSION"\n");

    inet_add_protocol(&net_pptp_protocol);
    if( (error = register_pppox_proto(PX_PROTO_PPTP, &pppox_pptp_proto)) ) {
        inet_del_protocol(&net_pptp_protocol);
        printk(KERN_INFO "PPTP: can't register pppox_proto, error: %d\n", error);
    }
    return error;
}

static void pptp_exit_module(void) {
    unregister_pppox_proto(PX_PROTO_PPTP);
    inet_del_protocol(&net_pptp_protocol);
}

module_init(pptp_init_module);
module_exit(pptp_exit_module);
