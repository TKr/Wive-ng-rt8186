/*
 *		IP_NAT_SIP	- SIP extension for UDP NAT alteration
 *
 */

#include <linux/module.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <net/tcp.h>
#include <linux/netfilter_ipv4/ip_nat.h>
#include <linux/netfilter_ipv4/ip_nat_helper.h>
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#include <linux/netfilter_ipv4/ip_conntrack_sip.h>
#include <linux/netfilter_ipv4/ip_nat_sip.h>
#include <linux/proc_fs.h>

#if 0 
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

#define MAX_PORTS 8
static int ports[MAX_PORTS];
static int ports_c = 0;
static char flag='1';

#define ISALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define ISDIGIT(c) (((c) >= '0') && ((c) <= '9'))

enum ip_sip_mangle_type
{
	IP_SIP_MANGLE_SRC,
	IP_SIP_MANGLE_DST,
	IP_SIP_MANGLE_NONE
};

DECLARE_LOCK_EXTERN(ip_sip_lock);


/**
 * called whenever the first packet of a related connection arrives
 */
static unsigned int
sip_nat_expected(struct sk_buff **pskb,
		 unsigned int hooknum,
		 struct ip_conntrack *ct,
		 struct ip_nat_info *info)
{
	struct ip_nat_multi_range mr;
	u_int32_t newdstip, newsrcip, newip;
	struct ip_ct_sip_expect *exp_sip_info;
	struct ip_conntrack *master = master_ct(ct);
	
	if(flag=='0')
		return NF_ACCEPT;
	
	IP_NF_ASSERT(info);
	IP_NF_ASSERT(master);

	IP_NF_ASSERT(!(info->initialized & (1<<HOOK2MANIP(hooknum))));

	DEBUGP("sip_nat_expected: We have a connection!\n");
	DEBUGP("sip_nat_expected: %u.%u.%u.%u:%u->%u.%u.%u.%u:%u\n",
		NIPQUAD(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip),
		ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
		NIPQUAD(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip),
		ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port));
	
	exp_sip_info = &ct->master->help.exp_sip_info;
	
	if (exp_sip_info->exptype == IP_CT_EXPECT_SIP) {
		if (master) {
			// Copy the original via (ip, port) from
			//   master conntrack.
			ct->nat.help.nat_sip_info.via_ip =
				master->nat.help.nat_sip_info.via_ip;
			ct->nat.help.nat_sip_info.via_port =
				master->nat.help.nat_sip_info.via_port;
		}
		
		// It's a SIP expectation
		LOCK_BH(&ip_sip_lock);
		if (hooknum == NF_IP_PRE_ROUTING) {
			// Let nat helper to run for this expectation.
			ct->nat.info.helper = master->nat.info.helper;
		}
		
		UNLOCK_BH(&ip_sip_lock);
	}
	
	LOCK_BH(&ip_sip_lock);
	
	if (exp_sip_info->siptype == IP_CT_SIP_CALLEE)
	{
		/* From callee: make connection go to the caller. */
		newdstip = master->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;
		newsrcip = master->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip;
		DEBUGP("sip_nat_expected: From callee. %u.%u.%u.%u->%u.%u.%u.%u\n",
		       NIPQUAD(newsrcip), NIPQUAD(newdstip));
	} else {
		/* From caller: make the connection go to the callee */
		newdstip = master->tuplehash[IP_CT_DIR_REPLY].tuple.src.ip;
		newsrcip = master->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
		DEBUGP("sip_nat_expected: From caller. %u.%u.%u.%u->%u.%u.%u.%u\n",
		       NIPQUAD(newsrcip), NIPQUAD(newdstip));
	}
	
	UNLOCK_BH(&ip_sip_lock);
	
	if (HOOK2MANIP(hooknum) == IP_NAT_MANIP_SRC)
		newip = newsrcip;
	else
		newip = newdstip;

	DEBUGP("sip_nat_expected: IP to %u.%u.%u.%u\n", NIPQUAD(newip));

	mr.rangesize = 1;
	/* We don't want to manip the per-protocol, just the IPs... */
	mr.range[0].flags = IP_NAT_RANGE_MAP_IPS;
	mr.range[0].min_ip = mr.range[0].max_ip = newip;

	/* ... unless we're doing a MANIP_DST, in which case, make
	   sure we map to the correct port */
	if (HOOK2MANIP(hooknum) == IP_NAT_MANIP_DST) {
		mr.range[0].flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
		mr.range[0].min = mr.range[0].max
			= ((union ip_conntrack_manip_proto)
				{ htons(exp_sip_info->port) });
	}
	return ip_nat_setup_info(ct, &mr, hooknum);
}

/**
 * searches for the body of a sip message and returns the position in the message
 */
static int sip_find_body_start (char *data, int len)
{
	char *orig_data = data;
	char *data_limit = data + len;

	while (data < data_limit)
	{
		if (!memcmp(data,"\n\n",2) || !memcmp(data,"\n\r",2)) {
			data += 2;
			return (data - orig_data);
		}

		if (!memcmp(data,"\r\n\r\n",4)) {
			data += 4;
			return (data - orig_data);
		}

		data++;
	}

	return 0;
}

static int 
ip_nat_mangle_udp_packetX(struct sk_buff **skb,
			 unsigned int match_offset,
			 unsigned int match_len,
			 char *rep_buffer,
			 unsigned int rep_len)
{
	struct iphdr *iph = (*skb)->nh.iph;
	struct udphdr *udph;
	unsigned char *data;
	u_int32_t udplen, newlen, newudplen;

	udplen = (*skb)->len - iph->ihl*4;
	newudplen = udplen - match_len + rep_len;
	newlen = iph->ihl*4 + newudplen;

	if (newlen > 65535) {
		if (net_ratelimit())
			printk("ip_nat_mangle_udp_packet: nat'ed packet "
				"exceeds maximum packet size\n");
		return 0;
	}

	if ((*skb)->len != newlen) {
		if (newlen > (*skb)->len + skb_tailroom(*skb))
		{
			struct sk_buff *newskb;
			newskb = skb_copy_expand(*skb, skb_headroom(*skb),
						 newlen - (*skb)->len,
						 GFP_ATOMIC);
        		
			if (!newskb) {
				printk("resize packet failed!!\n");
				return 0;
			} else {
				kfree_skb(*skb);
				*skb = newskb;
			}
		}
	}

	/* Alexey says: if a hook changes _data_ ... it can break
	   original packet sitting in tcp queue and this is fatal */
	if (skb_cloned(*skb)) {
		struct sk_buff *nskb = skb_copy(*skb, GFP_ATOMIC);
		if (!nskb) {
			if (net_ratelimit())
				printk("Out of memory cloning UDP packet\n");
			return 0;
		}
		/* Rest of kernel will get very unhappy if we pass it
		   a suddenly-orphaned skbuff */
		if ((*skb)->sk)
			skb_set_owner_w(nskb, (*skb)->sk);
		kfree_skb(*skb);
		*skb = nskb;
	}

	/* skb may be copied !! */
	iph = (*skb)->nh.iph;
	udph = (void *)iph + iph->ihl*4;
	data = (char *)udph + sizeof(struct udphdr);

	/* move post-replacement */
	memmove(data + match_offset + rep_len,
		 data + match_offset + match_len,
		 (*skb)->tail - (data + match_offset + match_len));

	/* insert data from buffer */
	memcpy(data + match_offset, rep_buffer, rep_len);

	/* update skb info */
	if (newlen > (*skb)->len) {
		DEBUGP("ip_nat_mangle_udp_packet: Extending packet by "
			"%u to %u bytes\n", newlen - (*skb)->len, newlen);
		skb_put(*skb, newlen - (*skb)->len);
	} else {
		DEBUGP("ip_nat_mangle_udp_packet: Shrinking packet from "
			"%u to %u bytes\n", (*skb)->len, newlen);
		skb_trim(*skb, newlen);
	}

	/* fix checksum information */

/*
	iph->tot_len = htons(newlen);
	udph->len = newudplen;
	udph->check = 0;
	(*skb)->csum = csum_partial((char *)udph, ntohs(udph->len), 0);
	
	udph->check = csum_tcpudp_magic(iph->saddr, iph->daddr,ntohs(udph->len),IPPROTO_UDP, (*skb)->csum);
	ip_send_check(iph);
*/

	return 1;
}

/**
 * mangles the topmost via header to contain the masq host and port
 */
static int masq_sip_mangle_via (struct sk_buff **skb_p, char *buf, unsigned buf_len)
{
	struct sk_buff *skb;
	struct iphdr *iph;
	struct udphdr *uh;
	char *data;
	char *data_limit;
	char *addrstart;
	int diff = 0;

	/* this is our current buffer - do we need this? */
	skb = *skb_p;
	iph = skb->nh.iph;
	uh = (struct udphdr *)&(((char *)iph)[iph->ihl*4]);

	/* lets keep the original around please */
	data = (char *)uh + sizeof(struct udphdr); // sip message

	/* We can search the packet up until here, being overly safe for now */
	data_limit = skb->h.raw + skb->len;

	/* mod up the via */
	while (data < data_limit)
	{
		/* find the topmost tag */
		if (strnicmp(data,"\nvia:",5) && strnicmp(data,"\nv:",3)
			&& strnicmp(data,"\rvia:",5) && strnicmp(data,"\rv:",3))
		{
			data++;
			continue;
		}

		DEBUGP("masq_sip: Found via header\n");

		/* Look for UDP, since this masq module only does udp anyways */
		while (*data!='U' && *data!='u')
		{
			if (data==data_limit)
				return 0;
			data++;
		}
		data += 3;

		if (data>=data_limit)
			return 0;

		/* skip white space */
		while (*data==' ')
		{
			if (data==data_limit)
				return 0;
			data++;
		}

		/* grab the ip address */
		addrstart = data;

		/* this accepts FQDNs or dotted quads */
		while (ISALPHA(*data) || ISDIGIT(*data) || (*data=='.') || (*data=='-'))
		{
			data++;
			if (data==data_limit)
				return 0;
		}

		/* skip the port information too, if any */
		if (*data==':') {
			data++;
			if (data==data_limit)
				return 0;

			while (ISDIGIT(*data))
			{
				data++;
				if (data==data_limit)
					return 0;
			}
		}

		/* replace the skb */
		if (!ip_nat_mangle_udp_packetX(skb_p,
			(int)addrstart - (int)uh - sizeof(struct udphdr),
			data-addrstart, buf, buf_len))
			return 0;

		/* we're supposed to return this */
		diff += (buf_len - (data-addrstart));
		DEBUGP("masq_sip: Via header replaced\n");

		break;
	}

	return diff;
}

/**
 * mangle the contact header to have the ipaddress be the masq addr
 */
static int masq_sip_mangle_contact (struct sk_buff **skb_p, char *buf, unsigned buf_len)
{
	struct sk_buff *skb;
	struct iphdr *iph;
	struct udphdr *uh;
	char *data;
	char *data_limit;
	char *starturi;
	char *addrstart;
	int diff = 0;

	/* this is our current buffer - do we need this? */
	skb = *skb_p;
	iph = skb->nh.iph;
	uh = (struct udphdr *)&(((char *)iph)[iph->ihl*4]);

	data = (char *)uh + sizeof(struct udphdr); // sip message

	/* We can search the packet up until here, being overly safe for now */
	data_limit = skb->h.raw + skb->len;

	/* mod up the contact */
	while (data < data_limit)
	{
		/* find the topmost tag */
		if (strnicmp(data,"\ncontact:",9) && strnicmp(data,"\nm:",3)
			&& strnicmp(data,"\rcontact:",9) && strnicmp(data,"\rm:",3))
		{
			data++;
			continue;
		}

		DEBUGP("masq_sip: Found contact header\n");

		/* Look for sip: */
		while (strnicmp(data,"sip:",4))
		{
			if (data == data_limit)
				break;
			data++;
		}
		data += 4;

		/* we have to look to see if there's user info in the contact header */
		starturi = data;

		while (*data!='@' && *data!='>' && *data!=';' && *data!='\n' && *data!='\r' && *data!='?' && *data!=',')
		{
			if (data == data_limit)
				break;
			data++;
		}

		/* check for userinfo */
		if (*data=='@') {
			/* woop! */
			data++;
		} else {
			data = starturi;
		}

		/* we should be fine now */
		addrstart = data;

		/* this accepts FQDNs or dotted quads */
		while (ISALPHA(*data) || ISDIGIT(*data) || (*data=='.') || (*data=='-'))
		{
			data++;
			if (data == data_limit)
				break;
		}

		/* skip the port information too, if any */
		if (*data==':') {
			data++;
			while (ISDIGIT(*data))
				data++;
		}

		/* replace the skb */
		if (!ip_nat_mangle_udp_packetX(skb_p,
			(int)addrstart - (int)uh - sizeof(struct udphdr),
			data-addrstart, buf, buf_len))
			return 0;

		/* we're supposed to return this */
		diff += (buf_len - (data-addrstart));

		DEBUGP("masq_sip: Contact tag replaced\n");
		break;
	}

	return diff;
}

/**
 * update the content length header
 */
int masq_sip_mangle_content_length (struct sk_buff **skb_p, int newlength)
{
	struct sk_buff *skb;
	struct iphdr *iph;
	struct udphdr *uh;
	char *data;
	char *data_limit;
	char lenbuf[32];
	unsigned lenbuf_len;
	char *p;
	unsigned p1;
	int diff = 0;

	/* make a buffer for the new length */
	sprintf(lenbuf,"%d",newlength);
	lenbuf_len = strlen(lenbuf);

	/* this is our current buffer - do we need this? */
	skb = *skb_p;
	iph = skb->nh.iph;
	uh = (struct udphdr *)&(((char *)iph)[iph->ihl*4]);

	data = (char *)uh + sizeof(struct udphdr); // sip message

	/* We can search the packet up until here, being overly safe for now */
	data_limit = skb->h.raw + skb->len;

	/* mod up the contact */
	while (data < data_limit)
	{
		/* find the topmost tag */
		if (strnicmp(data,"\ncontent-length:",16) && strnicmp(data,"\nl:",3)
			&& strnicmp(data,"\rcontent-length:",16) && strnicmp(data,"\rl:",3))
		{
			data++;
			continue;
		}

		DEBUGP("masq_sip: Found content-length header\n");

		while (*data!=':')
		{
			if (data == data_limit)
				break;
			data++;
		}
		data++;

		while (*data==' ')
		{
			if (data == data_limit)
				break;
			data++;
		}

		p = data;
		p1 = simple_strtoul(data,&data,20);

		/* replace the skb */
		if (!ip_nat_mangle_udp_packetX(skb_p,
			(int)p - (int)uh - sizeof(struct udphdr),
			data-p, lenbuf, lenbuf_len))
			return 0;
		
		/* we're supposed to return this */
		diff += (lenbuf_len - (data-p));
		
		DEBUGP("masq_sip: Content-length tag replaced\n");
		break;
	}

	return diff;
}

/**
 * create a masq entry and change the sdp to reflect it
 */
int masq_sip_mangle_sdp_out (struct sk_buff **skb_p, __u32 maddr, char *buf,
		unsigned buf_len, enum ip_conntrack_info ctinfo,
		struct ip_conntrack *ct, struct ip_conntrack_expect *expect)
{
	struct sk_buff *skb;
	struct iphdr *iph;
	struct udphdr *uh;
	char *sipdata, *data;
	char *data_limit;
	char *addrstart;
	char *portstart;
	char portbuf[7];
	unsigned portbuf_len;
	unsigned port;
	u_int16_t mport;
	struct ip_conntrack_tuple newtuple;
	int dir = CTINFO2DIR(ctinfo);;
	int diff = 0;

	/* this is our current buffer - do we need this? */
	skb = *skb_p;
	iph = skb->nh.iph;
	uh = (struct udphdr *)&(((char *)iph)[iph->ihl*4]);
	
	sipdata = (char *)uh + sizeof(struct udphdr);
	data = sipdata + sip_find_body_start(sipdata, ntohs(uh->len)-sizeof(struct udphdr));

	/* We can search the packet up until here, being overly safe for now */
	data_limit = skb->h.raw + skb->len;

	/* first we change the c= line to reflect the masq ip */
	while (data < data_limit)
	{
		/* find the c= line */
		if (strncmp(data,"\nc=",3) && strncmp(data,"\rc=",3))
		{
			data++;
			continue;
		}
		data += 3;

		DEBUGP("masq_sip: Found SDP c= line\n");

		/* make sure it's something we can masq */
		if (strncmp(data,"IN IP4 ",7))
			continue;
		data += 7;

		/* if it's the null address, then the call is on hold, no mangle required */
		if (!strncmp(data,"0.0.0.0",7)) {
			DEBUGP("masq_sip: Null address found in SDP message\n");
			return 0;
		}

		addrstart = data;

		/* this accepts FQDNs or dotted quads */
		while (ISALPHA(*data) || ISDIGIT(*data) || (*data=='.') || (*data=='-'))
		{
			data++;
			if (data == data_limit)
				return 0;
		}

		/* replace the skb */
		if (!ip_nat_mangle_udp_packetX(skb_p,
			(int)addrstart - (int)uh - sizeof(struct udphdr),
			data-addrstart, buf, buf_len))
			return 0;
		
		/* we're supposed to return this */
		diff += (buf_len - (data-addrstart));
		
		DEBUGP("masq_sip: c= line replaced\n");
		
		break;
	}

	/* this is our current buffer - do we need this? */
	skb = *skb_p;
	iph = skb->nh.iph;
	uh = (struct udphdr *)&(((char *)iph)[iph->ihl*4]);

	sipdata = (char *)uh + sizeof(struct udphdr);
	data = sipdata + sip_find_body_start(sipdata, ntohs(uh->len)-sizeof(struct udphdr));

	/* We can search the packet up until here, being overly safe for now */
	data_limit = skb->h.raw + skb->len;

	/* now we update the m= line with the new port */
	while (data < data_limit)
	{
		/* find the m= line */
		if (strncmp(data,"\nm=",3) && strncmp(data,"\rm=",3))
		{
			data++;
			continue;
		}
		data += 3;
		DEBUGP("masq_sip: Found SDP m= line\n");
		
		/* make sure it's something we can masq */
		if (strncmp(data,"audio ",6))
			continue;
		data += 6;
		
		portstart = data;
		port = simple_strtoul(data,&data,10);
		
		DEBUGP("masq_sip: Found port %d in SDP message\n",port);
		DEBUGP("masq_sip: change the expectation and make it NATable\n");
		
		/* Expect something from peer */
		newtuple.src.ip = 
			ct->tuplehash[!dir].tuple.src.ip;
		newtuple.dst.ip = 
			ct->tuplehash[!dir].tuple.dst.ip;
		newtuple.dst.protonum = IPPROTO_UDP;
		newtuple.src.u.udp.port = 0;

		/* Try to get same port: if not, try to change it. */
		for (mport = port; mport != 0; mport++)
		{
			newtuple.dst.u.udp.port = htons(mport);
			if (ip_conntrack_change_expect(expect, &newtuple) == 0)
				break;
		}
		
		if (mport == 0)
			return 0;
		
		DEBUGP("masq_sip: forwarding incomming rtp on port %d to "
			"%d.%d.%d.%d:%d\n", mport,
			NIPQUAD(expect->ct_tuple.dst.ip),port);

		sprintf(portbuf,"%d",mport);
		portbuf_len = strlen(portbuf);

		/* replace the skb */
		if (!ip_nat_mangle_udp_packetX(skb_p,
			(int)portstart - (int)uh - sizeof(struct udphdr),
			data-portstart, portbuf, portbuf_len))
			return 0;
		

		/* we're supposed to return this */
		diff += (portbuf_len - (data-portstart));

		DEBUGP("masq_sip: m= line replaced\n");
		break;
	}

	return diff;
}

static unsigned int help(struct ip_conntrack *ct,
			 struct ip_conntrack_expect *exp,
			 struct ip_nat_info *info,
			 enum ip_conntrack_info ctinfo,
			 unsigned int hooknum,
			 struct sk_buff **pskb)
{
	struct iphdr *iph = (*pskb)->nh.iph;
	struct udphdr *udph = (void *)iph + iph->ihl * 4;
	u_int32_t our_ip;
	u_int16_t our_port, sipport;
	char ouraddr[32], viaaddr[32];		/* xxx.xxx.xxx.xxx:xxxxx\000 */
	unsigned ouraddr_len;
	char ouraddrnoport[32];
	unsigned ouraddrnoport_len;
	char *sipdata;
	int siplen;
	int diff = 0;
	int dir;
	int i;
	enum ip_sip_mangle_type	mangle_type;
	struct ip_conntrack_tuple exptuple;
	struct ip_nat_sip_info *nat_sip_info;
	
	if(flag=='0')
		return NF_ACCEPT;
	
	if (!exp)
		DEBUGP("ip_nat_sip: no exp!!\n");
	
	dir = CTINFO2DIR(ctinfo);
	
	if (hooknum != NF_IP_POST_ROUTING)
		return NF_ACCEPT;
	
	nat_sip_info = &ct->nat.help.nat_sip_info;
	mangle_type = IP_SIP_MANGLE_NONE;
	
	LOCK_BH(&ip_sip_lock);
	if (dir == IP_CT_DIR_ORIGINAL)
	{
		our_ip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
		our_port = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port;
		for (i=0; i<info->num_manips; i++)
		{
			if (info->manips[i].direction == IP_CT_DIR_ORIGINAL)
			{
				if (info->manips[i].maniptype == IP_NAT_MANIP_SRC)
					mangle_type = IP_SIP_MANGLE_SRC;
				else
					mangle_type = IP_SIP_MANGLE_DST;
				
				break;
			}
		}
	}
	else
	{
		our_ip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip;
		our_port = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port;
		for (i=0; i<info->num_manips; i++)
		{
			if (info->manips[i].direction == IP_CT_DIR_REPLY)
			{
				if (info->manips[i].maniptype == IP_NAT_MANIP_SRC)
					mangle_type = IP_SIP_MANGLE_SRC;
				else
					mangle_type = IP_SIP_MANGLE_DST;
				
				break;
			}
		}
	}
	UNLOCK_BH(&ip_sip_lock);
	
	LOCK_BH(&ip_sip_lock);
	// Helper may be called multiple times for a single packet
	// provided that if the associated conntrack has multiple expectations.
	// We have two expectations in both direction for the RTP
	// stream, so we need to find the right one.

	if (exp != NULL && ct->expecting >= 2) {
		exptuple.src.ip = 
			ct->tuplehash[!dir].tuple.src.ip;
		exptuple.src.u.udp.port = 0;
		
		if (!ip_ct_tuple_src_equal(&exp->tuple, &exptuple)) {
			// not my expectation
			exp = 0;
		}
	}
	UNLOCK_BH(&ip_sip_lock);
	
	// Check for SIP expectation
	if (exp && exp->help.exp_sip_info.exptype == IP_CT_EXPECT_SIP) {
		if (mangle_type != IP_SIP_MANGLE_SRC)
			return NF_ACCEPT;
		
		exptuple.src.ip = 
			ct->tuplehash[!dir].tuple.src.ip;
		exptuple.dst.ip = 
			ct->tuplehash[!dir].tuple.dst.ip;
		exptuple.dst.protonum = IPPROTO_UDP;
		exptuple.src.u.udp.port = 0;

		/* Try to get same port: if not, try to change it. */
		for (sipport = ntohs(our_port); sipport != 0; sipport++)
		{
			exptuple.dst.u.udp.port = htons(sipport);
			if (ip_conntrack_change_expect(exp, &exptuple) == 0)
				break;
		}
		DEBUGP("ip_nat_sip: Change SIP expectation %p, ", ct);
		DUMP_TUPLE(&exptuple);
	}
	
/*
	for (i=0; i<info->num_manips; i++)
	{
		if (info->manips[i].direction == IP_CT_DIR_ORIGINAL)
			printk("ORIGINAL, ");
		else
			printk("REPLY, ");
		if (info->manips[i].maniptype == IP_NAT_MANIP_SRC)
			printk("SNAT\n");
		else
			printk("DNAT\n");
	}
*/
	
	sipdata = (char *) udph + sizeof(struct udphdr);
	
	/* save our address in ouraddr */
	sprintf(ouraddr,"%d.%d.%d.%d:%d",NIPQUAD(our_ip),ntohs(our_port));
	ouraddr_len = strlen(ouraddr);

	/* save our address without port in ouraddrnoport */
	sprintf(ouraddrnoport,"%d.%d.%d.%d",NIPQUAD(our_ip));
	ouraddrnoport_len = strlen(ouraddrnoport);

	if (strncmp(sipdata,"SIP/2.0",7)) {
		/* message is a request */
		/* so, mangle the via tag */
		//printk("%.3s\n", sipdata);
		if (mangle_type == IP_SIP_MANGLE_SRC) {
			// store the original via ip and port for the first time
			if (nat_sip_info->via_ip == 0)
				sip_find_via_ipport (iph, &nat_sip_info->via_ip, &nat_sip_info->via_port);
			diff += masq_sip_mangle_via(pskb, ouraddr, ouraddr_len);
		}
		else {	// need not mangle dst addr in sip request
			if (nat_sip_info->via_ip != 0) {
				// mangle the ip, port
				iph->daddr = nat_sip_info->via_ip;
				udph->dest = nat_sip_info->via_port;
			}
			return NF_ACCEPT;
		}
	}
	else {
		/* message is a status */
		/* so, mangle the via tag back to original */
		//printk("%.3s\n", sipdata+8);
		if (mangle_type == IP_SIP_MANGLE_DST) {
			if (nat_sip_info->via_ip != 0) {
				// mangle the ip, port
				iph->daddr = nat_sip_info->via_ip;
				udph->dest = nat_sip_info->via_port;
				sprintf(viaaddr,"%d.%d.%d.%d:%d",NIPQUAD(nat_sip_info->via_ip),ntohs(nat_sip_info->via_port));
				diff += masq_sip_mangle_via(pskb, viaaddr, strlen(viaaddr));
			}
			goto final;
		}
	}
	
	/* mangle the contact tag */
	if (mangle_type == IP_SIP_MANGLE_SRC)
		diff += masq_sip_mangle_contact(pskb, ouraddr, ouraddr_len);
	
	// Hey! man, if you are here for SIP expectation, it's not your
	// business to mangle sdp. Leave it for RTP expectation.
	if (exp && exp->help.exp_sip_info.exptype != IP_CT_EXPECT_SIP) {
		/* mangle the sdp body (if there is one) */
		diff += masq_sip_mangle_sdp_out(pskb, our_ip, ouraddrnoport,
				ouraddrnoport_len, ctinfo, ct, exp);
	}

	iph = (*pskb)->nh.iph;
	udph = (void *)iph + iph->ihl * 4;
	
	sipdata = (char *) udph + sizeof(struct udphdr);
	siplen = (*pskb)->len - iph->ihl*4 - sizeof(struct udphdr);
	
	/* redo the content length */
	diff += masq_sip_mangle_content_length(pskb,
		(siplen - sip_find_body_start(sipdata, siplen)));

final:
	iph = (*pskb)->nh.iph;
	udph = (void *)iph + iph->ihl * 4;
	
	// update the headers
	udph->check = 0;
	udph->len = htons(ntohs(udph->len) + diff);
	iph->tot_len = htons(ntohs(udph->len) + iph->ihl*4);

	/* update the checksums */
	(*pskb)->csum = csum_partial((char *)udph, ntohs(udph->len), 0);
	
	udph->check = csum_tcpudp_magic(iph->saddr, iph->daddr,
			ntohs(udph->len),IPPROTO_UDP, (*pskb)->csum);
	ip_send_check(iph);

	return NF_ACCEPT;
}

static int read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;
	
	len = sprintf(page, "%c\n", flag);
	
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}

static int write_proc(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(&flag, buffer, 1)) {
		return count;
	}
	
	return -EFAULT;
}

static struct ip_nat_helper sip[MAX_PORTS];
static char sip_names[MAX_PORTS][10];

/* Not __exit: called from init() */
static void fini(void)
{
	int i;

	for (i = 0; i < ports_c; i++) {
		DEBUGP("ip_nat_sip: unregistering port %d\n", ports[i]);
		ip_nat_helper_unregister(&sip[i]);
	}
	
}

static int __init init(void)
{
	int i, ret = 0;
	char *tmpname;
	struct proc_dir_entry *res=create_proc_entry("nat_sip",0, proc_net);
	
	if (res) {
		res->read_proc=read_proc;
		res->write_proc=write_proc;
	}
	
	if (ports[0] == 0)
		ports[0] = SIP_PORT;

	for (i = 0; (i < MAX_PORTS) && ports[i]; i++) {

		memset(&sip[i], 0, sizeof(struct ip_nat_helper));

		sip[i].tuple.dst.protonum = IPPROTO_UDP;
		sip[i].tuple.dst.u.udp.port = htons(ports[i]);
		sip[i].mask.dst.protonum = 0xFFFF;
		sip[i].mask.dst.u.udp.port = 0xFFFF;
		sip[i].help = help;
		sip[i].me = THIS_MODULE;
		// Helper might want to manip the packet even when there is
		// no expectation.
		sip[i].flags = IP_NAT_HELPER_F_ALWAYS;
		sip[i].expect = sip_nat_expected;

		tmpname = &sip_names[i][0];
		if (ports[i] == SIP_PORT)
			sprintf(tmpname, "sip");
		else
			sprintf(tmpname, "sip-%d", i);
		sip[i].name = tmpname;

		DEBUGP("ip_nat_sip: Trying to register for port %d\n",
				ports[i]);
		ret = ip_nat_helper_register(&sip[i]);

		if (ret) {
			printk("ip_nat_sip: error registering "
			       "helper for port %d\n", ports[i]);
			fini();
			return ret;
		}
		ports_c++;
	}

	return ret;
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
