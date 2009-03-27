/*
 *  linux/include/linux/de2fpga.h -- FPGA driver for the DragonEngine
 *
 *	Copyright (C) 2003 Georges Menie
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

#ifndef _DE2FPGA_H
#define _DE2FPGA_H

#define DE2FPGA_PARAMS_GET _IOR('F', 0, struct de2fpga_info)

#define DE2FPGA_MAX_TAGLEN 128

struct de2fpga_info {
	int status;
	int file;
	int part;
	int date;
	int time;
	char buf[DE2FPGA_MAX_TAGLEN];
};

#define DE2FPGA_STS_UNKNOWN 0
#define DE2FPGA_STS_ERROR -1
#define DE2FPGA_STS_OK 1

#endif
