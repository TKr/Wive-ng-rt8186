/* netsc520.c -- MTD map driver for AMD NetSc520 Demonstration Board
 *
 * Copyright (C) 2001 Mark Langsdorf (mark.langsdorf@amd.com)
 *	based on sc520cdp.c by Sysgo Real-Time Solutions GmbH
 *
 * $Id: netsc520.c,v 1.1.1.1 2004/07/28 06:27:24 ysc Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * The NetSc520 is a demonstration board for the Elan Sc520 processor available
 * from AMD.  It has a single back of 16 megs of 32-bit Flash ROM and another
 * 16 megs of SDRAM.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>


/*
** The single, 16 megabyte flash bank is divided into four virtual
** partitions.  The first partition is 768 KiB and is intended to
** store the kernel image loaded by the bootstrap loader.  The second
** partition is 256 KiB and holds the BIOS image.  The third 
** partition is 14.5 MiB and is intended for the flash file system
** image.  The last partition is 512 KiB and contains another copy
** of the BIOS image and the reset vector.
**
** Only the third partition should be mounted.  The first partition
** should not be mounted, but it can erased and written to using the
** MTD character routines.  The second and fourth partitions should
** not be touched - it is possible to corrupt the BIOS image by
** mounting these partitions, and potentially the board will not be
** recoverable afterwards.
*/

static __u8 netsc520_read8(struct map_info *map, unsigned long ofs)
{
	return readb(map->map_priv_1 + ofs);
}

static __u16 netsc520_read16(struct map_info *map, unsigned long ofs)
{
	return readw(map->map_priv_1 + ofs);
}

static __u32 netsc520_read32(struct map_info *map, unsigned long ofs)
{
	return readl(map->map_priv_1 + ofs);
}

static void netsc520_copy_from(struct map_info *map, void *to, unsigned long from, ssize_t len)
{
	memcpy_fromio(to, (void *)(map->map_priv_1 + from), len);
}

static void netsc520_write8(struct map_info *map, __u8 d, unsigned long adr)
{
	writeb(d, map->map_priv_1 + adr);
}

static void netsc520_write16(struct map_info *map, __u16 d, unsigned long adr)
{
	writew(d, map->map_priv_1 + adr);
}

static void netsc520_write32(struct map_info *map, __u32 d, unsigned long adr)
{
	writel(d, map->map_priv_1 + adr);
}

static void netsc520_copy_to(struct map_info *map, unsigned long to, const void *from, ssize_t len)
{
	memcpy_toio((void *)(map->map_priv_1 + to), from, len);
}

/* partition_info gives details on the logical partitions that the split the 
 * single flash device into. If the size if zero we use up to the end of the
 * device. */
static struct mtd_partition partition_info[]={
    { 
	    name: "NetSc520 boot kernel", 
	    offset: 0, 
	    size: 	0xc0000
    },
    { 
	    name: "NetSc520 Low BIOS", 
	    offset: 0xc0000, 
	    size: 	0x40000
    },
    { 
	    name: "NetSc520 file system", 
	    offset: 0x100000, 
	    size: 	0xe80000
    },
    { 
	    name: "NetSc520 High BIOS", 
	    offset: 0xf80000, 
	    size: 0x80000
    },
};
#define NUM_PARTITIONS (sizeof(partition_info)/sizeof(partition_info[0]))

/*
 * If no idea what is going on here.  This is taken from the FlashFX stuff.
 */
#define ROMCS 1


#define WINDOW_SIZE	0x00100000
#define WINDOW_ADDR	0x00200000

static struct map_info netsc520_map = {
	name: "netsc520 Flash Bank",
	size: WINDOW_SIZE,
	buswidth: 4,
	read8: netsc520_read8,
	read16: netsc520_read16,
	read32: netsc520_read32,
	copy_from: netsc520_copy_from,
	write8: netsc520_write8,
	write16: netsc520_write16,
	write32: netsc520_write32,
	copy_to: netsc520_copy_to,
	map_priv_2: WINDOW_ADDR
};

#define NUM_FLASH_BANKS	(sizeof(netsc520_map)/sizeof(struct map_info))

static struct mtd_info *mymtd;

static int __init init_netsc520(void)
{
	printk(KERN_NOTICE "NetSc520 flash device: %lx at %lx\n", netsc520_map.size, netsc520_map.map_priv_2);
	netsc520_map.map_priv_1 = (unsigned long)ioremap_nocache(netsc520_map.map_priv_2, netsc520_map.size);

	if (!netsc520_map.map_priv_1) {
		printk("Failed to ioremap_nocache\n");
		return -EIO;
	}
	mymtd = do_map_probe("cfi_probe", &netsc520_map);
	if(!mymtd)
		mymtd = do_map_probe("map_ram", &netsc520_map);
	if(!mymtd)
		mymtd = do_map_probe("map_rom", &netsc520_map);

	if (!mymtd) {
		iounmap((void *)netsc520_map.map_priv_1);
		return -ENXIO;
	}
		
	mymtd->module = THIS_MODULE;
	add_mtd_partitions( mymtd, partition_info, NUM_PARTITIONS );
	return 0;
}

static void __exit cleanup_netsc520(void)
{
	if (mymtd) {
		del_mtd_partitions(mymtd);
		map_destroy(mymtd);
	}
	if (netsc520_map.map_priv_1) {
		iounmap((void *)netsc520_map.map_priv_1);
		netsc520_map.map_priv_1 = 0;
	}
}

module_init(init_netsc520);
module_exit(cleanup_netsc520);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Langsdorf <mark.langsdorf@amd.com>");
MODULE_DESCRIPTION("MTD map driver for AMD NetSc520 Demonstration Board");
