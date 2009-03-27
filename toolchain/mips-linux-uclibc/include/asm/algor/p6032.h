/* $Id: p6032.h,v 1.1 2002/11/08 14:00:21 carstenl Exp $
 *
 * p6032.h
 *
 * Chris Dearman, chris@algor.co.uk
 * Copyright (C) 2001 Algorithmics Ltd. All Rights Reserved.
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
 * Defines of the Algorithmics P6032 board specific address-MAP, registers, 
 * etc.
 *
 */
#ifndef _ALGOR_P6032_H
#define _ALGOR_P6032_H

#include <asm/addrspace.h>
#include <asm/algor/bonito.h>

#define P6032_ISAPORT_ADDR(x) KSEG1ADDR(P6032_PCI_IO_BASE+(x))


/*
 * Algorithmics P6032 display register base.
 */
#define P6032_ASCII_DISPLAY_POS_BASE     (KSEG1ADDR(BONITO_DEV_BASE+0x40000))


/*
 * Physical address mappings of PCI Memory and I/O space
 */


#define BONITO_BASE	BONITO_REG_BASE

/*
 * Physical mapping of main memory as seen from PCI space
 */

#define P6032_PCI_MEM_BASE	0x10000000
#define P6032_PCI_MEM_MAX	0x1bffffff

#define	P6032_PCI_IO_BASE	0x1fd00000
#define	P6032_PCI_IO_MAX	0x1fdfffff

#define	P6032_MEM_IOBUS_BASE	0x80000000

#define P6032_PCIDEV_SLOT1	13	/* P9/P7 */
#define P6032_PCIDEV_SLOT2	14	/* P10/P8 */
#define P6032_PCIDEV_SLOT3	15	/* P11/P6 */
#define P6032_PCIDEV_SLOT4	18	/* P8/P9 */
#define P6032_PCIDEV_ETH	16
#define P6032_PCIDEV_I82371	17
#define P6032_PCIDEV_BONITO	19

void p6032_display_message(const char *str);
void p6032_display_word(unsigned int num);
void p6032_display_flash(unsigned int on);

#endif /* !_ALGOR_P6032_H */
