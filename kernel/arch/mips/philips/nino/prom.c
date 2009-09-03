/*
 *  arch/mips/philips/nino/prom.c
 *
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *  
 *  Early initialization code for the Philips Nino
 */
#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/bootinfo.h>
#include <asm/addrspace.h>
#include <asm/page.h>
#include <asm/system.h>

char arcs_cmdline[CL_SIZE];

#ifdef CONFIG_FB_TX3912
extern unsigned long tx3912fb_paddr;
extern unsigned long tx3912fb_vaddr;
extern unsigned long tx3912fb_size;
#endif

const char *get_system_type(void)
{
	return "Philips Nino";
}

unsigned short totall_system_memory_detected;
unsigned long detect_ram_sequence[4];
/* Do basic initialization */
void __init prom_init(int argc, char **argv, unsigned long magic, int *prom_vec)
{

	unsigned long mem_size,reg_mem;
	int cnt=1;
        unsigned long mempos;
	unsigned long memmeg;
	unsigned short save_dword;
	unsigned long flags;

#ifdef  CONFIG_RTK_MTD_ROOT
#ifdef  CONFIG_INIT_SINGLE
	strcat(arcs_cmdline,"root=/dev/mtd1 console=0 single noisapnp pci=nobios");
#else
	strcat(arcs_cmdline,"root=/dev/mtd1 console=0 noisapnp pci=nobios");
#endif
#else
#ifdef CONFIG_INIT_SINGLE
	strcat(arcs_cmdline,"root=/dev/ram console=0 ramdisk_start=0 single noisapnp pci=nobios");
#else
	strcat(arcs_cmdline,"root=/dev/ram console=0 ramdisk_start=0 noisapnp pci=nobios");
#endif
#endif
	mips_machgroup = MACH_GROUP_PHILIPS;
	mips_machtype = MACH_PHILIPS_NINO;
	local_irq_save(flags);
//Check for maximum RAM openned in MCR
	reg_mem = (*( volatile unsigned long*) 0xbd011000) & 0x30000000; 
	reg_mem >>=24;
	detect_ram_sequence[0] = reg_mem;
//If nothing in MCR we assume max available SDRAM
	if (!reg_mem)reg_mem=64;
	detect_ram_sequence[1] = reg_mem;
//Special test to be sure in RAM capacity (in some situations there's 16M in MCR and 8M real RAM)
	for(memmeg=4;memmeg<reg_mem;memmeg+=4){
	
	    mempos = 0xa0000000L + memmeg * 0x100000;
	           
	    save_dword = *(volatile unsigned short *)mempos;
	    
	    *(volatile unsigned short *)mempos = (unsigned short)0xABCD;
	    
	    if (*(volatile unsigned short *)mempos != (unsigned short)0xABCD){
		*(volatile unsigned short *)mempos = save_dword;
		break;
	    } 
	    
	    *(volatile unsigned short *)mempos = (unsigned short)0xDCBA;
	    
	    if (*(volatile unsigned short *)mempos != (unsigned short)0xDCBA){
		*(volatile unsigned short *)mempos = save_dword;
		break;
	    } 
	    
	    *(volatile unsigned short *)mempos = save_dword;
	}

	mem_size = memmeg << 20;
	
//workaround for non correct memory sizes detect for some devices
	if (reg_mem != memmeg) 
	    {
	    printk("Workaround for:");
	    //workaround for 16bit 16Mbyte RAM devices (Zyxel P330W)
	    if (reg_mem == 32 && memmeg == 20) 
		{
		printk("Zyxel P330W v.2 and others 16RAM 16Bit devices");
		reg_mem  = 16;
		memmeg 	 = 16;
		mem_size = 16777216;
		}
	    }

	detect_ram_sequence[2] = memmeg;
	detect_ram_sequence[3] = mem_size;
	
       local_irq_restore(flags);
       totall_system_memory_detected = memmeg;
       add_memory_region(0, mem_size, BOOT_MEM_RAM); 

}

void __init prom_free_prom_memory (void)
{
}
