/*
 * @(#) Definitions of IPsec Security Association (ipsec_sa)
 *
 * Copyright (C) 2001  Richard Guy Briggs  <rgb@freeswan.org>
 *                 and Michael Richardson  <mcr@freeswan.org>
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * RCSID $Id: ipsec_sa.h,v 1.8 2002/07/28 22:59:53 mcr Exp $
 *
 * This file derived from ipsec_xform.h on 2001/9/18 by mcr.
 *
 */

/* 
 * This file describes the IPsec Security Association Structure.
 *
 * This structure keeps track of a single transform that may be done
 * to a set of packets. It can describe applying the transform or
 * apply the reverse. (e.g. compression vs expansion). However, it
 * only describes one at a time. To describe both, two structures would
 * be used, but since the sides of the transform are performed 
 * on different machines typically it is usual to have only one side
 * of each association.
 * 
 */

#ifndef _IPSEC_SA_H_

#include "ipsec_stats.h"
#include "ipsec_life.h"
#include "ipsec_eroute.h"
#include "ipsec_param.h"

/*
 * SA reference table is two-stage.
 * One 8-bit table points to 2^8 16-bit tables with 2^16 entries.
 * The 16-bit tables are dynamically allocated as needed.
 *
 * At least one 16-bit table will be allocated. 
 *
 * The first 8 bits correspond to the 8 most significant bits of the
 * skb->nf_mark, the next 16 bits correspond to the middle 16 bits of
 * the skb->nf_mark.
 *
 * We pick the upper bits on the theory that they are less likely to
 * interfere with more pedestrian uses of nfmark.
*/

#if ((SA_REF_TABLE_MAX_WIDTH - SA_REF_MAINTABLE_WIDTH) < 0)
#error "SA_REF_TABLE_MAX_WIDTH="SA_REF_TABLE_MAX_WIDTH" MUST be < SA_REF_MAINTABLE_WIDTH="SA_REF_MAINTABLE_WIDTH
#endif

#define SA_REF_MAINTABLE_SIZE (2^SA_REF_MAINTABLE_WIDTH)
#define SA_REF_SUBTABLE_SIZE (2^(SA_REF_TABLE_MAX_WIDTH - SA_REF_MAINTABLE_WIDTH))

#define SA_REF_FIELD_WIDTH sizeof(((struct sk_buff*)NULL)->nfmark)*8

#define SAref2SArefTable(x) x>>(SA_REF_FIELD_WIDTH - SA_REF_MAINTABLE_WIDTH)
#define SAref2SArefEntry(x) x>>(SA_REF_FIELD_WIDTH - SA_REF_TABLE_MAX_WIDTH)

#define SAref2SAentry(x) SAref[SAref2SArefTable(x)]->entry[SAref2SArefTable(x)]
#define SAref2SA(x) SAref[SAref2SArefTable(x)]->entry[SAref2SArefTable(x)].SA
#define SA2SAref(x) x->ips_ref

struct SArefTableEntry
{
	struct ipsec_sa* SA;
	unsigned SAref;
};

struct SArefSubTable
{
	struct SArefTableEntry entry[SA_REF_SUBTABLE_SIZE];
};

extern struct SArefSubTable* SAref[SA_REF_MAINTABLE_SIZE];
extern struct SArefTableEntry * SArefFreeList;
extern struct SArefTableEntry * SArefFreeListTail;

/* 'struct ipsec_sa' should be 64bit aligned when allocated. */
struct ipsec_sa 	                        
{
	unsigned	ips_ref;		/* reference table entry number */
	atomic_t	ips_refcount;		/* reference count for this struct */
	struct ipsec_sa	*ips_hnext;		/* next in hash chain */
	struct ipsec_sa	*ips_inext;	 	/* pointer to next xform */
	struct ipsec_sa	*ips_onext;	 	/* pointer to prev xform */

	struct ifnet	*ips_rcvif;	 	/* related rcv encap interface */

	struct sa_id	ips_said;	 	/* SA ID */

	__u32		ips_seq;		/* seq num of msg that initiated this SA */
	__u32		ips_pid;		/* PID of process that initiated this SA */
	__u8		ips_authalg;		/* auth algorithm for this SA */
	__u8		ips_encalg;		/* enc algorithm for this SA */

	struct ipsec_stats ips_errs;

	__u8		ips_replaywin;		/* replay window size */
	__u8		ips_state;		/* state of SA */
	__u32		ips_replaywin_lastseq;	/* last pkt sequence num */
	__u64		ips_replaywin_bitmap;	/* bitmap of received pkts */
	__u32		ips_replaywin_maxdiff;	/* max pkt sequence difference */

	__u32		ips_flags;		/* generic xform flags */


	struct ipsec_lifetimes ips_life;	/* lifetime records */

	/* selector information */
	struct sockaddr*ips_addr_s;		/* src sockaddr */
	struct sockaddr*ips_addr_d;		/* dst sockaddr */
	struct sockaddr*ips_addr_p;		/* proxy sockaddr */
	__u16		ips_addr_s_size;
	__u16		ips_addr_d_size;
	__u16		ips_addr_p_size;
	ip_address	ips_flow_s;
	ip_address	ips_flow_d;
	ip_address	ips_mask_s;
	ip_address	ips_mask_d;

	__u16		ips_key_bits_a;		/* size of authkey in bits */
	__u16		ips_auth_bits;		/* size of authenticator in bits */
	__u16		ips_key_bits_e;		/* size of enckey in bits */
	__u16		ips_iv_bits;	 	/* size of IV in bits */
	__u8		ips_iv_size;
	__u16		ips_key_a_size;
	__u16		ips_key_e_size;

	caddr_t		ips_key_a;		/* authentication key */
	caddr_t		ips_key_e;		/* encryption key */
	caddr_t	        ips_iv;			/* Initialisation Vector */

	struct ident	ips_ident_s;		/* identity src */
	struct ident	ips_ident_d;		/* identity dst */

#ifdef CONFIG_IPSEC_IPCOMP
	__u16		ips_comp_adapt_tries;	/* ipcomp self-adaption tries */
	__u16		ips_comp_adapt_skip;	/* ipcomp self-adaption to-skip */
	__u64		ips_comp_ratio_cbytes;	/* compressed bytes */
	__u64		ips_comp_ratio_dbytes;	/* decompressed (or uncompressed) bytes */
#endif /* CONFIG_IPSEC_IPCOMP */

#if 0
	__u32		ips_sens_dpd;
	__u8		ips_sens_sens_level;
	__u8		ips_sens_sens_len;
	__u64*		ips_sens_sens_bitmap;
	__u8		ips_sens_integ_level;
	__u8		ips_sens_integ_len;
	__u64*		ips_sens_integ_bitmap;
#endif
};

extern int ipsec_SArefSubTable_alloc(unsigned table);
extern int ipsec_sadb_init(void);
extern struct ipsec_sa *ipsec_sa_alloc(void);
extern struct SArefTableEntry *ipsec_SAref_alloc(void);
extern int ipsec_sa_free(struct ipsec_sa* ips);
extern struct ipsec_sa *ipsec_sa_getbyid(struct sa_id *said);
extern int ipsec_sa_put(struct ipsec_sa *ips);
extern int ipsec_sa_add(struct ipsec_sa *ips);
extern int ipsec_sa_del(struct ipsec_sa *ips);
extern int ipsec_sa_delchain(struct ipsec_sa *ips);
extern int ipsec_sadb_cleanup(__u8 proto);
extern int ipsec_sa_wipe(struct ipsec_sa *ips);

enum ipsec_direction {
	ipsec_incoming = 1,
	ipsec_outgoing = 2
};

#define _IPSEC_SA_H
#endif /* _IPSEC_SA_H_ */

/*
 * $Log: ipsec_sa.h,v $
 * Revision 1.8  2002/07/28 22:59:53  mcr
 * 	clarified/expanded one comment.
 *
 * Revision 1.7  2002/07/26 08:48:31  rgb
 * Added SA ref table code.
 *
 * Revision 1.6  2002/05/31 17:27:48  rgb
 * Comment fix.
 *
 * Revision 1.5  2002/05/27 18:55:03  rgb
 * Remove final vistiges of tdb references via IPSEC_KLIPS1_COMPAT.
 *
 * Revision 1.4  2002/05/23 07:13:36  rgb
 * Convert "usecount" to "refcount" to remove ambiguity.
 *
 * Revision 1.3  2002/04/24 07:36:47  mcr
 * Moved from ./klips/net/ipsec/ipsec_sa.h,v
 *
 * Revision 1.2  2001/11/26 09:16:15  rgb
 * Merge MCR's ipsec_sa, eroute, proc and struct lifetime changes.
 *
 * Revision 1.1.2.1  2001/09/25 02:24:58  mcr
 * 	struct tdb -> struct ipsec_sa.
 * 	sa(tdb) manipulation functions renamed and moved to ipsec_sa.c
 * 	ipsec_xform.c removed. header file still contains useful things.
 *
 *
 * Local variables:
 * c-file-style: "linux"
 * End:
 *
 */
