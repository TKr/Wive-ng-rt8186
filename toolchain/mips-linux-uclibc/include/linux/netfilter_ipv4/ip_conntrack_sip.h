#ifndef _IP_CONNTRACK_SIP_H
#define _IP_CONNTRACK_SIP_H
/* SIP tracking. */
/* Created by Kao */

#ifdef __KERNEL__

#include <linux/netfilter_ipv4/lockhelp.h>

/* Protects sip part of conntracks */
DECLARE_LOCK_EXTERN(ip_sip_lock);
int sip_find_via_ipport (const struct iphdr *iph, u_int32_t *vip, u_int16_t *vport);

#define SIP_PORT	5060

#endif /* __KERNEL__ */

enum ip_ct_sip_type
{
	/* Expectation from caller */
	IP_CT_SIP_CALLER,
	/* Expectation from callee */
	IP_CT_SIP_CALLEE,
};

enum ip_ct_sip_expect_type
{
	/* Expect for SIP */
	IP_CT_EXPECT_SIP,
	/* Expect for RTP */
	IP_CT_EXPECT_RTP
};

/* This structure is per expected connection */
struct ip_ct_sip_expect
{
	/* We record sip expected connection type here. */

	enum ip_ct_sip_type siptype;	/* CALLER or CALLEE ? */
	enum ip_ct_sip_expect_type exptype; /* SIP or RTP expectation ? */
	u_int16_t port; 		/* UDP port that was to be used */
};

/* This structure exists only once per master */
struct ip_ct_sip_master {
	/* Incoming SIP should be expected to our port from arbitary port */
	int sip_expected;
};

#endif /* _IP_CONNTRACK_SIP_H */
