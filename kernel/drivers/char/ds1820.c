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

#ifdef CONFIG_X86_LOCAL_APIC
#include <asm/smp.h>
#endif

#include "dsstandart.h"

/*
 * Versions of gcc older than that listed below may actually compile
 * and link okay, but the end product can have subtle run time bugs.
 * To avoid associated bogus bug reports, we flatly refuse to compile
 * with a gcc that is known to be too old from the very beginning.
 */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 91)
#error Sorry, your GCC is too old. It builds incorrect kernels.
#endif

static unsigned char ThermalLasered[8];
static int AutoCooler=1;
static int CoolerSwitch[5] = { /* off */ 30,
                               /* 25% */ 35,
			       /* 50% */ 40,
			       /* 75% */ 45,
			       /* on  */ 50};
static struct timer_list thermal_timer;
extern int cooler_speed;
//static int jiffies_convert_start;
 static int thermal_converted=0;
//-------------------------------------------------------------------------------------------
static void thermal_timer_func(unsigned long data)
{
 unsigned char scratchpad[9];
 int i,themp;

 if(AutoCooler)
   {
    if(thermal_converted)
      {
       TouchPullUp(0);
       TouchReset(); 
       TouchMatchRom(ThermalLasered);
       TouchWriteByte(0xBE); // Read scratchpad
       for(i=0;i<9;i++) scratchpad[i]=TouchReadByte();
       themp = scratchpad[1];
       themp <<= 4;
       themp |= scratchpad[0] >> 4;
       
       for(i=0;i<4;i++) if(themp <= CoolerSwitch[i]) break;
       cooler_speed = i;
       thermal_converted=0;
      }
    else
      {
       TouchReset(); 
       TouchMatchRom(ThermalLasered);
       TouchWriteByte(0x44);
       TouchPullUp(1);
       thermal_converted=1;
      }  
   }
 mod_timer(&thermal_timer, jiffies + 1500);
}
//-------------------------------------------------------------------------------------------
static int proc_ds1820_read(char *page,char **start,off_t off,int count,int *eof,void *data)
{
 int len,themp,i;
 unsigned char scratchpad[9];
 
 if(AutoCooler & !thermal_converted) 
   len = sprintf(page,"NA");
 else  
   {
    TouchReset(); 
    TouchMatchRom(ThermalLasered);
    TouchWriteByte(0xBE); // Read scratchpad
    for(i=0;i<9;i++) scratchpad[i]=TouchReadByte();
 
    themp = scratchpad[1];
    themp <<= 4;
    themp |= scratchpad[0] >> 4;
    if(scratchpad[1] & 0x80) themp |= 0xFFFFFE00;
 
    len = sprintf(page,"%d\n",themp);
   }    
 
 return len;
}
//-------------------------------------------------------------------------------------------
static ssize_t proc_ds1820_write(struct file *filp,const char *buff,unsigned long len,void *data)
{
 int t1,t2,t3,t4,t5;
 if(AutoCooler & !thermal_converted) {}
 else
   {
    TouchReset(); 
    TouchMatchRom(ThermalLasered);
    TouchWriteByte(0x44);
   }    
 
 if(!strncmp(buff,"on",2)) AutoCooler=1;
 else
 if(!strncmp(buff,"off",3)) AutoCooler=0;
 else
 if(sscanf(buff,"%d %d %d %d %d",&t1,&t2,&t3,&t4,&t5) == 5)
   {
    CoolerSwitch[0]=t1;
    CoolerSwitch[1]=t2;
    CoolerSwitch[2]=t3;
    CoolerSwitch[3]=t4;
    CoolerSwitch[4]=t5;
   }   
 return len;
}
//-------------------------------------------------------------------------------------------
void __init setup_ds1820_proc(void)
{
 struct proc_dir_entry *proc_entry;
 int cnt,i;
 unsigned char * bytes;

 proc_entry = create_proc_entry( "thermal", 0644, NULL );
 if(proc_entry == NULL) {printk(KERN_INFO "thermal: Couldn't create proc entry\n");} 
 else 
   {
    proc_entry->read_proc  = proc_ds1820_read;
    proc_entry->write_proc = proc_ds1820_write;
    printk(KERN_INFO "thermal: Initialized.\n");
    if(TouchReset())
      {
       TouchSearchRom();
       cnt = TouchDevicesCount();
       printk(KERN_INFO "\t1-wire: %d devices detected\n",cnt);
       for(i=0;i<cnt;i++)
        {
         bytes = TouchDevice(i);
	 printk(KERN_INFO "\t1-wire DS");
	 switch(bytes[0])
	   {
	    case 0x01: printk("1990A key\n"); break;
	    case 0x02: printk("1991\n"); break;
	    case 0x08: printk("1992\n"); break;
	    case 0x06: printk("1993\n"); break;
	    case 0x04: printk("1994\n"); break;
	    case 0x0A: printk("1995\n"); break;
	    case 0x0C: printk("1996\n"); break;
	    case 0x09: printk("1982\n"); break;
	    case 0x0B: printk("1985\n"); break;
	    case 0x0F: printk("1986\n"); break;
	    case 0x10: printk("1920/1820 thermometr\n"); memcpy(ThermalLasered,bytes,sizeof(ThermalLasered)); break;
	    case 0x28: printk("18B20 thermometr\n");     memcpy(ThermalLasered,bytes,sizeof(ThermalLasered)); break;
	    case 0x22: printk("1822 thermometr\n");      memcpy(ThermalLasered,bytes,sizeof(ThermalLasered)); break;
	   }
        }
       // Set convertation 9 bit	
       TouchReset();
       TouchMatchRom(ThermalLasered);
       TouchWriteByte(0x4E); // Write Scratchpad
       TouchWriteByte(0x00); // TH
       TouchWriteByte(0x00); // TL
       TouchWriteByte(0x1F); // 00011111B
       // Setup timer
       init_timer (&thermal_timer);
       thermal_timer.expires=jiffies+150;
       thermal_timer.data =(unsigned long) NULL ;
       thermal_timer.function = &thermal_timer_func;
       mod_timer(&thermal_timer, jiffies+150);	
      }
    else
      printk(KERN_INFO "\t1-wire devices not found");
   } 
}
//-------------------------------------------------------------------------------------------


