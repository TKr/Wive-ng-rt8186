/* SIP extension for IP connection tracking. */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include <linux/netfilter_ipv4/lockhelp.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include <linux/netfilter_ipv4/ip_conntrack_sip.h>

DECLARE_LOCK(ip_sip_lock);
struct module *ip_conntrack_sip = THIS_MODULE;

#define MAX_PORTS 8
static int ports[MAX_PORTS];
static int ports_c = 0;
static char sip_conntrack_flag='1';

#if 0 
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

extern struct ip_conntrack_helper sip[MAX_PORTS];

/**
 * called when the first packet of an expected connection arrives
 */
static int sip_ct_expected(struct ip_conntrack *new)
{
	int i;
	struct ip_ct_sip_expect *exp_sip_info;
	struct ip_conntrack *master = master_ct(new);
	
	if (!new->master) {
		// Oh! I am tired of this. How could it happen!
		// This packet is in the expected list, but with no
		// master expectation. Maybe the master conntrack of the
		// expectation is not confirmed.
		// It should be a brain new conntrack.
		// Find the correct helper.
		DEBUGP("sip_ct_expected: No master expectation\n");
		for (i = 0; (i < MAX_PORTS) && ports[i]; i++) {
			if (new->tuplehash[IP_CT_DIR_ORIGINAL].
				tuple.dst.u.udp.port == htons(ports[i])) {
				new->helper = &sip[i];
				return 0;
			}
		}
		return 0;
	}
	
	exp_sip_info = &new->master->help.exp_sip_info;
	
	if (exp_sip_info->exptype == IP_CT_EXPECT_SIP) {
		// It's SIP expectation
		LOCK_BH(&ip_sip_lock);
		// Tell conntrack "NOT" to set SIP expected.
		new->help.ct_sip_info.sip_expected = 1;
		
		// This SIP expectation has just been removed.
		// Restart a new one!
		DEBUGP("sip_ct_expected: Restart expectation %p, ", master);
		DUMP_TUPLE(&new->master->tuple);
		ip_conntrack_expect_related(master, new->master);
		// Let ct helper to run for this expectation.
		new->helper = master->helper;
		UNLOCK_BH(&ip_sip_lock);
	}
	return 0;
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

/**
 * searches for the sip message and returns the via port in the message
 */
int sip_find_via_ipport (const struct iphdr *iph, u_int32_t *vip, u_int16_t *vport)
{
	struct udphdr *uh;
	char *data;
	char *data_limit;
	u_int32_t ipArray[4] = { 0 };
	int i;
	
	uh = (struct udphdr *)&(((char *)iph)[iph->ihl*4]);
	
	// set default
	if (vip)
		*vip = iph->saddr;
	if (vport)
		*vport = SIP_PORT;

	/* lets keep the original around please */
	data = (char *)uh + sizeof(struct udphdr); // sip message

	/* We can search the packet up until here, being overly safe for now */
	data_limit = (char *)iph + iph->tot_len;

	while (data < data_limit)
	{
		/* find the topmost tag */
		if (strnicmp(data,"\nvia:",5) && strnicmp(data,"\nv:",3)
			&& strnicmp(data,"\rvia:",5) && strnicmp(data,"\rv:",3))
		{
			data++;
			continue;
		}

		/* Look for UDP, since this masq module only does udp anyways */
		while (*data!='U' && *data!='u')
		{
			if (data==data_limit)
				return 0;
			data++;
		}
		data += 3;

		if (data>=data_limit)
		{
			return 0;
		}

		/* skip white space */
		while (*data==' ')
		{
			if (data==data_limit)
				return 0;
			data++;
		}

		for (i = 0; i < 4 && data < data_limit; data++)
		{
			if (*data >= '0' && *data <= '9') {
				ipArray[i] = ipArray[i]*10 + *data - '0';
			}
			else if (*data == '.')
				i++;
			else {
				// Unexpected character; if ip numbers are all
				// filled, we're finished.
				if (i == 3)
					break;
				
				DEBUGP("Char (got %u nums) `%u' unexpected\n",
				       i, *data);
				return 0;
			}
		}
		
		if (vip)
		{
			*vip = htonl((ipArray[0] << 24) | (ipArray[1] << 16)
				| (ipArray[2] << 8) | ipArray[3]);
			//printk("Via ip is %x\n", *vip);
		}
		
		while (*data!=':' && *data!='\n' && *data!='\r')
		{
			if (data==data_limit)
				return 0;
			data++;
		}
		
		if (*data == ':')
		{
			int port;
			
			data++;
			port = simple_strtoul(data,&data,10);
			//printk("Via port is %d\n", port);
			if (vport)
				*vport = port;
		}
	}
	
	return 0;
}

static int help(const struct iphdr *iph, size_t len,
		struct ip_conntrack *ct,
		enum ip_conntrack_info ctinfo)
{
	char *sipdata, *bodydata;
	char *data_limit;
	u_int32_t ipArray[4] = { 0 };
	u_int16_t portnum = 0;
	int dir = CTINFO2DIR(ctinfo);
	int i;
	struct udphdr *udph = (void *)iph + iph->ihl * 4;
	struct ip_ct_sip_master *ct_sip_info = &ct->help.ct_sip_info;
	struct ip_conntrack_expect expect;
	struct ip_ct_sip_expect *exp_sip_info = &expect.help.exp_sip_info;
	
	DEBUGP("ip_ct_sip: help\n");
	if(sip_conntrack_flag=='0')
		return NF_ACCEPT;
	
	// Add SIP expectation for master connection only
	if (!ct->master && !ct_sip_info->sip_expected)
	{
		memset(&expect, 0, sizeof(expect));
		if (dir == IP_CT_DIR_ORIGINAL)
			// expect a connection from callee.
			exp_sip_info->siptype = IP_CT_SIP_CALLEE;
		else
			// expect a connection from caller.
			exp_sip_info->siptype = IP_CT_SIP_CALLER;
		
		exp_sip_info->exptype = IP_CT_EXPECT_SIP;
		// Expected port should be via port
		sip_find_via_ipport(iph, 0, &portnum);
		exp_sip_info->port = htons(portnum);
		
		LOCK_BH(&ip_sip_lock);
		expect.tuple = ((struct ip_conntrack_tuple)
			{ { ct->tuplehash[!dir].tuple.src.ip,
			    { 0 } },
			  { ct->tuplehash[!dir].tuple.dst.ip,
			    { exp_sip_info->port },
			    IPPROTO_UDP }});
		expect.mask = ((struct ip_conntrack_tuple)
			{ { 0xFFFFFFFF, { 0 } },
			  { 0xFFFFFFFF, { 0xFFFF }, 0xFFFF }});
        	
		expect.expectfn = sip_ct_expected;
		DEBUGP("ip_ct_sip: Expect SIP %p, ", ct);
		DUMP_TUPLE(&expect.tuple);
		ip_conntrack_expect_related(ct, &expect);
		ct_sip_info->sip_expected = 1;
		UNLOCK_BH(&ip_sip_lock);
	}
	
	sipdata = (char *) udph + sizeof(struct udphdr);
	bodydata = sipdata + sip_find_body_start(sipdata, ntohs(udph->len)-sizeof(struct udphdr));
	data_limit = (void *)iph + len;
	
	/*
	printk("ct=%p\n", ct);
	if (strncmp(sipdata,"SIP/2.0",7))
		printk("%.3s\n", sipdata);
	else
		printk("%.3s\n", sipdata+8);
	*/
	
	DEBUGP("ip_ct_sip: pkt %u->%u\n", ntohs(udph->source), ntohs(udph->dest));
	// find the c= line
	while (bodydata < data_limit)
	{
		if (strncmp(bodydata,"\nc=",3) && strncmp(bodydata,"\rc=",3))
		{
			bodydata++;
			continue;
		}
		bodydata += 3;

		DEBUGP("ip_ct_sip: Found SDP c= line\n");

		/* make sure it's something we are insterested */
		if (strncmp(bodydata,"IN IP4 ",7))
			continue;
		bodydata += 7;

		/* if it's the null address, then the call is on hold, regardless */
		if (!strncmp(bodydata,"0.0.0.0",7)) {
			DEBUGP("ip_ct_sip: Null address found in SDP message\n");
			return NF_ACCEPT;
		}
		
		for (i = 0; i < 4 && bodydata < data_limit; bodydata++)
		{
			if (*bodydata >= '0' && *bodydata <= '9') {
				ipArray[i] = ipArray[i]*10 + *bodydata - '0';
			}
			else if (*bodydata == '.')
				i++;
			else {
				// Unexpected character; if ip numbers are all
				// filled, we're finished.
				if (i == 3)
					break;
				
				DEBUGP("Char (got %u nums) `%u' unexpected\n",
				       i, *bodydata);
				return NF_ACCEPT;
			}
		}
		
		if (i != 3)
		{
			DEBUGP("ip_ct_sip: Failed to fill ip numbers\n");
			return NF_ACCEPT;
		}
		
		DEBUGP("ip_ct_sip: Found ip %u.%u.%u.%u\n",
			ipArray[0], ipArray[1], ipArray[2], ipArray[3]);
			
		break;
	}
	
	if (ipArray[0] == 0)	// not found c= line ip
		return NF_ACCEPT;
	
	bodydata = sipdata + sip_find_body_start(sipdata, ntohs(udph->len)-sizeof(struct udphdr));
	
	// find the m= line
	while (bodydata < data_limit)
	{
		if (strncmp(bodydata,"\nm=",3) && strncmp(bodydata,"\rm=",3))
		{
			bodydata++;
			continue;
		}
		bodydata += 3;
		DEBUGP("ip_ct_sip: Found SDP m= line\n");

		/* make sure it's an audio stream */
		if (strncmp(bodydata,"audio ",6))
			continue;
		bodydata += 6;
		portnum = simple_strtoul(bodydata, &bodydata, 10);
		DEBUGP("ip_ct_sip: Found port %u\n", portnum);
	}
	
	if (portnum == 0)	// not found m= line port number
		return NF_ACCEPT;
	
	memset(&expect, 0, sizeof(expect));
	
	if (dir == IP_CT_DIR_ORIGINAL)
		// expect a connection from callee.
		exp_sip_info->siptype = IP_CT_SIP_CALLEE;
	else
		// expect a connection from caller.
		exp_sip_info->siptype = IP_CT_SIP_CALLER;
	
	exp_sip_info->exptype = IP_CT_EXPECT_RTP;
	exp_sip_info->port = htons(portnum);
	
	LOCK_BH(&ip_sip_lock);
	expect.tuple = ((struct ip_conntrack_tuple)
		{ { ct->tuplehash[!dir].tuple.src.ip,
		    { 0 } },
		  { htonl((ipArray[0] << 24) | (ipArray[1] << 16)
			  | (ipArray[2] << 8) | ipArray[3]),
		    { htons(portnum) },
		    IPPROTO_UDP }});
	expect.mask = ((struct ip_conntrack_tuple)
		{ { 0xFFFFFFFF, { 0 } },
		  { 0xFFFFFFFF, { 0xFFFF }, 0xFFFF }});

	expect.expectfn = NULL;

	DEBUGP("ip_ct_sip: Expect RTP %p, ", ct);
	DUMP_TUPLE(&expect.tuple);
	
	// Check if max number of expected been exceeded
	if (ct->helper->max_expected && 
		ct->expecting >= ct->helper->max_expected) {
		struct list_head *cur_item;
		// choose the oldest RTP expectation to evict
		list_for_each_prev(cur_item, &ct->sibling_list) { 
			struct ip_conntrack_expect *cur;

			cur = list_entry(cur_item, 
					 struct ip_conntrack_expect,
					 expected_list);
			if (cur->sibling == NULL) {
				if (cur->help.exp_sip_info.exptype == IP_CT_EXPECT_RTP) {
					ip_conntrack_unexpect_related(cur);
					break;
				}
			}
		}
	}
	
	/* Ignore failure; should only happen with NAT */
	ip_conntrack_expect_related(ct, &expect);
	UNLOCK_BH(&ip_sip_lock);
	
	return NF_ACCEPT;
}

static int read_proc(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len;
	
	len = sprintf(page, "%c\n", sip_conntrack_flag);
	
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
	
	if (buffer && !copy_from_user(&sip_conntrack_flag, buffer, 1)) {
		return count;
	}
	
	return -EFAULT;
}

static struct ip_conntrack_helper sip[MAX_PORTS];
static char sip_names[MAX_PORTS][10];

/* Not __exit: called from init() */
static void fini(void)
{
	int i;
	for (i = 0; i < ports_c; i++) {
		DEBUGP("ip_ct_sip: unregistering helper for port %d\n",
				ports[i]);
		ip_conntrack_helper_unregister(&sip[i]);
	}
}

static int __init init(void)
{
	int i, ret;
	char *tmpname;
	struct proc_dir_entry *res=create_proc_entry("ct_sip",0, proc_net);
	
	if (res) {
		res->read_proc=read_proc;
		res->write_proc=write_proc;
	}
	
	if (ports[0] == 0)
		ports[0] = SIP_PORT;

	for (i = 0; (i < MAX_PORTS) && ports[i]; i++) {
		memset(&sip[i], 0, sizeof(struct ip_conntrack_helper));
		sip[i].tuple.dst.u.udp.port = htons(ports[i]);
		sip[i].tuple.dst.protonum = IPPROTO_UDP;
		sip[i].mask.dst.u.udp.port = 0xFFFF;
		sip[i].mask.dst.protonum = 0xFFFF;
		sip[i].max_expected = 3;
		sip[i].timeout = 0;
		sip[i].flags = IP_CT_HELPER_F_REUSE_EXPECT;
		sip[i].me = ip_conntrack_sip;
		sip[i].help = help;

		tmpname = &sip_names[i][0];
		if (ports[i] == SIP_PORT)
			sprintf(tmpname, "sip");
		else
			sprintf(tmpname, "sip-%d", ports[i]);
		sip[i].name = tmpname;

		DEBUGP("ip_ct_sip: registering helper for port %d\n", 
				ports[i]);
		ret = ip_conntrack_helper_register(&sip[i]);

		if (ret) {
			fini();
			return ret;
		}
		ports_c++;
	}
	return 0;
}

#ifdef CONFIG_IP_NF_NAT_NEEDED
EXPORT_SYMBOL(ip_sip_lock);
#endif

MODULE_LICENSE("GPL");
module_init(init);
module_exit(fini);
