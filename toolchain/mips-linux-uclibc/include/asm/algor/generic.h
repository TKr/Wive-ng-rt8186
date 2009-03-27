/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
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
 * Algorithmics boards generic information.
 *
 */
#ifndef _ALGOR_GENERIC_H
#define _ALGOR_GENERIC_H

#include <asm/addrspace.h>
#include <asm/byteorder.h>

extern void (*_display_message)(const char *str);
#define algor_display_message(str) do { if (_display_message) (*_display_message)(str); } while (0)

extern void (*_display_word)(unsigned int num);
#define algor_display_word(num) do { if (_display_word) (*_display_word)(num); } while (0)

extern void (*_display_flash)(unsigned int on);
#define algor_display_flash(on) do { if (_display_flash) (*_display_flash)(on); } while (0)


extern void algor_rtc_init (void);

#endif  /* !_ALGOR_GENERIC_H */
