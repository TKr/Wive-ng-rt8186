/* hfload.h
 *
 * This file is subject to the terms and conditions of the GNU
 * General Public License.  See the file "COPYING" in the main
 * directory of this archive for more details.
 *
 * Copyright (C) 2000, Jay Carlson
 */

#ifndef HELLOAD_H
#define HELLOAD_H

//sc_yang
#include <linux/config.h>
#ifdef CONFIG_RTL8186_AP
#include <linux/string.h>
#define printf prom_printf
#else
#include <string.h>
#endif


#ifdef LANGUAGE_C
extern int file_offset;
#endif


#ifdef COMPRESSED_KERNEL
	#define UNCOMPRESS_OUT  0x80000000
#if 0
#ifndef boot32 
	#define FREEMEM_START	0x80680000
	#define FREEMEM_END	0x80800000
#else
	#define FREEMEM_START	0x80a00000
	#define FREEMEM_END	0x81000000
#endif
#endif //sc_yang
#if defined (CONFIG_RTL8186_AP)
	#define FREEMEM_END	0x80800000
#else
	#define FREEMEM_END	0x81000000
#endif
#endif


// david -----------------
#ifdef BZ2_COMPRESS
	#define prom_printf
	#undef FREEMEM_START
	#define FREEMEM_START   0x80500000
#endif
//-----------------------   

#endif

