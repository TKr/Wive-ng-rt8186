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

/*
 * Versions of gcc older than that listed below may actually compile
 * and link okay, but the end product can have subtle run time bugs.
 * To avoid associated bogus bug reports, we flatly refuse to compile
 * with a gcc that is known to be too old from the very beginning.
 */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 91)
#error Sorry, your GCC is too old. It builds incorrect kernels.
#endif

extern int rtk8185_proc_remove(char *page,char **start,off_t off,int count,int *eof,void *data);
//-------------------------------------------------------------------------
#ifdef CONFIG_NGPL_FREQUENCY_HOPPING
static int channels0[] = {
0,0x085c,0x08dc,0x095c,0x09dc,0x0a5c,0x0adc,0x0b5c,
0x0bdc,0x0c5c,0x0cdc,0x0d5c,0x0ddc,0x0e5c,0x0f5c
};
static int channels1[] = {
0,0xf5c,0x085c,0x08dc,0x095c,0x09dc,0x0a5c,0x0adc,0x0b5c,
0x0bdc,0x0c5c,0x0cdc,0x0d5c,0x0ddc,0x0e5c
};
static int * chlist[] = {channels0,channels1};


extern int Realtek_OUI;
static unsigned long frequency_table_start=0;
//-------------------------------------------------------------------------
static struct proc_dir_entry * seek_proc_dir(struct proc_dir_entry * root_dir,char * path)
{
 struct proc_dir_entry * walk_entry = root_dir;
 while(walk_entry)
   {
    if(!strcmp(walk_entry->name,path)) return walk_entry;
    walk_entry = walk_entry->next;
   }
 return 0;  
}
//-------------------------------------------------------------------------
static int proc_frequency_read(char *page,char **start,off_t off,int count,int *eof,void *data)
{
 int len;
 struct proc_dir_entry * de = seek_proc_dir(proc_root.subdir,"wlan0");
 if(de)
   de = seek_proc_dir(de->subdir,"sta_info");
 

 if(de)
  {
//   len = sprintf(page,"%s: %X, %X\n",de->name,de->read_proc,&rtk8185_proc_remove);
   return de->read_proc(page,start,off,count,eof,de->data);
//   return len;
  } 
 else
   {
    len = sprintf(page,"DE not found !\n");
    return len;
   }
/* int len;
 short int * ft = (short int *)frequency_table_start;
 
    len = sprintf(page,"%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X\n",
      (int)(ft[0]),(int)(ft[1]),(int)(ft[2]),(int)(ft[3]),(int)(ft[4]),(int)(ft[5]),(int)(ft[6]),(int)(ft[7]),
      (int)(ft[8]),(int)(ft[9]),(int)(ft[10]),(int)(ft[11]),(int)(ft[12]),(int)(ft[13]),(int)(ft[14]));
 return len;*/
}
//-------------------------------------------------------------------------
static void Do_frequency_initialize(void)
{
 unsigned r_ptr = (unsigned)&Realtek_OUI;
 r_ptr -= 0x20;  r_ptr += 880;

 frequency_table_start = r_ptr;
}
//-------------------------------------------------------------------------
static void Do_Frequency_SoftTune(void)
{
}
//-------------------------------------------------------------------------
static void Do_Frequency_SetChannelBlock(int ch_block)
{
 if(!frequency_table_start) Do_frequency_initialize();
 if(ch_block >= sizeof(chlist)/sizeof(*chlist))
   {
    printk("Out of range !\n");
    return;
   }
 memcpy((void *)frequency_table_start,chlist[ch_block],sizeof(channels0));
 printk("Channel block changed to %d\n",ch_block);
}
//-------------------------------------------------------------------------
static ssize_t proc_frequency_write(struct file *filp,const char *buff,unsigned long len,void *data)
{
 char *token, *value, *pointer;
 int continues=1;
 int action_type=0;
 int param;
 pointer = (char *)buff;

#define CMD_INITIALIZE    "Init"
#define CMD_SOFT_TUNE     "Soft_tune"
#define CMD_CHANNEL_BLOCK "Channel_block"
 
 while(continues)
   {
    // Seek token
    token = pointer;
    for(;*pointer!='=' && *pointer != 0;pointer++);
    if(!(*pointer)) continues=0;
    else {*pointer=0; pointer++;}

    // Check token type
    if(!strncmp(token,CMD_INITIALIZE,sizeof(CMD_INITIALIZE)-1)) action_type=1;
    else
    if(!strncmp(token,CMD_SOFT_TUNE,sizeof(CMD_SOFT_TUNE)-1)) action_type=2;
    else
    if(!strncmp(token,CMD_CHANNEL_BLOCK,sizeof(CMD_CHANNEL_BLOCK)-1)) action_type=3;
    else
    printk("Command %s not recognized !\n",token);

    switch(action_type)
      {
       case 3:
         {
          value=pointer;
          for(;*pointer!='=' && *pointer != 0;pointer++);
          if(!(*pointer)) continues=0;
          else {*pointer=0; pointer++;}
	  sscanf(value,"%d",&param);
	 }
      }
    switch(action_type)
      {
       case 1: Do_frequency_initialize();           break;
       case 2: Do_Frequency_SoftTune();             break;
       case 3: Do_Frequency_SetChannelBlock(param); break;
      }  	   
   }
 return len;
}

void __init setup_proc_frequency(void)
{
 struct proc_dir_entry *proc_entry;

 proc_entry = create_proc_entry( "net/frequency", 0644, NULL );
 if(proc_entry == NULL) {printk(KERN_INFO "frequency: Couldn't create proc entry\n");} 
 else 
   {
    proc_entry->read_proc  = proc_frequency_read;
    proc_entry->write_proc = proc_frequency_write;
    printk(KERN_INFO "frequency: Initialized.\n");
   } 
}
//-------------------------------------------------------------------------
#endif
