/* p5064int.h,v 1.4 2001/09/04 09:46:00 chris Exp
 *
 * p5064int.h
 *
 * Kevin D. Kissell, kevink@mips.com and Carsten Langgaard, carstenl@mips.com
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
 * Defines for the Algorithmics P5064 board interrupt controller.
 *
 */
#ifndef _ALGOR_P5064INT_H
#define _ALGOR_P5064INT_H

/*
 * Interrupt sources.  The first 16 interrupt numbers are 
 * allocated to ISA interrupts allowing standard interrupt
 * numbers to be used.  The remainder correspond to bit positions
 * in the ictrl_regs entries.  Only the word-aligned
 * bytes are implemented within each "word" of register.
 */

#define P5064INT_ISA0		0
#define P5064INT_ISAKBD		1
#define P5064INT_CASCADE	2
#define P5064INT_ISACOM2	3
#define P5064INT_ISACOM1	4
#define P5064INT_ISA5		5
#define P5064INT_ISA6		6
#define P5064INT_ISA7		7
#define P5064INT_ISA8		8
#define P5064INT_ISA9		9
#define P5064INT_ISA10		10
#define P5064INT_ISA11		11
#define P5064INT_ISAPS2MOUSE	12
#define P5064INT_ISA13		13
#define P5064INT_ISAIDE0	14
#define P5064INT_ISAIDE1	15

/* "LOCINT" Interrupts */
#define P5064INT_PCIBR		(16+0)
#define P5064INT_FLP		(16+1)
#define P5064INT_KBD		(16+2)
#define P5064INT_UART0		(16+3)
#define P5064INT_UART1		(16+4)
#define P5064INT_CENT		(16+5)
#define P5064INT_NULL6		(16+6)
#define P5064INT_RTC		(16+7)

/* "PANIC" Interrupts */
#define P5064INT_DEBUG		(16+8)
#define P5064INT_PFAIL		(16+9)
#define P5064INT_BERR		(16+10)
#define P5064INT_ISANMI		(16+11)
#define P5064INT_IOPERR		(16+12)
#define P5064INT_CENTCLR	(16+13)
#define P5064INT_EWAKE		(16+14)
#define P5064INT_ECODERR	(16+15)

/* "PCIINT" Interrupts */
#define P5064INT_EMDINT		(16+16)
#define P5064INT_ETH		(16+17)
#define P5064INT_SCSI		(16+18)
#define P5064INT_USB		(16+19)
#define P5064INT_PCI0		(16+20)
#define P5064INT_PCI1		(16+21)
#define P5064INT_PCI2		(16+22)
#define P5064INT_PCI3		(16+23)

/* "ISAINT" Interrupts */
#define P5064INT_ISABR		(16+24)
#define P5064INT_IDE0		(16+25)
#define P5064INT_IDE1		(16+26)

/* Number of IRQ supported on hw interrupt 0. */
#define P5064INT_END      	(16+27)


#ifndef CAT
#ifdef __STDC__
#define __CAT(str1,str2) str1##str2
#else
#define __CAT(str1,str2) str1/**/str2
#endif
#define CAT(str1,str2) __CAT(str1,str2)
#endif

#define P5064ICTRL(r,o) \
	volatile unsigned char r; \
	volatile unsigned char CAT(filla,o); \
	volatile unsigned char CAT(fillb,o); \
	volatile unsigned char CAT(fillc,o)

struct p5064_ictrl_regs {
	P5064ICTRL(locint, 0);
	P5064ICTRL(panic, 1);
	P5064ICTRL(pciint, 2);
	P5064ICTRL(isaint, 3);
	P5064ICTRL(devxbar0, 4);
	P5064ICTRL(devxbar1, 5);
	P5064ICTRL(xpcixbar, 6);
	P5064ICTRL(isaxbar, 7);
	P5064ICTRL(lpcixbar, 8);
};

extern void p5064int_init(void);


#endif /* !_ALGOR_P5064INT_H */
