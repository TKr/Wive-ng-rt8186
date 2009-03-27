/*
 *  arch/mips/philips/nino/setup.c
 *
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Interrupt and exception initialization for Philips Nino
 */
#include <linux/console.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include <asm/param.h>
#include <asm/rtl8186.h>
#include <linux/netdevice.h>
#include <linux/delay.h>

#define EARLY_PRINTK_ENABLE
static void shutdown_netdev()
{
	struct net_device *dev;
	printk("Shutdown network interface\n");
	read_lock(&dev_base_lock);
      	for (dev = dev_base; dev != NULL; dev = dev->next) {
		if (dev->flags&IFF_UP && dev->stop){
			printk("%s:===>\n",dev->name);
			rtnl_lock();
			dev_close(dev);
			rtnl_unlock();
		}
      	}
	read_unlock(&dev_base_lock);

}
static void nino_machine_restart(char *command)
{
	unsigned long flags;
	static void (*back_to_prom)(void) = (void (*)(void)) 0xbfc00000;
	outl(0,GIMR0);  
	save_flags(flags); cli();
	mdelay(100);
	printk("Enable G700 and over bad AP workaround CPU<=>HZ registr. \n");
	outl(0x35B60, TC0DATA);
	printk("Enable Watch Dog to Reset whole system. \n");
	*(volatile unsigned long *)(0xBD010058)=0x10; // this is to enable watch dog reset
	*(volatile unsigned long *)(0xBD01005c)=0x00; // this is to enable watch dog reset
	mdelay(10000);
	/* Reboot */
	printk("No reboot ? Fix delay in setup.c. \n");
}

static void nino_machine_halt(void)
{
	printk("Nino halted.\n");
	while(1);
}

static void nino_machine_power_off(void)
{
	printk("Nino halted. Please turn off power.\n");
	while(1);
}

static void __init nino_board_init()
{
	/*
	 * Set up the master clock module. The value set in
	 * the Clock Control Register by WindowsCE is 0x00432ba.
	 * We set a few values here and let the device drivers
	 * handle the rest.
	 *
	 * NOTE: The UART clocks must be enabled here to provide
	 *       enough time for them to settle.
	 */
}

static __init void nino_time_init(void)
{
}

static __init void nino_timer_setup(struct irqaction *irq)
{
	//correct timer for 180MHz Clock
	outl(CORRECT_HZ, TC0DATA); //correct timer for CONFIG_HZ
	irq->dev_id = (void *) irq;
	setup_irq(0, irq);
   
#ifdef CONFIG_RTL8181_WTDOG 
    *(volatile unsigned long *)(0xbd010058)=0xfff; // watch dog time-out for about 12 s
    *(volatile unsigned long *)(0xbd01005c)=0x700;
#endif    
    outl(0x3, TCCNR);
    outl(0x01, TCIR);
}

#ifdef EARLY_PRINTK_ENABLE
#ifdef CONFIG_SERIAL_CONSOLE
extern void serial_console_init();
#endif
static int __init early_printk_setup(char* arg) 
{ 
 /* early printk only works for serial ports now */ 
 console_setup(arg); 
#ifdef CONFIG_SERIAL_CONSOLE
 serial_console_init(); 
#endif
  printk( "early printk enabled \n" ); 
 return 1; 
} 
#endif
void __init nino_setup(void)
{
	extern void nino_irq_setup(void);
	//clear GIMR first!
	irq_setup = nino_irq_setup;
	set_io_port_base(KSEG1ADDR(0x1d010000));
	outl(0,GIMR0);	
	//mips_io_port_base=RTL8181_REG_BASE;
	_machine_restart = nino_machine_restart;
	_machine_halt = nino_machine_halt;
	_machine_power_off = nino_machine_power_off;

	board_time_init = nino_time_init;
	board_timer_setup = nino_timer_setup;

#ifdef CONFIG_FB
	conswitchp = &dummy_con;
#endif

	nino_board_init();

#ifdef EARLY_PRINTK_ENABLE
	//sc_yang early printk 
	early_printk_setup("0");
#endif
}
