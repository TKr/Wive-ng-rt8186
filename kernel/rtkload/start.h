#ifndef __RTL_START_H__
#define __RTL_START_H__

#include <linux/config.h>

//----- System Registers ------------------------------------------------
#define REG_REVISION	0xbd01010c
#define	REG_PLLMNR		0xbd010104
#define	REG_SYSCLKR		0xbd010108
#define	REG_GPEFDIR		0xbd010144
#define	REG_GPEFDATA		0xbd010140
#define	REG_MTCR0		0xbd011004
#define	REG_MCR			0xbd011000
#define	REG_MTCR1		0xbd011008
#define	REG_TKNR		0xbd010110
#define	REG_GISR		0xbd010004
#define	REG_WDTCNR		0xbd01005c
#define	REG_CDBR		0xbd010058

//----- Revision bit ----------------------------------------------------
#define VERSION_B		0x0		//0000
#define	VERSION_C		0x8		//1000
#define VERSION_D		0x4		//0100

//----- DPLL and Clock Setting ------------------------------------------
#define VAL_PLLMNR_8186C    0x35a03		// 180/108
#define VAL_SYSCLKR_8186C   0x090b		// CPU=270/1.5 MEM=270/2.5

//#define VAL_PLLMNR_8186C    0x36a04 		// 210/168
//#define VAL_SYSCLKR_8186C   0xa0b 		// CPU=420/2 MEM=420/2.5

//#define VAL_PLLMNR_8186C    0x36703		// 40*40/4=400 MHz
//#define VAL_SYSCLKR_8186C   0x0d0d		// CPU=100 MEM=100

//#define VAL_PLLMNR_8186C    0x36303		// 360MHz
//#define VAL_SYSCLKR_8186C   0x0a0c		// CPU=360/2 MEM=360/3
//#define VAL_PLLMNR_8186C    0x35f03		// 40*320/4=320 MHz
//#define VAL_PLLMNR_8186C    0x36303		// 40*360/4=360 MHz
//#define VAL_SYSCLKR_8186C   0x0d0d		// CPU=360/4=90 MEM=360/4=90
//#define VAL_SYSCLKR_8186C   0x0e0e		// CPU=360/6=60 MEM=360/6=60
//#define VAL_SYSCLKR_8186C   0x0f0f		// CPU=360/6=60 MEM=360/6=45

//----- Memory Setting --------------------------------------------------
#define	VAL_FLASH_TIMING	0x88880000
//#define	VAL_FLASH_TIMING	0x33330000	//high speed.


#if (defined(CONFIG_SKIP_ADJUST_SYSCLK) )
	#define CONFIG_DONOT_ADJUST_SYSCLK_MEMSP
#endif
//----- Others ----------------------------------------------------------
#define PCI_LOW_COUNTER	0xFFFF
#define	BOOT_ADDR		0x80100000

#endif

