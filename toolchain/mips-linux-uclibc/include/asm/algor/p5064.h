/* p5064.h,v 1.4 2001/09/04 09:46:00 chris Exp
 *
 * p5064.h
 *
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999 MIPS Technologies, Inc.  All rights reserved.
 * Chris Dearman, chris@algor.co.uk
 * Copyright (C) 2001 Algorithmics Ltd.  All rights reserved.
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 *
 * Defines of the Algorithmics P5064 board specific address-MAP, registers, 
 * etc.
 *
 */
#ifndef _ALGOR_P5064_H
#define _ALGOR_P5064_H

#include <asm/addrspace.h>

#define P5064_ISAPORT_ADDR(x) KSEG1ADDR(P5064_PCI_IO_BASE+(x))


/* 
 * Algorithmics P5064 RTC-device indirect register access.
 */
#define P5064_RTC_ADR_REG       0x70
#define P5064_RTC_DAT_REG       0x71


/*
 * Algorithmics P5064 interrupt controller register base.
 */
#define P5064_ICTRL_REGS_BASE   (KSEG1ADDR(0x1ff90000))

/*
 * Algorithmics P5064 UART register base.
 */
#define P5064_UART0_REGS_BASE    0x3f8
#define P5064_UART1_REGS_BASE    0x2f8
#define P5064_BASE_BAUD ( 1843200 / 16 ) 

/*
 * Algorithmics P5064 display register base.
 */
#define P5064_ASCII_DISPLAY_POS_BASE     (KSEG1ADDR(0x1ff20010))


/*
 * i960 to PCI bridge controller.
 */
#define PCI_CONF_SPACE    (KSEG1ADDR(0x1ee00000)) /*   1MB: s/w configurable */
#define V96XPBC_BASE      (KSEG1ADDR(0x1ef00000)) /*  64KB: s/w configurable */

/*
 * Physical address mappings of PCI Memory and I/O space
 */

#define P5064_PCI_MEM_BASE	0x11000000
#define P5064_PCI_MEM_MAX	0x17ffffff
#define P5064_PCI_MEM_MASK	0x07ffffff

#define P5064_PCI_IO_BASE	0x1d000000
#define P5064_PCI_IO_MAX	0x1dffffff
#define P5064_PCI_IO_MASK	0x00ffffff

/*
 * Physical mapping of main memory as seen from PCI space
 */

#define	P5064_MEM_IOBUS_BASE	0x80000000

#define  P5064_PCIDEV_ETH	0
#define  P5064_PCIDEV_SCSI	1
#define  P5064_PCIDEV_PIIX	2

#endif /* !_ALGOR_P5064_H */
