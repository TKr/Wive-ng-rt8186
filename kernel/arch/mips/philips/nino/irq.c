/*
 *  arch/mips/philips/nino/irq.c
 *
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Interrupt service routines for Philips Nino
 */
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/mipsregs.h>

#ifdef CONFIG_RTL8186_AP
#include <asm/rtl8186.h>
#endif
#ifdef CONFIG_RTL8196_AP
#include <asm/rtl8196.h>
#endif

#define ALLINTS (IE_IRQ0 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)

static void  unmask_irq(unsigned int irq)
{
	outl((inl(GIMR0) | (1 << irq)),GIMR0);
    inl(GIMR0);
}
static void  mask_irq(unsigned int irq)
{
	outl(inl(GIMR0) & (~(1 << irq)),GIMR0);
    inl(GIMR0);
}

extern asmlinkage void do_IRQ(int irq, struct pt_regs *regs);

static void enable_irq(unsigned int irq)
{

	return 0;
}

static unsigned int startup_irq(unsigned int irq)
{
	unmask_irq(irq);
	return 0;		/* Never anything pending  */
}

static void disable_irq(unsigned int irq)
{

	return 0;
}

#define shutdown_irq		disable_irq

static void mask_and_ack_irq(unsigned int irq)
{

	return 0;
}
static void end_irq(unsigned int irq)
{
	return 0;
}

static struct hw_interrupt_type irq_type = {
	"MIPS",
	startup_irq,
	shutdown_irq,
	enable_irq,
	disable_irq,
	mask_and_ack_irq,
	end_irq,
	NULL
};

void irq_dispatch(int irq_nr, struct pt_regs *regs)
{
   int i,j,irq;
   volatile unsigned int gimr, gisr,irq_x;  
    
    gimr = inl(GIMR0);
    {	    
	gisr = inl(GISR);
	irq_x = (gimr & gisr & 0xff);
	irq=0;	
        for (i=0; i<=7; i++)
    	{
        	if (irq_x & 0x01)
			{
				do_IRQ(irq, regs);

			}  
        	irq++;
        	irq_x = irq_x >> 1;
    	}
     }
}

void __init nino_irq_setup(void)
{
	extern asmlinkage void ninoIRQ(void);

	unsigned int i;

	/* Disable all hardware interrupts */
	change_cp0_status(ST0_IM, 0x00);

	/* Initialize IRQ action handlers */
	for (i = 0; i < 16; i++) {
		hw_irq_controller *handler = NULL;
		handler		= &irq_type;

		irq_desc[i].status	= IRQ_DISABLED;
		irq_desc[i].action	= 0;
		irq_desc[i].depth	= 1;
		irq_desc[i].handler	= handler;
	}

	/* Set up the external interrupt exception vector */
	set_except_vector(0, ninoIRQ);

	/* Enable all interrupts */
	change_cp0_status(ST0_IM, ALLINTS);
}

void (*irq_setup)(void);

void __init init_IRQ(void)
{
	/* Invoke board-specific irq setup */
	irq_setup();
	
#ifdef CONFIG_REMOTE_DEBUG
	extern void breakpoint(void);
	extern void set_debug_traps(void);
	extern void rs_kgdb_hook(void);
	rs_kgdb_hook();
	clear_cp0_status(STATUSF_IP2);
	clear_cp0_status(STATUSF_IP3);
	clear_cp0_status(STATUSF_IP4);
	clear_cp0_status(STATUSF_IP5);
	clear_cp0_status(STATUSF_IP6);
	clear_cp0_status(STATUSF_IP7);	
	printk("Wait for gdb client connection ...\n");
	set_debug_traps();
	set_cp0_status(STATUSF_IP2);
	set_cp0_status(STATUSF_IP3);
	set_cp0_status(STATUSF_IP4);
	set_cp0_status(STATUSF_IP5);
	set_cp0_status(STATUSF_IP6);
	set_cp0_status(STATUSF_IP7);	
	breakpoint();
#endif	
	
}
