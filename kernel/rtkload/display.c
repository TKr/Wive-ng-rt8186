/* display.c 
 * 
 * This file is subject to the terms and conditions of the GNU
 * General Public License.  See the file "COPYING" in the main
 * directory of this archive for more details.
 *
 * Copyright (C) 2000, Jay Carlson
 */

/* 
 * Initialize the Helio LCD with values from the standard hardware
 * setup.
 *
 * We don't care where the screen points as long as it's on.  The
 * kernel will point it at its own framebuffer memory.
 */

#include <linux/autoconf.h>

#ifdef CONFIG_RTL8186_AP
#include <asm/rtl8186.h>
#endif

void display_on() 
{
        int i;

        //setup Line Control Register
        rtl_outb(UART_LCR, 0x83); // Divisor Latch Enable

#ifndef RTL8186
        // setup baudrate 38400
        rtl_outb(UART_DLL, 0x22);
#else
        if ((*(volatile unsigned long *)0xbd01010c) & 0x80000000) {    
    	    // C-cut in 180MHz
            rtl_outb(UART_DLM, 0x01);
	    rtl_outb(UART_DLL, 0x21);
        }
        else {    
    	    // B-cut in 160MHz
            rtl_outb(UART_DLM, 0x0);
            rtl_outb(UART_DLL, 0xf9);
        }
#endif

        // disable the DLB
        rtl_outb(UART_LCR,0x03);
        // reset FIFO
        rtl_outb(UART_FCR,0x06);
        for(i=0; i<=0x1000; i++);
            rtl_outb(UART_FCR,0x0);

        // setup AutoFlowControl
        rtl_outb(UART_MCR, 0x0);
        rtl_outb(UART_IER, 0x0);
}
