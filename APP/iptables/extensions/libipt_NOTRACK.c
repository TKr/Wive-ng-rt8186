/* Shared library add-on to iptables for the NOTRACK target,
 *	the simplest target ever added to netfilter...
 *
 * (C) 2001 by Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is distributed under the terms of GNU GPL
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>

#include <linux/netfilter_ipv4/ip_tables.h>

static void init(struct ipt_entry_target *t, unsigned int *nfcache) 
{
}

static void help(void) 
{
	printf(
"NOTRACK target\n"		"No options\n"
);
}

static int parse(int c, char **argv, int invert, unsigned int *flags,
		const struct ipt_entry *entry,
		struct ipt_entry_target **target)
{
	if (optarg) 
		exit_error(PARAMETER_PROBLEM, 
				"NOTRACK: You must not speficy an option");

	if (check_inverse(optarg, &invert,NULL,0))
		exit_error(PARAMETER_PROBLEM,
				"NOTRACK: unexpected `!'");
	return 1;
}

static void final_check(unsigned int flags)
{
}

static void save(const struct ipt_ip *ip,
		const struct ipt_entry_target *target)
{
}

static void print(const struct ipt_ip *ip,
		const struct ipt_entry_target *target, int numeric)
{
}

static struct option opts[] = { };

struct iptables_target NOTRACK = { NULL, 
	"NOTRACK",
	NETFILTER_VERSION,
	IPT_ALIGN(0),
	IPT_ALIGN(0),
	&help,
	&init,
	&parse,
	&final_check,
	&print,
	&save,
	opts 
};

void _init(void)
{
	register_target(&NOTRACK);
}
