/*
 *
 * Chris Dearman, chris@algor.co.uk
 * Copyright (C) 2001 Algorithmics Ltd. All rights reserved.
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
 * Defines for the Algorithmics P6032 board interrupt controller.
 *
 */
#ifndef _ALGOR_P6032INT_H
#define _ALGOR_P6032INT_H

/*
 * Interrupt sources.  The first 16 interrupt numbers are 
 * allocated to ISA interrupts allowing standard interrupt
 * numbers to be used.
 */

#define P6032INT_ISA0		0
#define P6032INT_ISAKBD		1
#define P6032INT_CASCADE	2
#define P6032INT_ISACOM2	3
#define P6032INT_ISACOM1	4
#define P6032INT_ISA5		5
#define P6032INT_ISA6		6
#define P6032INT_ISA7		7
#define P6032INT_ISA8		8
#define P6032INT_ISA9		9
#define P6032INT_ISA10		10
#define P6032INT_ISA11		11
#define P6032INT_ISAPS2MOUSE	12
#define P6032INT_ISA13		13
#define P6032INT_ISAIDE0	14
#define P6032INT_ISAIDE1	15

/* The remainder are handled by Bonito */
#define P6032INT_BONITO_BASE	16
#define P6032INT_MBOX0		16
#define P6032INT_MBOX1		17
#define P6032INT_MBOX2		18
#define P6032INT_MBOX3		19
#define P6032INT_DMARDY		20
#define P6032INT_DMAEMPTY	21
#define P6032INT_COPYRDY	22
#define P6032INT_COPYEMPTYRDY	23
#define P6032INT_COPYERR	24
#define P6032INT_PCIIRQ		25
#define P6032INT_MASTERERR	26
#define P6032INT_SYSTEMERR	27
#define P6032INT_DRAMPERR	28
#define P6032INT_MTIMEOUT	29
#define P6032INT_IRQA		32
#define P6032INT_IRQB		33
#define P6032INT_IRQC		34
#define P6032INT_IRQD		35
#define P6032INT_ISANMI		41
#define P6032INT_ISAIRQ		42
#define P6032INT_ETH		43
#define P6032INT_IDE		44
#define P6032INT_UART1		45
#define P6032INT_UART0		46

/* Special handler for CPU timer interrupt */
#define P6032INT_TIMER		48

/* Number of interrupts supported */
#define P6032INT_END      	49

extern void p6032int_init(void);

#endif /* !(_ALGOR_P6032INT_H) */
