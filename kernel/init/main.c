/*
 *  linux/init/main.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  GK 2/5/95  -  Changed to support mounting root fs via NFS
 *  Added initrd & change_root: Werner Almesberger & Hans Lermen, Feb '96
 *  Moan early if gcc is old, avoiding bogus kernels - Paul Gortmaker, May '96
 *  Simplified starting of init:  Michael A. Griffith <grif@acm.org> 
 */

#define __KERNEL_SYSCALLS__

#include <linux/config.h>
#include <linux/proc_fs.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/utsname.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <linux/blk.h>
#include <linux/hdreg.h>
#include <linux/iobuf.h>
#include <linux/bootmem.h>
#include <linux/tty.h>

#include <asm/io.h>
#include <asm/bugs.h>

#if defined(CONFIG_ARCH_S390)
#include <asm/s390mach.h>
#include <asm/ccwcache.h>
#endif

#ifdef CONFIG_PCI
#include <linux/pci.h>
#endif

#ifdef CONFIG_DIO
#include <linux/dio.h>
#endif

#ifdef CONFIG_ZORRO
#include <linux/zorro.h>
#endif

#ifdef CONFIG_MTRR
#  include <asm/mtrr.h>
#endif

#ifdef CONFIG_NUBUS
#include <linux/nubus.h>
#endif

#ifdef CONFIG_ISAPNP
#include <linux/isapnp.h>
#endif

#ifdef CONFIG_IRDA
extern int irda_proto_init(void);
extern int irda_device_init(void);
#endif

#ifdef CONFIG_X86_LOCAL_APIC
#include <asm/smp.h>
#endif

/*
 * Versions of gcc older than that listed below may actually compile
 * and link okay, but the end product can have subtle run time bugs.
 * To avoid associated bogus bug reports, we flatly refuse to compile
 * with a gcc that is known to be too old from the very beginning.
 */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 91)
#if !defined(CONFIG_V850E) && !defined(CONFIG_NIOS)
#error Sorry, your GCC is too old. It builds incorrect kernels.
#else
#warning Sorry, your GCC is too old. It builds incorrect kernels.
#endif
#endif

extern char _stext, _etext;
extern char *linux_banner;

static int init(void *);

extern void init_IRQ(void);
extern void init_modules(void);
extern void sock_init(void);
extern void fork_init(unsigned long);
extern void mca_init(void);
extern void sbus_init(void);
extern void ppc_init(void);
extern void sysctl_init(void);
extern void signals_init(void);
extern int init_pcmcia_ds(void);

extern void free_initmem(void);

#ifdef CONFIG_TC
extern void tc_init(void);
#endif

extern void ecard_init(void);

#if defined(CONFIG_SYSVIPC)
extern void ipc_init(void);
#endif
#ifdef CONFIG_PERFMON
extern void perfmon_init(void);
#endif

/*
 * Boot command-line arguments
 */
#define MAX_INIT_ARGS 8
#define MAX_INIT_ENVS 8

extern void time_init(void);
extern void softirq_init(void);

int rows, cols;

#ifdef CONFIG_BLK_DEV_INITRD
unsigned int real_root_dev;	/* do_proc_dointvec cannot handle kdev_t */
#endif

#ifdef CONFIG_NGPL_FREQUENCY_HOPPING
extern void __init setup_proc_frequency(void);
#endif 

#ifdef CONFIG_PROC_DS1820
extern void __init setup_ds1820_proc(void);
#endif

int root_mountflags = MS_RDONLY;
char *execute_command;
char root_device_name[64];


static char * argv_init[MAX_INIT_ARGS+2] = { "init", NULL, };
static char * envp_init[MAX_INIT_ENVS+2] = { "HOME=/", "TERM=linux", NULL, };

static int __init profile_setup(char *str)
{
    int par;
    if (get_option(&str,&par)) prof_shift = par;
	return 1;
}

__setup("profile=", profile_setup);


static struct dev_name_struct {
	const char *name;
	const int num;
} root_dev_names[] __initdata = {
#ifdef JACKSON_NET_WORK
	{ "nfs",     0x00ff },
	{ "hda",     0x0300 },
	{ "hdb",     0x0340 },
	{ "loop",    0x0700 },
	{ "hdc",     0x1600 },
	{ "hdd",     0x1640 },
	{ "hde",     0x2100 },
	{ "hdf",     0x2140 },
	{ "hdg",     0x2200 },
	{ "hdh",     0x2240 },
	{ "hdi",     0x3800 },
	{ "hdj",     0x3840 },
	{ "hdk",     0x3900 },
	{ "hdl",     0x3940 },
	{ "hdm",     0x5800 },
	{ "hdn",     0x5840 },
	{ "hdo",     0x5900 },
	{ "hdp",     0x5940 },
	{ "hdq",     0x5A00 },
	{ "hdr",     0x5A40 },
	{ "hds",     0x5B00 },
	{ "hdt",     0x5B40 },
	{ "sda",     0x0800 },
	{ "sdb",     0x0810 },
	{ "sdc",     0x0820 },
	{ "sdd",     0x0830 },
	{ "sde",     0x0840 },
	{ "sdf",     0x0850 },
	{ "sdg",     0x0860 },
	{ "sdh",     0x0870 },
	{ "sdi",     0x0880 },
	{ "sdj",     0x0890 },
	{ "sdk",     0x08a0 },
	{ "sdl",     0x08b0 },
	{ "sdm",     0x08c0 },
	{ "sdn",     0x08d0 },
	{ "sdo",     0x08e0 },
	{ "sdp",     0x08f0 },
	{ "ada",     0x1c00 },
	{ "adb",     0x1c10 },
	{ "adc",     0x1c20 },
	{ "add",     0x1c30 },
	{ "ade",     0x1c40 },
	{ "fd",      0x0200 },
	{ "md",      0x0900 },	     
	{ "xda",     0x0d00 },
	{ "xdb",     0x0d40 },
	{ "ram",     0x0100 },
	{ "scd",     0x0b00 },
	{ "mcd",     0x1700 },
	{ "cdu535",  0x1800 },
	{ "sonycd",  0x1800 },
	{ "aztcd",   0x1d00 },
	{ "cm206cd", 0x2000 },
	{ "gscd",    0x1000 },
	{ "sbpcd",   0x1900 },
	{ "eda",     0x2400 },
	{ "edb",     0x2440 },
	{ "pda",	0x2d00 },
	{ "pdb",	0x2d10 },
	{ "pdc",	0x2d20 },
	{ "pdd",	0x2d30 },
	{ "pcd",	0x2e00 },
	{ "pf",		0x2f00 },
	{ "apblock", APBLOCK_MAJOR << 8},
	{ "ddv", DDV_MAJOR << 8},
	{ "jsfd",    JSFD_MAJOR << 8},
#endif
#if defined(CONFIG_ARCH_S390)
	{ "dasda", (DASD_MAJOR << MINORBITS) },
	{ "dasdb", (DASD_MAJOR << MINORBITS) + (1 << 2) },
	{ "dasdc", (DASD_MAJOR << MINORBITS) + (2 << 2) },
	{ "dasdd", (DASD_MAJOR << MINORBITS) + (3 << 2) },
	{ "dasde", (DASD_MAJOR << MINORBITS) + (4 << 2) },
	{ "dasdf", (DASD_MAJOR << MINORBITS) + (5 << 2) },
	{ "dasdg", (DASD_MAJOR << MINORBITS) + (6 << 2) },
	{ "dasdh", (DASD_MAJOR << MINORBITS) + (7 << 2) },
#endif
#if defined(CONFIG_BLK_CPQ_DA) || defined(CONFIG_BLK_CPQ_DA_MODULE)
	{ "ida/c0d0p",0x4800 },
	{ "ida/c0d1p",0x4810 },
	{ "ida/c0d2p",0x4820 },
	{ "ida/c0d3p",0x4830 },
	{ "ida/c0d4p",0x4840 },
	{ "ida/c0d5p",0x4850 },
	{ "ida/c0d6p",0x4860 },
	{ "ida/c0d7p",0x4870 },
	{ "ida/c0d8p",0x4880 },
	{ "ida/c0d9p",0x4890 },
	{ "ida/c0d10p",0x48A0 },
	{ "ida/c0d11p",0x48B0 },
	{ "ida/c0d12p",0x48C0 },
	{ "ida/c0d13p",0x48D0 },
	{ "ida/c0d14p",0x48E0 },
	{ "ida/c0d15p",0x48F0 },
#endif
#if defined(CONFIG_BLK_CPQ_CISS_DA) || defined(CONFIG_BLK_CPQ_CISS_DA_MODULE)
	{ "cciss/c0d0p",0x6800 },
	{ "cciss/c0d1p",0x6810 },
	{ "cciss/c0d2p",0x6820 },
	{ "cciss/c0d3p",0x6830 },
	{ "cciss/c0d4p",0x6840 },
	{ "cciss/c0d5p",0x6850 },
	{ "cciss/c0d6p",0x6860 },
	{ "cciss/c0d7p",0x6870 },
	{ "cciss/c0d8p",0x6880 },
	{ "cciss/c0d9p",0x6890 },
	{ "cciss/c0d10p",0x68A0 },
	{ "cciss/c0d11p",0x68B0 },
	{ "cciss/c0d12p",0x68C0 },
	{ "cciss/c0d13p",0x68D0 },
	{ "cciss/c0d14p",0x68E0 },
	{ "cciss/c0d15p",0x68F0 },
#endif
#ifdef JACKSON_NET_WORK
	{ "nftla", 0x5d00 },
	{ "nftlb", 0x5d10 },
	{ "nftlc", 0x5d20 },
	{ "nftld", 0x5d30 },
	{ "ftla", 0x2c00 },
	{ "ftlb", 0x2c08 },
	{ "ftlc", 0x2c10 },
	{ "ftld", 0x2c18 },
	{ "mtdblock", 0x1f00 },
#endif
	{ NULL, 0 }
};

#ifdef JACKSON_NET_WORK 
kdev_t __init name_to_kdev_t(char *line)
{
	int base = 0;

	if (strncmp(line,"/dev/",5) == 0) {
		struct dev_name_struct *dev = root_dev_names;
		line += 5;
		do {
			int len = strlen(dev->name);
			if (strncmp(line,dev->name,len) == 0) {
				line += len;
				base = dev->num;
				break;
			}
			dev++;
		} while (dev->name);
	}
	return to_kdev_t(base + simple_strtoul(line,NULL,base?10:16));
}

static int __init root_dev_setup(char *line)
{
	int i;
	char ch;

	ROOT_DEV = name_to_kdev_t(line);
	memset (root_device_name, 0, sizeof root_device_name);
	if (strncmp (line, "/dev/", 5) == 0) line += 5;
	for (i = 0; i < sizeof root_device_name - 1; ++i)
	{
	    ch = line[i];
	    if ( isspace (ch) || (ch == ',') || (ch == '\0') ) break;
	    root_device_name[i] = ch;
	}
	return 1;
}

__setup("root=", root_dev_setup);
#endif
static int __init checksetup(char *line)
{
	struct kernel_param *p;

	p = &__setup_start;
	do {
		int n = strlen(p->str);
		if (!strncmp(line,p->str,n)) {
			if (p->setup_func(line+n))
				return 1;
		}
		p++;
	} while (p < &__setup_end);
	return 0;
}

/* this should be approx 2 Bo*oMips to start (note initial shift), and will
   still work even if initially too large, it will just take slightly longer */
unsigned long loops_per_jiffy = (1<<12);

/* This is the number of bits of precision for the loops_per_jiffy.  Each
   bit takes on average 1.5/HZ seconds.  This (like the original) is a little
   better than 1% */
#define LPS_PREC 8

#ifdef CONFIG_PROC_BOGOMIPS
char * calculate_bogomips(void)
{
	unsigned long ticks, loopbit;
	int lps_precision = LPS_PREC;
	static char retbuff[128];

	loops_per_jiffy = (1<<12);

//	printk("Calibrating delay loop... ");
	while (loops_per_jiffy <<= 1) {
		/* wait for "start of" clock tick */
		ticks = jiffies;
		while (ticks == jiffies)
			/* nothing */;
		/* Go .. */
		ticks = jiffies;
		__delay(loops_per_jiffy);
		ticks = jiffies - ticks;
		if (ticks)
			break;
	}

/* Do a binary approximation to get loops_per_jiffy set to equal one clock
   (up to lps_precision bits) */
	loops_per_jiffy >>= 1;
	loopbit = loops_per_jiffy;
	while ( lps_precision-- && (loopbit >>= 1) ) {
		loops_per_jiffy |= loopbit;
		ticks = jiffies;
		while (ticks == jiffies);
		ticks = jiffies;
		__delay(loops_per_jiffy);
		if (jiffies != ticks)	/* longer than 1 tick */
			loops_per_jiffy &= ~loopbit;
	}

/* Round the value and print it */	
	sprintf(retbuff,"%lu.%02lu BogoMIPS\n",
		loops_per_jiffy/(500000/HZ),
		(loops_per_jiffy/(5000/HZ)) % 100);
        return retbuff;		
}

int bogomips_read(char *page,char **start,off_t off,int count,int *eof,void *data);

void __init calibrate_delay(void)
{
 printk("%s",calculate_bogomips());
}

void __init setup_bogomips(void)
{
 struct proc_dir_entry *proc_entry;
 proc_entry = create_proc_entry( "bogomips", 0644, NULL );
 if(proc_entry == NULL) 
   {
    printk(KERN_INFO "bogomips: Couldn't create proc entry\n");
   } 
 else 
   {
    proc_entry->read_proc = bogomips_read;
    printk(KERN_INFO "bogomips: Initialized.\n");
   } 
}

int bogomips_read(char *page,char **start,off_t off,int count,int *eof,void *data)
{
 int len;
 if(off > 0) 
   {
    *eof = 1;
     return 0;
   }
 len = sprintf(page, "%s\n",calculate_bogomips());
 return len;
}
					   
#else //CONFIG_PROC_BOGOMIPS
void __init calibrate_delay(void)
{
	unsigned long ticks, loopbit;
	int lps_precision = LPS_PREC;

#ifdef FIXED_BOGOMIPS
	int bogus;

/* FIXED_BOGOMIPS converted to __delay units.  */
#define FIXED_LOOPS_PER_JIFFY	(unsigned long)(FIXED_BOGOMIPS * (500000 / HZ))

/* The maximum error in FIXED_LOOPS_PER_JIFFY that we will tolerate.  */
#define FIXED_LPJ_TOLERANCE	(unsigned long)(FIXED_LOOPS_PER_JIFFY * 0.10)

	/* Make sure fixed delay - T% is zero ticks.  */
	ticks = jiffies;
	while (ticks == jiffies) /* Synchronize with start of tick */
		/* nothing */;
	ticks = jiffies;
	__delay(FIXED_LOOPS_PER_JIFFY - FIXED_LPJ_TOLERANCE);
	bogus = (jiffies != ticks);

	if (! bogus) {
		/* Make sure fixed delay + T% is one tick.  The delay here
		   is very short because we're actually continuing timing from
		   the tick synchronization above (we don't resynchronize).  */
		__delay(2 * FIXED_LPJ_TOLERANCE);
		bogus = (jiffies != ticks + 1);
	}
	
	if (! bogus) {
		/* Use the precomputed value.  */
		loops_per_jiffy = FIXED_LOOPS_PER_JIFFY;
		printk("Delay loop constant: %lu.%02lu BogoMIPS (precomputed)\n",
		       (unsigned long)FIXED_BOGOMIPS,
		       ((unsigned long)(FIXED_BOGOMIPS * 100)) % 100);
		return;
	} else {
		printk("Precomputed BogoMIPS value (%lu.%02lu) inaccurate!\n",
		       (unsigned long)FIXED_BOGOMIPS,
		       ((unsigned long)(FIXED_BOGOMIPS * 100)) % 100);
		/* ... and fall through to normal bogomips calculation.  */
	}
#endif /* FIXED_BOGOMIPS */

	loops_per_jiffy = (1<<12);

	printk("Calibrating delay loop... ");
	while (loops_per_jiffy <<= 1) {
		/* wait for "start of" clock tick */
		ticks = jiffies;
		while (ticks == jiffies)
			/* nothing */;
		/* Go .. */
		ticks = jiffies;
		__delay(loops_per_jiffy);
		ticks = jiffies - ticks;
		if (ticks)
			break;
	}

/* Do a binary approximation to get loops_per_jiffy set to equal one clock
   (up to lps_precision bits) */
	loops_per_jiffy >>= 1;
	loopbit = loops_per_jiffy;
	while ( lps_precision-- && (loopbit >>= 1) ) {
		loops_per_jiffy |= loopbit;
		ticks = jiffies;
		while (ticks == jiffies);
		ticks = jiffies;
		__delay(loops_per_jiffy);
		if (jiffies != ticks)	/* longer than 1 tick */
			loops_per_jiffy &= ~loopbit;
	}

/* Round the value and print it */	
	printk("%lu.%02lu BogoMIPS\n",
		loops_per_jiffy/(500000/HZ),
		(loops_per_jiffy/(5000/HZ)) % 100);
}
#endif

#ifdef CONFIG_USERSPACE_WATCHDOG
static int userspace_watchdog_read(char *page,char **start,off_t off,int count,int *eof,void *data)
{
 int len;
 *(volatile unsigned long *)(0xbd01005c)=0x700;
 len = sprintf(page, "WD cleared");
 return len;
}
					    

void __init setup_userspace_watchdog(void)
{
 struct proc_dir_entry *proc_entry;

 *(volatile unsigned long *)(0xbd010058)=0xffff; // watch dog time-out for about 6 min
 *(volatile unsigned long *)(0xbd01005c)=0x700;

 proc_entry = create_proc_entry( "user_watchdog", 0644, NULL );
 if(proc_entry == NULL) 
   {
    printk(KERN_INFO "user_watchdog: Couldn't create proc entry\n");
   } 
 else 
   {
    proc_entry->read_proc = userspace_watchdog_read;
    printk(KERN_INFO "user_watchdog: Initialized.\n");
   } 
}
#endif

#ifdef CONFIG_PROC_LEDS
extern int led_flash0[16];
extern int led_flash1[16];

ssize_t proc_led0_write(struct file *filp,const char *buff,unsigned long len,void *data)
{
 int i;
 memset(led_flash0,0,sizeof(led_flash0));

 sscanf(buff,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
   led_flash0   ,led_flash0+ 1,led_flash0+ 2,led_flash0+ 3,
   led_flash0+ 4,led_flash0+ 5,led_flash0+ 6,led_flash0+ 7,
   led_flash0+ 8,led_flash0+ 9,led_flash0+10,led_flash0+11,
   led_flash0+12,led_flash0+13,led_flash0+14,led_flash0+15);


 for(i=0;i<16;i+=2) {if(led_flash0[i+1]<10) led_flash0[i]=10;}
 return len;
}

ssize_t proc_led1_write(struct file *filp,const char *buff,unsigned long len,void *data)
{
 int i;
 memset(led_flash1,0,sizeof(led_flash1));

 sscanf(buff,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
   led_flash1   ,led_flash1+ 1,led_flash1+ 2,led_flash1+ 3,
   led_flash1+ 4,led_flash1+ 5,led_flash1+ 6,led_flash1+ 7,
   led_flash1+ 8,led_flash1+ 9,led_flash1+10,led_flash1+11,
   led_flash1+12,led_flash1+13,led_flash1+14,led_flash1+15);


 for(i=0;i<16;i+=2) {if(led_flash1[i+1]<10) led_flash1[i]=10;}
 return len;
}
						    
void __init setup_proc_leds(void)
{
 struct proc_dir_entry *proc_entry;

 /* create /proc/leds */
 struct proc_dir_entry *leds;
 leds = proc_mkdir("leds", 0);

 proc_entry = create_proc_entry( "led0", 0644, leds);
 if(proc_entry == NULL) {printk(KERN_INFO "led0: Couldn't create proc entry\n");} 
 else 
   {
    proc_entry->write_proc = proc_led0_write;
    printk(KERN_INFO "led0: Initialized.\n");
   } 

 proc_entry = create_proc_entry( "led1", 0644, leds);
 if(proc_entry == NULL) {printk(KERN_INFO "led1: Couldn't create proc entry\n");} 
 else 
   {
    proc_entry->write_proc = proc_led1_write;
    printk(KERN_INFO "led1: Initialized.\n");
   } 
}
#endif

#ifdef CONFIG_PROC_COOLER
extern int cooler_speed;
ssize_t proc_cooler_write(struct file *filp,const char *buff,unsigned long len,void *data)
{
 sscanf(buff,"%d",&cooler_speed);
 printk("Set cooler speed to %d\n",cooler_speed);
 return len;
}

void __init setup_proc_cooler(void)
{
 struct proc_dir_entry *proc_entry;

 proc_entry = create_proc_entry("cooler", 0644, NULL );
 if(proc_entry == NULL) {printk(KERN_INFO "cooler: Couldn't create proc entry\n");} 
 else 
   {
    proc_entry->write_proc = proc_cooler_write;
    printk(KERN_INFO "cooler: Initialized.\n");
   } 
}
#endif

extern unsigned short totall_system_memory_detected;

static int proc_sysmem_read(char *page,char **start,off_t off,int count,int *eof,void *data)
{
 int len;
 len = sprintf(page, "%dM\n",totall_system_memory_detected);
 return len;
}

void __init setup_proc_sysmem(void)
{
 struct proc_dir_entry *proc_entry;

 proc_entry = create_proc_entry( "sysmem", 0644, NULL );
 if(proc_entry == NULL) {printk(KERN_INFO "sysmem: Couldn't create proc entry\n");} 
 else 
   {
    proc_entry->read_proc = proc_sysmem_read;
    printk(KERN_INFO "sysmem: Initialized.\n");
   }
}

#ifdef JACKSON_NET_WORK
static int __init readonly(char *str)
{
	if (*str)
		return 0;
	root_mountflags |= MS_RDONLY;
	return 1;
}

static int __init readwrite(char *str)
{
	if (*str)
		return 0;
	root_mountflags &= ~MS_RDONLY;
	return 1;
}

static int __init debug_kernel(char *str)
{
	if (*str)
		return 0;
	console_loglevel = 10;
	return 1;
}

static int __init quiet_kernel(char *str)
{
	if (*str)
		return 0;
	console_loglevel = 4;
	return 1;
}

__setup("ro", readonly);
__setup("rw", readwrite);
__setup("debug", debug_kernel);
__setup("quiet", quiet_kernel);
#endif
/*
 * This is a simple kernel command line parsing function: it parses
 * the command line, and fills in the arguments/environment to init
 * as appropriate. Any cmd-line option is taken to be an environment
 * variable if it contains the character '='.
 *
 * This routine also checks for options meant for the kernel.
 * These options are not given to init - they are for internal kernel use only.
 */
static void __init parse_options(char *line)
{
	char *next,*quote;
	int args, envs;

	if (!*line)
		return;
	args = 0;
	envs = 1;	/* TERM is set to 'linux' by default */
	next = line;
	while ((line = next) != NULL) {
                quote = strchr(line,'"');
                next = strchr(line, ' ');
                while (next != NULL && quote != NULL && quote < next) {
                        /* we found a left quote before the next blank
                         * now we have to find the matching right quote
                         */
                        next = strchr(quote+1, '"');
                        if (next != NULL) {
                                quote = strchr(next+1, '"');
                                next = strchr(next+1, ' ');
                        }
                }
                if (next != NULL)
                        *next++ = 0;
		if (!strncmp(line,"init=",5)) {
			line += 5;
			execute_command = line;
			/* In case LILO is going to boot us with default command line,
			 * it prepends "auto" before the whole cmdline which makes
			 * the shell think it should execute a script with such name.
			 * So we ignore all arguments entered _before_ init=... [MJ]
			 */
			args = 0;
			continue;
		}
		if (checksetup(line))
			continue;
		
		/*
		 * Then check if it's an environment variable or
		 * an option.
		 */
		if (strchr(line,'=')) {
			if (envs >= MAX_INIT_ENVS)
				break;
			envp_init[++envs] = line;
		} else {
			if (args >= MAX_INIT_ARGS)
				break;
			if (*line)
				argv_init[++args] = line;
		}
	}
	argv_init[args+1] = NULL;
	envp_init[envs+1] = NULL;
}


extern void setup_arch(char **);

unsigned long wait_init_idle;

#ifndef CONFIG_SMP

#ifdef CONFIG_X86_LOCAL_APIC
static void __init smp_init(void)
{
	APIC_init_uniprocessor();
}
#else
#define smp_init()	do { } while (0)
#endif

#else


/* Called by boot processor to activate the rest. */
static void __init smp_init(void)
{
	/* Get other processors into their bootup holding patterns. */
	smp_boot_cpus();
	wait_init_idle = cpu_online_map;
	clear_bit(current->processor, &wait_init_idle); /* Don't wait on me! */

	smp_threads_ready=1;
	smp_commence();

	/* Wait for the other cpus to set up their idle processes */
	printk("Waiting on wait_init_idle (map = 0x%lx)\n", wait_init_idle);
	while (wait_init_idle) {
		cpu_relax();
		barrier();
	}
	printk("All processors have done init_idle\n");
}

#endif

/*
 * We need to finalize in a non-__init function or else race conditions
 * between the root thread and the init thread may cause start_kernel to
 * be reaped by free_initmem before the root thread has proceeded to
 * cpu_idle.
 */

static void rest_init(void)
{
	kernel_thread(init, NULL, CLONE_FS | CLONE_FILES | CLONE_SIGNAL);
	unlock_kernel();
	current->need_resched = 1;
 	cpu_idle();
} 

/*
 *	Activate the first processor.
 */

asmlinkage void __init start_kernel(void)
{
	char * command_line;
	unsigned long mempages;
	extern char saved_command_line[];

/*
 * Interrupts are still disabled. Do necessary setups, then
 * enable them
 */
	lock_kernel();
	printk(linux_banner);
	setup_arch(&command_line);
	printk("Kernel command line: %s\n", saved_command_line);
	parse_options(command_line);
	trap_init();
	init_IRQ();
	sched_init();
	softirq_init();
	time_init();

	/*
	 * HACK ALERT! This is early. We're enabling the console before
	 * we've done PCI setups etc, and console_init() must be aware of
	 * this. But we do want output early, in case something goes wrong.
	 */
	console_init();
#ifdef CONFIG_MODULES
	init_modules();
#endif
	if (prof_shift) {
		unsigned int size;
		/* only text is profiled */
		prof_len = (unsigned long) &_etext - (unsigned long) &_stext;
		prof_len >>= prof_shift;
		
		size = prof_len * sizeof(unsigned int) + PAGE_SIZE-1;
		prof_buffer = (unsigned int *) alloc_bootmem(size);
	}

	kmem_cache_init();
	sti();
	calibrate_delay();
#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_start && !initrd_below_start_ok &&
			initrd_start < min_low_pfn << PAGE_SHIFT) {
		printk(KERN_CRIT "initrd overwritten (0x%08lx < 0x%08lx) - "
		    "disabling it.\n",initrd_start,min_low_pfn << PAGE_SHIFT);
		initrd_start = 0;
	}
#endif
	mem_init();
	kmem_cache_sizes_init();
	pgtable_cache_init();

#ifdef CONFIG_PERFMON
	perfmon_init();
#endif
	mempages = num_physpages;

	fork_init(mempages);
	proc_caches_init();
	vfs_caches_init(mempages);
	buffer_init(mempages);
	page_cache_init(mempages);
#if defined(CONFIG_ARCH_S390)
	ccwcache_init();
#endif
	signals_init();
#ifdef CONFIG_PROC_FS
	proc_root_init();
#endif
#if defined(CONFIG_SYSVIPC)
	ipc_init();
#endif
	check_bugs();
	printk("POSIX conformance testing by UNIFIX\n");

	/* 
	 *	We count on the initial thread going ok 
	 *	Like idlers init is an unlocked kernel thread, which will
	 *	make syscalls (and thus be locked).
	 */
	smp_init();
	rest_init();
}

#ifdef CONFIG_BLK_DEV_INITRD
static int do_linuxrc(void * shell)
{
	static char *argv[] = { "linuxrc", NULL, };

	close(0);close(1);close(2);
	setsid();
	(void) open("/dev/console",O_RDWR,0);
	(void) dup(0);
	(void) dup(0);
	return execve(shell, argv, envp_init);
}

#endif

struct task_struct *child_reaper = &init_task;

static void __init do_initcalls(void)
{
	initcall_t *call;

	call = &__initcall_start;
	do {
		(*call)();
		call++;
	} while (call < &__initcall_end);

	/* Make sure there is no pending stuff from the initcall sequence */
	flush_scheduled_tasks();
}

/*
 * Ok, the machine is now initialized. None of the devices
 * have been touched yet, but the CPU subsystem is up and
 * running, and memory and process management works.
 *
 * Now we can finally start doing some real work..
 */
static void __init do_basic_setup(void)
{

	/*
	 * Tell the world that we're going to be the grim
	 * reaper of innocent orphaned children.
	 *
	 * We don't want people to have to make incorrect
	 * assumptions about where in the task array this
	 * can be found.
	 */
	child_reaper = current;

#if defined(CONFIG_MTRR)	/* Do this after SMP initialization */
/*
 * We should probably create some architecture-dependent "fixup after
 * everything is up" style function where this would belong better
 * than in init/main.c..
 */
	mtrr_init();
#endif

#ifdef CONFIG_SYSCTL
	sysctl_init();
#endif
#ifdef CONFIG_PROC_BOGOMIPS
        setup_bogomips();
#endif
#ifdef CONFIG_USERSPACE_WATCHDOG
        setup_userspace_watchdog();
#endif
#ifdef CONFIG_PROC_LEDS
        setup_proc_leds();
#endif
#ifdef CONFIG_PROC_COOLER
        setup_proc_cooler();
#endif
	setup_proc_sysmem();
#ifdef CONFIG_NGPL_FREQUENCY_HOPPING
	setup_proc_frequency();
#endif	
#ifdef CONFIG_PROC_DS1820
        setup_ds1820_proc();
#endif

	/*
	 * Ok, at this point all CPU's should be initialized, so
	 * we can start looking into devices..
	 */
#if defined(CONFIG_ARCH_S390)
	s390_init_machine_check();
#endif

#ifdef CONFIG_PCI
	pci_init();
#endif
#ifdef CONFIG_SBUS
	sbus_init();
#endif
#if defined(CONFIG_PPC)
	ppc_init();
#endif
#ifdef CONFIG_MCA
	mca_init();
#endif
#ifdef CONFIG_ARCH_ACORN
	ecard_init();
#endif
#ifdef CONFIG_ZORRO
	zorro_init();
#endif
#ifdef CONFIG_DIO
	dio_init();
#endif
#ifdef CONFIG_NUBUS
	nubus_init();
#endif
#ifdef CONFIG_ISAPNP
	isapnp_init();
#endif
#ifdef CONFIG_TC
	tc_init();
#endif

	/* Networking initialization needs a process context */ 
	sock_init();

	start_context_thread();
	do_initcalls();

#ifdef CONFIG_IRDA
	irda_proto_init();
	irda_device_init(); /* Must be done after protocol initialization */
#endif
#ifdef CONFIG_PCMCIA
	init_pcmcia_ds();		/* Do this last */
#endif
}

extern void rd_load(void);
extern void initrd_load(void);

/*
 * Prepare the namespace - decide what/where to mount, load ramdisks, etc.
 */
static void prepare_namespace(void)
{
#ifdef CONFIG_BLK_DEV_INITRD
	int real_root_mountflags = root_mountflags;
	if (!initrd_start)
		mount_initrd = 0;
	if (mount_initrd)
		root_mountflags &= ~MS_RDONLY;
	real_root_dev = ROOT_DEV;
#endif

#ifdef CONFIG_BLK_DEV_RAM
#ifdef CONFIG_BLK_DEV_INITRD
	if (mount_initrd)
		initrd_load();
	else
#endif
	rd_load();
#endif

	/* Mount the root filesystem.. */
	mount_root();

	mount_devfs_fs ();

#ifdef CONFIG_BLK_DEV_INITRD
	root_mountflags = real_root_mountflags;
	if (mount_initrd && ROOT_DEV != real_root_dev
	    && MAJOR(ROOT_DEV) == RAMDISK_MAJOR && MINOR(ROOT_DEV) == 0) {
		int error;
		int i, pid;

		pid = kernel_thread(do_linuxrc, "/linuxrc", SIGCHLD);
		if (pid > 0) {
			while (pid != wait(&i)) {
				current->policy |= SCHED_YIELD;
				schedule();
			}
		}
		if (MAJOR(real_root_dev) != RAMDISK_MAJOR
		     || MINOR(real_root_dev) != 0) {
			error = change_root(real_root_dev,"/initrd");
			if (error)
				printk(KERN_ERR "Change root to /initrd: "
				    "error %d\n",error);
		}
	}
#endif
}

static int init(void * unused)
{
	
    int err;
    unsigned long flags;


	lock_kernel();
	do_basic_setup();

	prepare_namespace();

	/*
	 * Ok, we have completed the initial bootup, and
	 * we're essentially up and running. Get rid of the
	 * initmem segments and start the user-mode stuff..
	 */
	free_initmem();
	unlock_kernel();

#ifdef CONFIG_NO_CONSOLE_OPEN
	if (open("/dev/tty", O_RDWR, 0) < 0)
		printk("Warning: unable to open an initial console.\n");
#else
	if (open("/dev/console", O_RDWR, 0) < 0)
		printk("Warning: unable to open an initial console.\n");
#endif

	(void) dup(0);
	(void) dup(0);


    flags=MS_MGC_VAL;
    flags&=~MS_NOSUID;
    flags&=~MS_NODEV;
    flags&=~MS_NOEXEC;
    flags&=~MS_SYNCHRONOUS;
    
extern asmlinkage long sys_mount(char * dev_name, char * dir_name, char * type,
                                unsigned long new_flags, void * data);
                                
    if ((err=sys_mount("none","/proc","proc",flags,NULL)))
        printk("mount /proc file system fail! error code=%d\n",err);
    else
        printk("mount /proc file system ok!\n");
#ifdef CONFIG_RTK_MTD_ROOT
    if ((err=sys_mount("tmpfs","/var","tmpfs",flags,"size=700k")))
        printk("mount /var file system fail! error code=%d\n",err);
    else
        printk("mount /var  file system ok!\n");
#endif
#ifdef CONFIG_RO_FS_MOUNT
    flags |= MS_RDONLY;
    if ((err=sys_mount("/dev/mtd1",CONFIG_RO_MOUNT_POINT,"squashfs",flags,NULL)))
        printk("mount /dev/mtd1 file system fail! error code=%d\n",err);
    else
        printk("mount /dev/mtd1 file system ok!\n");
#endif

	/*
	 * We try each of these until one succeeds.
	 *
	 * The Bourne shell can be used instead of init if we are 
	 * trying to recover a really broken machine.
	 */

	if (execute_command)
		execve(execute_command,argv_init,envp_init);
	execve("/sbin/init",argv_init,envp_init);
	panic("No init found.  Try passing init= option to kernel.");
}


