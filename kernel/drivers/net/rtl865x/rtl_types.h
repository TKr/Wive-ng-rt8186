/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.                                                    
* 
* Abstract : realtek type definition
 *
 * $Author: davidhsu $
 *
 * $Log: rtl_types.h,v $
 * Revision 1.15  2009/07/29 12:29:41  davidhsu
 * Use MIPS16 for IRAM function
 *
 * Revision 1.14  2009/06/22 11:11:21  joeylin
 * define CONFIG_RTK_CHECK_ETH_TX_HANG for all packages
 *
 * Revision 1.13  2009/06/05 11:35:07  joeylin
 * add "check Ethernet Tx hang up" feature
 *
 * Revision 1.12  2009/03/23 02:43:45  joeylin
 * Enable hardware pass through support for IPv6 pass through
 *
 * Revision 1.11  2009/03/16 14:00:33  pluswang
 *
 * roll back
 *
 * Revision 1.9  2009/03/13 02:11:46  joeylin
 * add the feature: trap Ethernet LAN/WAN packets to CPU when configure in bridge mode under 8651c platform. but disabled now.
 *
 * Revision 1.8  2009/02/09 11:47:18  joeylin
 * Fine tune refill threshold
 *
 * Revision 1.7  2009/02/03 02:59:01  davidhsu
 * Fine tune refill threshold
 *
 * Revision 1.6  2009/01/22 06:18:52  joeylin
 * Add DELAY_REFILL_ETH_RX_BUF to reduce pre-allocate rx buffer
 *
 * Revision 1.5  2008/12/11 14:36:54  joeylin
 * refine the code
 *
 * Revision 1.4  2008/12/05 12:35:40  joeylin
 * add RTL865X_RX_RUNOUT_BUG flag, the code is no effect.
 *
 * Revision 1.3  2008/07/22 13:40:29  jasonwang
 * Added Wireless and Fastpath Module supportI
 *
 * Revision 1.2  2007/12/04 12:00:18  joeylin
 * add hardware NAT feature
 *
 * Revision 1.1.1.1  2007/08/06 10:04:52  root
 * Initial import source to CVS
 *
 * Revision 1.5  2005/09/22 05:22:31  bo_zhao
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2005/09/05 12:38:24  alva
 * initial import for add TFTP server
 *
 * Revision 1.4  2004/08/26 13:53:27  yjlou
 * -: remove all warning messages!
 * +: add compile flags "-Wno-implicit -Werror" in Makefile to treat warning as error!
 *
 * Revision 1.3  2004/05/12 06:35:11  yjlou
 * *: fixed the ASSERT_CSP() and ASSERT_ISR() macro: print #x will cause unpredictable result.
 *
 * Revision 1.2  2004/03/31 01:49:20  yjlou
 * *: all text files are converted to UNIX format.
 *
 * Revision 1.1  2004/03/16 06:36:13  yjlou
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2003/09/25 08:16:56  tony
 *  initial loader tree 
 *
 * Revision 1.1.1.1  2003/05/07 08:16:07  danwu
 * no message
 *
*/
#include <linux/kernel.h>


#ifndef _RTL_TYPES_H
#define _RTL_TYPES_H

#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
#include <linux/config.h>
#include <linux/module.h>
#endif
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/string.h>

/*
 * Internal names for basic integral types.  Omit the typedef if
 * not possible for a machine/compiler combination.
 */

#if 1
#define __IRAM_FWD
#define __IRAM_GEN
#define __IRAM_EXTDEV
#define __IRAM_L2_FWD
#define __DRAM_EXTDEV
#endif

/* ===============================================================================
		print macro
    =============================================================================== */
#define rtlglue_printf	printk

/*
typedef unsigned long long	uint64;
typedef long long		int64;
typedef unsigned int	uint32;
typedef int			int32;
typedef unsigned short	uint16;
typedef short			int16;
typedef unsigned char	uint8;
typedef char			int8;
*/
#define uint64 unsigned long long
#define int64 long long
#define uint32 unsigned int 
#define int32 int
#define uint16 unsigned short
#define int16 short
#define uint8 unsigned char
#define int8 char



#define UINT32_MAX	UINT_MAX
#define INT32_MIN	INT_MIN
#define INT32_MAX	INT_MAX
#define UINT16_MAX	USHRT_MAX
#define INT16_MIN	SHRT_MIN
#define INT16_MAX	SHRT_MAX
#define UINT8_MAX	UCHAR_MAX
#define INT8_MIN		SCHAR_MIN
#define INT8_MAX	SCHAR_MAX

typedef uint32		memaddr;	
typedef uint32          ipaddr_t;

typedef struct {
    uint16      mac47_32;
    uint16      mac31_16;
    uint16      mac15_0;
    uint16		align;
} macaddr_t;


typedef int8*			calladdr_t;

typedef struct ether_addr_s {
	uint8 octet[6];
} ether_addr_t;

#define RX_OFFSET 2
#define MBUF_LEN	1600
#define CROSS_LAN_MBUF_LEN		(MBUF_LEN+16)

#ifdef CONFIG_RTL8196B
	#ifdef CONFIG_RTL865X_ETH_PRIV_SKB
	#define DELAY_REFILL_ETH_RX_BUF
	#define ETH_REFILL_THRESHOLD	140 // 24	// must < NUM_RX_PKTHDR_DESC
	#endif
#endif

/* 
	CN SD6 Mantis issue #1085: NIC RX can't work correctly after runout.
	this bug still happened in RTL8196B
 */
#define  RTL865X_RX_RUNOUT_BUG

//----------------------------------------------------
//#include "rtl_depend.h"
#ifndef _RTL_DEPEND_H_
#define _RTL_DEPEND_H_


/*
 * many standard C library API references "size_t"
 * cygwin's stdio.h and ghs's stdio.h will test
 * _SIZE_T symbol before typedef ...
 */
#ifndef _SIZE_T
#define _SIZE_T
	typedef unsigned int size_t;
#endif /* _SIZE_T */

#define RTL_EXTERN_INLINE
#define RTL_STATIC_INLINE   static __inline__

#endif   /* _RTL_DEPEND_H_ */
//------
#ifdef CONFIG_WIRELESS_LAN_MODULE
#define __IRAM_SECTION_	//__attribute__ ((section(".iram-fwd")))
#define __DRAM_SECTION_	//__attribute__ ((section(".dram-fwd")))
#else
#define __IRAM_SECTION_	__attribute__ ((section(".iram-fwd")))
#define __DRAM_SECTION_	__attribute__ ((section(".dram-fwd")))
#endif

#define __MIPS16			__attribute__ ((mips16))

#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCESS
#define SUCCESS 	0
#endif
#ifndef FAILED
#define FAILED -1
#endif

#define CLEARBITS(a,b)	((a) &= ~(b))
#define SETBITS(a,b)		((a) |= (b))
#define ISSET(a,b)		(((a) & (b))!=0)
#define ISCLEARED(a,b)	(((a) & (b))==0)

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif			   /* max */

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif			   /* min */



//#define ASSERT_CSP(x) if (!(x)) {dprintf("\nAssertion fail at file %s, function %s, line number %d: (%s).\n", __FILE__, __FUNCTION__, __LINE__, #x); while(1);}
//#define ASSERT_ISR(x) if (!(x)) {printfByPolling("\nAssertion fail at file %s, function %s, line number %d: (%s).\n", __FILE__, __FUNCTION__, __LINE__, #x); while(1);}

//wei add, because we only use polling mode uart-print
extern void *memset(void *__s, int __c, size_t __count);

#define ASSERT_CSP(x) if (!(x)) {printk("\nAssertion fail!, file=%s, fun=%s, line=%d", __FILE__, __FUNCTION__, __LINE__); while(1);}
#define ASSERT_ISR(x) ASSERT_CSP(x)
#define bzero( p, s ) memset( p, 0, s )

//round down x to multiple of y.  Ex: ROUNDDOWN(20, 7)=14
#ifndef ROUNDDOWN
#define	ROUNDDOWN(x, y)	(((x)/(y))*(y))
#endif

//round up x to multiple of y. Ex: ROUNDUP(11, 7) = 14
#ifndef ROUNDUP
#define	ROUNDUP(x, y)	((((x)+((y)-1))/(y))*(y))  /* to any y */
#endif

#ifndef ROUNDUP2
#define	ROUNDUP2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#endif

#ifndef ROUNDUP4
#define	ROUNDUP4(x)		((1+(((x)-1)>>2))<<2)
#endif

#ifndef IS4BYTEALIGNED
#define IS4BYTEALIGNED(x)	 ((((x) & 0x3)==0)? 1 : 0)
#endif

#ifndef __offsetof
#define __offsetof(type, field) ((unsigned long)(&((type *)0)->field))
#endif

#ifndef offsetof
#define offsetof(type, field) __offsetof(type, field)
#endif

#ifndef RTL_PROC_CHECK
#define RTL_PROC_CHECK(expr, success) \
	do {\
			int __retval; \
			if ((__retval = (expr)) != (success))\
			{\
				rtlglue_printf("ERROR >>> [%s]:[%d] failed -- return value: %d\n", __FUNCTION__,__LINE__, __retval);\
				return __retval; \
			}\
		}while(0)
#endif

#ifndef RTL_STREAM_SAME
#define RTL_STREAM_SAME(s1, s2) \
	((strlen(s1) == strlen(s2)) && (strcmp(s1, s2) == 0))
#endif

#define RTL_STATIC_INLINE   static __inline__

#if defined(RTL865X_TEST)||defined(RTL865X_MODEL_USER)
#define UNCACHE_MASK		0
#define UNCACHE(addr)		(addr)
#define CACHED(addr)			((uint32)(addr))
#else
#define UNCACHE_MASK		0x20000000
#define UNCACHE(addr)		((UNCACHE_MASK)|(uint32)(addr))
#define CACHED(addr)			((uint32)(addr) & ~(UNCACHE_MASK))
#endif

#ifndef BIT
#define BIT(x)	(1 << (x))
#endif

#ifdef CONFIG_RTL8196B
#define ALL_PHYSICAL_PORTS 		(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4))
#else
#define ALL_PHYSICAL_PORTS 		(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5))
#endif
#define EXT_PORT_HWLOOKUP 		BIT(6)
#define EXT_PORT_WLAN 				BIT(6)	// not support yet
#define EXT_PORT_PPTPL2TP 			BIT(7)	// not support yet
#define ALL_PORTS 	(ALL_PHYSICAL_PORTS | EXT_PORT_HWLOOKUP)

//#define TWO_VLANS_IN_BRIDGE_MODE
#define CPU_PORT 		BIT(6)

#ifdef CONFIG_HW_PROTOCOL_VLAN_TBL
#define PASSTHRU_VLAN_ID 100

#define IP6_PASSTHRU_RULEID 5
#define PPPOE_PASSTHRU_RULEID1 3
#define PPPOE_PASSTHRU_RULEID2 4

#define IP6_PASSTHRU_MASK 0x1
#define PPPOE_PASSTHRU_MASK 0x1<<1

extern int oldStatus;
#endif

#define CONFIG_RTK_CHECK_ETH_TX_HANG

#endif
