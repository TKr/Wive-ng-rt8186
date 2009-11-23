/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /cvs/AP/rtl865x/linux-2.4.18/drivers/net/rtl865x/swCore.c,v 1.54 2009/06/10 05:52:22 joeylin Exp $
*
* Abstract: Switch core driver source code.
*
* $Author: joeylin $
* ---------------------------------------------------------------
*/
#include <linux/autoconf.h>
#include <linux/delay.h>

#include "rtl_types.h"
#include "rtl_errno.h"
#include "asicregs.h"
#include "phy.h"
#include "swTable.h"

#include "rtl865xC_tblAsicDrv.h"
#include "rtl8651_layer2.h"
#include "swNic_poll.h"
#ifdef CONFIG_RTL8196_RTL8366
#include "RTL8366RB_DRIVER/gpio.h"
#include "RTL8366RB_DRIVER/rtl8366rb_apiBasic.h"
#endif

#define BICOLOR_LED 1

#define RTL8651_ETHER_AUTO_100FULL	0x00
#define RTL8651_ETHER_AUTO_100HALF	0x01
#define RTL8651_ETHER_AUTO_10FULL		0x02
#define RTL8651_ETHER_AUTO_10HALF	0x03
#define RTL8651_ETHER_AUTO_1000FULL	0x08
#define RTL8651_ETHER_AUTO_1000HALF	0x09

#define tick_Delay10ms(x) { int i=x; while(i--) __delay(5000); }
static int32 miiPhyAddress = -1;
extern unsigned int wan_port; 
extern const uint32 cPVCR[6][4];

#ifdef CONFIG_RTK_VOIP_865xC_QOS
#if !defined(RTL865XC_MNQUEUE_OUTPUTQUEUE) && !defined(RTL865XC_QOS_OUTPUTQUEUE)
typedef struct  {
 	uint32  ifg;					// default: Bandwidth Control Include/exclude Preamble & IFG 
	uint32	gap;					// default: Per Queue Physical Length Gap = 20 
	uint32	drop;					// default: Descriptor Run Out Threshold = 500 

	uint32	systemSBFCOFF;				//*System shared buffer flow control turn off threshold
	uint32	systemSBFCON;				//*System shared buffer flow control turn on threshold

	uint32	systemFCOFF;				// system flow control turn off threshold 
	uint32	systemFCON;				// system flow control turn on threshold 

	uint32	portFCOFF;				// port base flow control turn off threshold 
	uint32	portFCON;				// port base flow control turn on threshold 	

	uint32	queueDescFCOFF;				// Queue-Descriptor=Based Flow Control turn off Threshold  
	uint32	queueDescFCON;				// Queue-Descriptor=Based Flow Control turn on Threshold  

	uint32	queuePktFCOFF;				// Queue-Packet=Based Flow Control turn off Threshold  
	uint32	queuePktFCON;				// Queue-Packet=Based Flow Control turn on Threshold  
}	rtl865xC_outputQueuePara_t;
#endif
#endif

#define GIGA_P0_PHYID	0x16  //for P0 8211C
/*
	0: UTP [embedded PHY]
	1: others [external PHY]
 */
#ifdef CONFIG_RTL8197B_PANA
static int ExtP0GigaPhyMode=1;
#else
static int ExtP0GigaPhyMode=0;
#endif

/*
	0x00: RGMII mode,
	0x01: GMII/MII MAC auto mode
	0x02: GMII/MII PHY auto mode
	0x10: RGMII, force mode   //8196B Mode, because sw MDC/MDIO fail
	0x11: GMII/MII mac, force mode
	0x12: GMII/MII phy,  force mode
 */
static int SW_P0Mode=0x12;		//when ExtP0GigaPhyMode=1, this setting is valid

//------------------------------------------------------------------------
unsigned int read_gpio_hw_setting(void)
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
	return 0;
#endif

#if 0
	unsigned int tmp;
	int b1,b2,b3,b4;

	REG32(PEFGHCNR_REG) = REG32(PEFGHCNR_REG)  &(~(0x03<<24)) & (~(0x48<<8) ); //set (GP4 GP3 GP2 GP1)=(H1 H0 F3 F6)= gpio
	REG32(PEFGHPTYPE_REG)=REG32(PEFGHPTYPE_REG)&(~(0x03<<24)) & (~(0x48<<8) );  //change to GPIO mode
	REG32(PEFGHDIR_REG) = REG32(PEFGHDIR_REG)    &(~(0x03<<24)) & (~(0x48<<8) );;  //0 input, 1 output, set  inpur
	tmp=REG32(PEFGHDAT_REG);
	b4 = (tmp&(0x02<<24))>>25;
	b3 = (tmp&(0x01<<24))>>24;
	b2 = (tmp&(0x08<<8))>>11;
	b1 = (tmp&(0x40<<8))>>14;
	tmp = (b1&0x1) | ((b2<<1)&0x2) |  ((b3<<2)&0x4) | ((b4<<2)&0x8);	
//	printk("GPIO val=%08x\r\n",tmp); // for debug
	return tmp;	
#else
	unsigned int tmp, b2;

	/* 
		read GPIO PORT F3 to determine the LED mode, 0 LED scan mode, 1 LED matrix mode
		PEFGHCNR_REG: Port EFGH control, b.31-b.24 port H, b.23-b.16 port G, b.15-b.8 port F, b.7-b.0 port E 
		GPIOF[7]= HCI_RDY[0]
		GPIOF[6]= HCI_CS[0]
		GPIOF[5]= HCI_RDY[2]
		GPIOF[4]= HCI_RDY[1]
		GPIOF[3]= HCI_INT#  <<=========== 0 LED scan mode, 1 LED matrix mode
		GPIOF[2]= HCI_CS[2]
		GPIOF[1]= reserved
		GPIOF[0]= reserved
	*/
	REG32(PEFGHCNR_REG) = REG32(PEFGHCNR_REG) & (~(0x8<<8) ); //set (GP2)=(F3)= gpio
	REG32(PEFGHPTYPE_REG) = REG32(PEFGHPTYPE_REG) & (~(0x8<<8) );  //change to GPIO mode
	REG32(PEFGHDIR_REG) = REG32(PEFGHDIR_REG) & (~(0x8<<8) );  //0 input, 1 output, set  inpur
	tmp=REG32(PEFGHDAT_REG);
	b2 = (tmp&(0x08<<8))>>11;
	//printk(">> ................GPIO val=%08x.\n",b2); 
	return b2;	
#endif
}

#ifndef CONFIG_RTL865X_HW_TABLES
static void _rtl8651_clearSpecifiedAsicTable(uint32 type, uint32 count) 
{
    struct { uint32 _content[8]; } entry;
    uint32 idx;
    
    bzero(&entry, sizeof(entry));
    for (idx=0; idx<count; idx++)// Write into hardware
        swTable_addEntry(type, idx, &entry);

}
#endif

#define CPUSSR 		0xb8019004		/* bit 31~16: reserved */
#define _MAGIC_FORM_BOOT		0x00007DE8

void FullAndSemiReset( void )
{
#ifdef CONFIG_RTL865XC

	/* FIXME: Currently workable for FPGA, may need further modification for real chip */

	/* Perform full-reset for sw-core. */ 
	#ifdef CONFIG_RTL8196B_TLD
	if ((REG32(CPUSSR) & 0x0000ffff) != _MAGIC_FORM_BOOT) 
	#endif
	{

		REG32(SIRR) |= FULL_RST;
		tick_Delay10ms(50);
	}

	/* Enable TRXRDY */
	REG32(SIRR) |= TRXRDY;

#endif /* CONFIG_RTL865XC */
}

int32 rtl865xC_setAsicEthernetMIIMode(uint32 port, uint32 mode)
{
	if ( port != 0 && port != RTL8651_MII_PORTNUMBER )
		return FAILED;
	if ( mode != LINK_RGMII && mode != LINK_MII_MAC && mode != LINK_MII_PHY )
		return FAILED;

	if ( port == 0 )
	{
		/* MII port MAC interface mode configuration */
		WRITE_MEM32( P0GMIICR, ( READ_MEM32( P0GMIICR ) & ~CFG_GMAC_MASK ) | ( mode << LINKMODE_OFFSET ) );
	}
	else
	{
		/* MII port MAC interface mode configuration */
		WRITE_MEM32( P5GMIICR, ( READ_MEM32( P5GMIICR ) & ~CFG_GMAC_MASK ) | ( mode << LINKMODE_OFFSET ) );
	}
	return SUCCESS;

}

int32 rtl865xC_setAsicEthernetRGMIITiming(uint32 port, uint32 Tcomp, uint32 Rcomp)
{
	if ( port != 0 && port != RTL8651_MII_PORTNUMBER )
		return FAILED;
	if ( Tcomp < RGMII_TCOMP_0NS || Tcomp > RGMII_TCOMP_7NS || Rcomp < RGMII_RCOMP_0NS || Rcomp > RGMII_RCOMP_2DOT5NS )
		return FAILED;
	
	if ( port == 0 )
	{
		WRITE_MEM32(P0GMIICR, ( ( ( READ_MEM32(P0GMIICR) & ~RGMII_TCOMP_MASK ) | Tcomp ) & ~RGMII_RCOMP_MASK ) | Rcomp );
	}
	else
	{
		WRITE_MEM32(P5GMIICR, ( ( ( READ_MEM32(P5GMIICR) & ~RGMII_TCOMP_MASK ) | Tcomp ) & ~RGMII_RCOMP_MASK ) | Rcomp );
	}

	return SUCCESS;
}

int32 rtl8651_setAsicEthernetMII(uint32 phyAddress, int32 mode, int32 enabled)
{
	/* Input validation */
	if ( phyAddress < 0 || phyAddress > 31 )
		return FAILED;
	if ( mode != P5_LINK_RGMII && mode != P5_LINK_MII_MAC && mode != P5_LINK_MII_PHY )
		return FAILED;
	
	/* Configure driver level information about mii port 5 */
	if ( enabled )
	{
		if ( miiPhyAddress >= 0 && miiPhyAddress != phyAddress )
			return FAILED;

		miiPhyAddress = phyAddress;
	}
	else
	{
		miiPhyAddress = -1;
	}

	/* MII port MAC interface mode configuration */
	WRITE_MEM32( P5GMIICR, ( READ_MEM32( P5GMIICR ) & ~CFG_GMAC_MASK ) | ( mode << P5_LINK_OFFSET ) );

	return SUCCESS;
}


int32 rtl8651_getAsicEthernetPHYReg(uint32 phyId, uint32 regId, uint32 *rData)
{
	uint32 status;
	
	WRITE_MEM32( MDCIOCR, COMMAND_READ | ( phyId << PHYADD_OFFSET ) | ( regId << REGADD_OFFSET ) );

#ifdef RTL865X_TEST
	status = READ_MEM32( MDCIOSR );
#else
	do { status = READ_MEM32( MDCIOSR ); } while ( ( status & STATUS ) != 0 );
#endif

	status &= 0xffff;
	*rData = status;

	return SUCCESS;
}

int32 rtl8651_setAsicEthernetPHYReg(uint32 phyId, uint32 regId, uint32 wData)
{
	WRITE_MEM32( MDCIOCR, COMMAND_WRITE | ( phyId << PHYADD_OFFSET ) | ( regId << REGADD_OFFSET ) | wData );

#ifdef RTL865X_TEST
#else
	while( ( READ_MEM32( MDCIOSR ) & STATUS ) != 0 );		/* wait until command complete */
#endif

	return SUCCESS;
}

int32 rtl8651_restartAsicEthernetPHYNway(uint32 port, uint32 phyid)
{
	uint32 statCtrlReg0;

	/* read current PHY reg 0 */
	rtl8651_getAsicEthernetPHYReg( phyid, 0, &statCtrlReg0 );

	/* enable 'restart Nway' bit */
	statCtrlReg0 |= RESTART_AUTONEGO;

	/* write PHY reg 0 */
	rtl8651_setAsicEthernetPHYReg( phyid, 0, statCtrlReg0 );

	return SUCCESS;
}

int32 rtl8651_setAsicEthernetPHY(uint32 port, int8 autoNegotiation, uint32 advCapability, uint32 speed, int8 fullDuplex, 
	uint32 phyId, uint32 isGPHY) 
{
	uint32 statCtrlReg0, statCtrlReg4, statCtrlReg9;

	/* ====================
		Arrange PHY reg 0
	   ==================== */

	/* Read PHY reg 0 (control register) first */
	rtl8651_getAsicEthernetPHYReg(phyId, 0, &statCtrlReg0);

	if ( autoNegotiation == TRUE )	
	{
		statCtrlReg0 |= ENABLE_AUTONEGO;
	}
	else
	{
		statCtrlReg0 &= ~ENABLE_AUTONEGO;

		/* Clear speed & duplex setting */
		if ( isGPHY )
			statCtrlReg0 &= ~SPEED_SELECT_1000M;
		statCtrlReg0 &= ~SPEED_SELECT_100M;
		statCtrlReg0 &= ~SELECT_FULL_DUPLEX;

		if ( speed == 1 )	/* 100Mbps, assume 10Mbps by default */
			statCtrlReg0 |= SPEED_SELECT_100M;

		if ( fullDuplex == TRUE )
			statCtrlReg0 |= SELECT_FULL_DUPLEX;
	}

	/* =============================================================
		Arrange PHY reg 4, if GPHY, also need to arrange PHY reg 9.
	   ============================================================= */
	rtl8651_getAsicEthernetPHYReg( phyId, 4, &statCtrlReg4 );

	/* Clear all capability */
	statCtrlReg4 &= ~CAP_100BASE_MASK;

	if ( isGPHY )
	{
		rtl8651_getAsicEthernetPHYReg( phyId, 9, &statCtrlReg9 );

		/* Clear all 1000BASE capability */
		statCtrlReg9 &= ~ADVCAP_1000BASE_MASK;
	}
	else
	{
		statCtrlReg9 = 0;
	}
	
	if ( advCapability == RTL8651_ETHER_AUTO_1000FULL )
	{
		statCtrlReg9 = statCtrlReg9 | CAPABLE_1000BASE_TX_FD | CAPABLE_1000BASE_TX_HD;
		statCtrlReg4 = statCtrlReg4 | CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD;
	}
	else if ( advCapability == RTL8651_ETHER_AUTO_1000HALF )
	{
		statCtrlReg9 = statCtrlReg9 | CAPABLE_1000BASE_TX_HD;
		statCtrlReg4 = statCtrlReg4 | CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD;
	}
	else if ( advCapability == RTL8651_ETHER_AUTO_100FULL )
	{
		statCtrlReg4 = statCtrlReg4 | CAPABLE_100BASE_TX_FD | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD;
	}
	else if ( advCapability == RTL8651_ETHER_AUTO_100HALF )
	{
		statCtrlReg4 = statCtrlReg4 | CAPABLE_100BASE_TX_HD | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD;
	}
	else if ( advCapability == RTL8651_ETHER_AUTO_10FULL )
	{
		statCtrlReg4 = statCtrlReg4 | CAPABLE_10BASE_TX_FD | CAPABLE_10BASE_TX_HD;
	}
	else if ( advCapability == RTL8651_ETHER_AUTO_10HALF )
	{
		statCtrlReg4 = statCtrlReg4 | CAPABLE_10BASE_TX_HD;
	}
	else
	{
//		RTL_WARN(RTL_MSG_GENERIC, "Invalid advertisement capability!");
		return FAILED;
	}

	/* ===============================
		Set PHY reg 4.
		Set PHY reg 9 if necessary.
	   =============================== */
	rtl8651_setAsicEthernetPHYReg( phyId, 4, statCtrlReg4 );

	if ( isGPHY )
	{
		rtl8651_setAsicEthernetPHYReg( phyId, 9, statCtrlReg9 );
	}

	/* =================
		Set PHY reg 0.
	   ================= */
	rtl8651_setAsicEthernetPHYReg( phyId, 0, statCtrlReg0 );

	/* =======================================================
		Restart Nway.
		If 'Nway enable' is FALSE, ASIC won't execute Nway.
	   ======================================================= */
	rtl8651_restartAsicEthernetPHYNway(port, phyId);

	return SUCCESS;
}


int32 rtl8651_setAsicFlowControlRegister(uint32 port, uint32 enable, uint32 phyid)
{
	uint32 statCtrlReg4;

	/* Read */
	rtl8651_getAsicEthernetPHYReg( phyid, 4, &statCtrlReg4 );

	if ( enable && ( statCtrlReg4 & CAPABLE_PAUSE ) == 0 )
	{
		statCtrlReg4 |= CAPABLE_PAUSE;		
	}
	else if ( enable == 0 && ( statCtrlReg4 & CAPABLE_PAUSE ) )
	{
		statCtrlReg4 &= ~CAPABLE_PAUSE;
	}
	else
		return SUCCESS;	/* The configuration does not change. Do nothing. */

	rtl8651_setAsicEthernetPHYReg( phyid, 4, statCtrlReg4 );
	
	/* restart N-way. */
	rtl8651_restartAsicEthernetPHYNway(port, phyid);

	return SUCCESS;
}

#ifdef CONFIG_RTL8214_SUPPORT
static unsigned int _wanPort;
unsigned int ExtP1P4GigaPhyMode=0, ExtP5GigaPhyMode=0;

unsigned int ProbeP1toP4GigaPHYChip(void)
{
	unsigned int uid,tmp;
	unsigned int i;

	/* Read */
	for(i=0; i<4; i++)  //probe p1-p4
	{
		rtl8651_getAsicEthernetPHYReg( CONFIG_RTL8214_PHYID_P1+i, 2, &tmp );
		uid=tmp<<16;
		rtl8651_getAsicEthernetPHYReg( CONFIG_RTL8214_PHYID_P1+i, 3, &tmp );
		uid=uid | tmp;

		if( uid==0x001CC912 )  //0x001cc912 is 8212 two giga port , 0x001cc940 is 8214 four giga port
		{
			//printk("Find Port1-4 have 8212 PHY Chip! \r\n");
			return 1;
		}
		else if(uid==0x001CC940)
		{
			//printk("Find Port1-4 8214 PHY Chip! \r\n");
			//FixPHYChip();
			//RstGigaPhy();
			return 1;
		}
	}
	return 0;
}

unsigned int ProbeP5GigaPHYChip(void)
{
	unsigned int uid,tmp;

	/* Read */
#ifndef CONFIG_RTL8196B
	rtl8651_getAsicEthernetPHYReg( GIGA_P5_PHYID, 2, &tmp );
	uid=tmp<<16;
	rtl8651_getAsicEthernetPHYReg( GIGA_P5_PHYID, 3, &tmp );
	uid=uid | tmp;
#endif
	if( uid==0x001CC912 )  //0x001cc912 is 8212 two giga port , 0x001cc940 is 8214 four giga port
	{	//printk("Find Port5   have 8211 PHY Chip! \r\n");
		return 1;
	}	

	return 0;
}

unsigned int ProbeWanPort(int probe)
{
	if (!probe)
		return  (_wanPort);
    
	ExtP1P4GigaPhyMode=ProbeP1toP4GigaPHYChip(); //wei add,
	ExtP5GigaPhyMode=ProbeP5GigaPHYChip(); //wei add,	

	if(ExtP1P4GigaPhyMode)	//if giga appear,GP3 bit value 0=port1, 1=port5
		_wanPort = 5;	
	else
		_wanPort = 0;	
	printk("Probe external PHY status, WAN port is set to port %d.\n", _wanPort);
	return (_wanPort);    
}
#endif

#ifdef CONFIG_RTK_VOIP_865xC_QOS

void rtl865xC_set_DSCP(int SIP, int RTP) 
{
	//printk("enter rtl865xC_set_DSCP \n");
	//clear dscp priority assignment, otherwise pkt with dscp value 0 will be assign priority 1		
	WRITE_MEM32(DSCPCR0,0);
	WRITE_MEM32(DSCPCR1,0);
	WRITE_MEM32(DSCPCR2,0);
	WRITE_MEM32(DSCPCR3,0);
	WRITE_MEM32(DSCPCR4,0);
	WRITE_MEM32(DSCPCR5,0);
	WRITE_MEM32(DSCPCR6,0);	         
        rtl8651_setAsicDscpPriority(RTP, PRI7);
        rtl8651_setAsicDscpPriority(SIP, PRI7);
        //printk("left rtl865xC_set_DSCP \n"); 
} 

//0 disable, 1 enable
void Enable_VOIP_QoS(int enable){
	/*
	effect throughput
	#ifdef CONFIG_RTK_VOIP_DRIVERS_WAN_PORT_0	
		REG32(FCCR0) &=0xfffffffd;//disable Qeueu 1 flow control
		REG32(FCCR0) |=0x20;	  //enable Qeueu 5 flow control
	#endif
	
	#ifdef CONFIG_FIX_WAN_TO_4	
		REG32(FCCR1) &=0xfffffffd;
		REG32(FCCR1) |=0x20;
	#endif
	*/
	if(enable)
	{
		REG32(FCCR1) &=0xfffdffff;//disable CPU Qeueu 1 flow control	
		REG32(PSRP6_RW) &= 0xffffffbf;
		printk("Enable QoS\n");
	}
	else
	{	REG32(FCCR1) |=0x20000;
		REG32(PSRP6_RW) |=0x40;	
		printk("Disable QoS\n");
	}
}

int32 rtl865x_QoS_mechanism()
{
	rtl865xC_outputQueuePara_t	outputQueuePara = {
	1, 	// default: Bandwidth Control Include/exclude Preamble & IFG 
	20, 	// default: Per Queue Physical Length Gap = 20 
	500, 	// default: Descriptor Run Out Threshold = 500 
	
	0xd4,   //default: System shared buffer flow control turn off threshold = 0x78 
	0xf8,	//default: System shared buffer flow control turn on threshold = 0x88 

	0x14a, 	//default: system flow control turn off threshold = 0x14a
	0x158,	//default: system flow control turn on threshold = 0x158
	
	0xf8,   //default: port base flow control turn off threshold = 0xf8
	0x108,  //default: port base flow control turn on threshold = 0x108
	
	0x14, 	//Queue-Descriptor=Based Flow Control turn off Threshold =0x14 
	0x21, 	//Queue-Descriptor=Based Flow Control turn on Threshold = 0x21 
	
	0x03, 	// Queue-Packet=Based Flow Control turn off Threshold = 0x03 
	0x05	// Queue-Packet=Based Flow Control turn on Threshold =0x05 
	};
  	
    	REG32(QNUMCR) = P0QNum_6 | P1QNum_6 | P2QNum_6 | P3QNum_6 | P4QNum_6| P6QNum_6;

	printk("Qos threshold Start \n");
	/* Set the threshold value for qos sytem */
	int32 retval;
	int32	i,j;
	retval = rtl8651_setAsicSystemBasedFlowControlRegister(outputQueuePara.systemSBFCON, outputQueuePara.systemSBFCOFF, outputQueuePara.systemFCON, outputQueuePara.systemFCOFF, outputQueuePara.drop);
	if (retval!= SUCCESS)
	{
		rtlglue_printf("Set System Base Flow Control Para Error.\n");
		return retval;
	}
	for(i =0; i < RTL8651_OUTPUTQUEUE_SIZE; i++)
	{
		retval = rtl8651_setAsicQueueDescriptorBasedFlowControlRegister(0, i, outputQueuePara.queueDescFCON, outputQueuePara.queueDescFCOFF);
		if (retval!= SUCCESS)
		{
			rtlglue_printf("Set Queue Descriptor Base Flow Control Para Error.\n");
			return retval;
		}
		for(j=1;j<=CPU;j++)
			rtl8651_setAsicQueueDescriptorBasedFlowControlRegister(PHY0+j, i, outputQueuePara.queueDescFCON, outputQueuePara.queueDescFCOFF);


		retval = rtl8651_setAsicQueuePacketBasedFlowControlRegister(0, i, outputQueuePara.queuePktFCON, outputQueuePara.queuePktFCOFF);
		if (retval!= SUCCESS)
		{
			rtlglue_printf("Set Queue Packet Base Flow Control Para Error.\n");
			return retval;
		}
		for(j=1;j<=CPU;j++)
			rtl8651_setAsicQueuePacketBasedFlowControlRegister(PHY0+j, i, outputQueuePara.queuePktFCON, outputQueuePara.queuePktFCOFF);

	}
// add for CPU port
	REG32(0xbb804570)= 0x140021;
	REG32(0xbb804574)= 0x140021;
	REG32(0xbb804578)= 0x140021;
	
	REG32(0xbb8045c4) = 0x038005;
	REG32(0xbb8045c8) = 0x038005;
	REG32(0xbb8045cc) = 0x038005;
		
	retval = rtl8651_setAsicPortBasedFlowControlRegister(0, outputQueuePara.portFCON, outputQueuePara.portFCOFF);
	if (retval!= SUCCESS)
	{
		rtlglue_printf("Set Port Base Flow Control Para Error.\n");
		return retval;
	}
	for(j=1;j<=CPU;j++)
		rtl8651_setAsicPortBasedFlowControlRegister(PHY0+j, outputQueuePara.portFCON, outputQueuePara.portFCOFF);
// add for CPU port	
	REG32(0xbb804524)= 0xf80108;
	retval = rtl8651_setAsicPerQueuePhysicalLengthGapRegister(outputQueuePara.gap);
	if (retval!= SUCCESS)
	{
		rtlglue_printf("Set Queue Physical Lenght Gap Reg Error.\n");
		return retval;
	}

	//clear dscp priority assignment, otherwise pkt with dscp value 0 will be assign priority 1		
	WRITE_MEM32(DSCPCR0,0);
	WRITE_MEM32(DSCPCR1,0);
	WRITE_MEM32(DSCPCR2,0);
	WRITE_MEM32(DSCPCR3,0);
	WRITE_MEM32(DSCPCR4,0);
	WRITE_MEM32(DSCPCR5,0);
	WRITE_MEM32(DSCPCR6,0);			

	WRITE_MEM32(QRR,QRST);	

	//for 802.1Q mapping => Priority 1Q == absolutely priority 
	WRITE_MEM32(LPTM8021Q,0xFAC688);
	
	//For CPU port: Priority to Qid	
	WRITE_MEM32(CPUQIDMCR0,0x55432001);
	
	// open all port Queue 5 flow control
	WRITE_MEM32(FCCR0, 0x3f3f3f3f);
	WRITE_MEM32(FCCR1, 0x3f3f3f3f);
	
	printk("Qos threshold End \n");

	
	return SUCCESS;
}
#endif

#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
extern int L2_table_disabled;
#endif

int32 swCore_init(int VlanCount)
{
#ifdef CONFIG_RTL865XC
	uint32 rev, chip_id;
	int port;

	if (VlanCount == -2) {
		_rtl8651_clearSpecifiedAsicTable(TYPE_L2_SWITCH_TABLE, RTL8651_L2TBL_ROW*RTL8651_L2TBL_COLUMN);
		_rtl8651_clearSpecifiedAsicTable(TYPE_ARP_TABLE, RTL8651_ARPTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_L3_ROUTING_TABLE, RTL8651_ROUTINGTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_MULTICAST_TABLE, RTL8651_IPMULTICASTTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_NETINTERFACE_TABLE, RTL865XC_NETINTERFACE_NUMBER);
		_rtl8651_clearSpecifiedAsicTable(TYPE_VLAN_TABLE, RTL865XC_VLAN_NUMBER);
		_rtl8651_clearSpecifiedAsicTable(TYPE_EXT_INT_IP_TABLE, RTL8651_IPTABLE_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_ALG_TABLE, RTL8651_ALGTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_SERVER_PORT_TABLE, RTL8651_SERVERPORTTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_L4_TCP_UDP_TABLE, RTL8651_TCPUDPTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_L4_ICMP_TABLE, RTL8651_ICMPTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_PPPOE_TABLE, RTL8651_PPPOE_NUMBER);
		_rtl8651_clearSpecifiedAsicTable(TYPE_ACL_RULE_TABLE, RTL8651_ACLTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_NEXT_HOP_TABLE, RTL8651_NEXTHOPTBL_SIZE);
		_rtl8651_clearSpecifiedAsicTable(TYPE_RATE_LIMIT_TABLE, RTL8651_RATELIMITTBL_SIZE);		
		return 0;
	}
	
	if (VlanCount == -1) {
		_rtl8651_clearSpecifiedAsicTable(TYPE_NETINTERFACE_TABLE, RTL865XC_NETINTERFACE_NUMBER);
		return 0;			
	}

#ifdef CONFIG_RTL865X_HW_TABLES
    
	/*
	 * 8211 Giga port compatibility issue:
	 * after test Chariot (3D+3U) for one to two minutes, then issue reboot in console,
	 * sometimes the 8211 Giga port will not work (LAN PC shows 100Mbps, 
	 *      Target's LED is incorrect, ping 192.168.1.254 is failed)
	 * this issue is easy to happen when the LAN PC's Gigabit Ethernet is Marvell's.
	 * 
	 * do the external PHY reset and restart auto-negotiation to fix it.
	 */
#ifndef CONFIG_RTL8196B
	rtl8651_setAsicEthernetPHYReg(GIGA_P5_PHYID, 0, (PHY_RESET | ENABLE_AUTONEGO));		
	rtl8651_restartAsicEthernetPHYNway(5, GIGA_P5_PHYID);		
#endif

	/*
	 * after switch operation mode from Gateway mode to Bridge/WISP mode,
	 * ping br0(192.168.1.254) with 32 bytes is okay, 
	 * but ping br0(192.168.1.254) with 65500 bytes is failed.
	 * the fragmented packets sent from 192.168.1.254 have no Ethernet header and IP header,
	 * they are started with ping payload, like:
	 *      0000   75 76 77 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d  uvwabcdefghijklm
	 *      0010   6e 6f 70 71 72 73 74 75 76 77 61 62 63 64 65 66  nopqrstuvwabcdef
	 *      0020   67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76  ghijklmnopqrstuv
	 *      0030   77 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f  wabcdefghijklmno
	 *      0040   70 71 72 73 74 75 76 77 61 62 63 64 65 66 67 68  pqrstuvwabcdefgh
	 *      0050   69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 61  ijklmnopqrstuvwa
	 *      0060   62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71  bcdefghijklmnopq
	 *
	 * after do the switch semi-reset, the issue is fixed
	 */
	/* Perform the semi-reset */
#ifndef CONFIG_RTL8196B
	REG32(SIRR) |= SEMI_RST;
	tick_Delay10ms(10);        

#endif
#else
	/* Full reset and semreset */
	FullAndSemiReset();

	/* rtl8651_clearAsicAllTable */
	_rtl8651_clearSpecifiedAsicTable(TYPE_L2_SWITCH_TABLE, RTL8651_L2TBL_ROW*RTL8651_L2TBL_COLUMN);
	_rtl8651_clearSpecifiedAsicTable(TYPE_ARP_TABLE, RTL8651_ARPTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_L3_ROUTING_TABLE, RTL8651_ROUTINGTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_MULTICAST_TABLE, RTL8651_IPMULTICASTTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_NETINTERFACE_TABLE, RTL865XC_NETINTERFACE_NUMBER);
	_rtl8651_clearSpecifiedAsicTable(TYPE_VLAN_TABLE, RTL865XC_VLAN_NUMBER);
	_rtl8651_clearSpecifiedAsicTable(TYPE_EXT_INT_IP_TABLE, RTL8651_IPTABLE_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_ALG_TABLE, RTL8651_ALGTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_SERVER_PORT_TABLE, RTL8651_SERVERPORTTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_L4_TCP_UDP_TABLE, RTL8651_TCPUDPTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_L4_ICMP_TABLE, RTL8651_ICMPTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_PPPOE_TABLE, RTL8651_PPPOE_NUMBER);
	_rtl8651_clearSpecifiedAsicTable(TYPE_ACL_RULE_TABLE, RTL8651_ACLTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_NEXT_HOP_TABLE, RTL8651_NEXTHOPTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_RATE_LIMIT_TABLE, RTL8651_RATELIMITTBL_SIZE);
#endif /* CONFIG_RTL865X_HW_TABLES */
    

	chip_id = (READ_MEM32(CVIDR)) >> 16;

	rev = ((READ_MEM32(CRMR)) >> 12) & 0x0f ;/*Runtime determined patch for A cut revison. RLRevID_OFFSET = 12, RLRevID_MASK = 0x0f */
	if ((chip_id != 0x8196) && (rev < 0x01)) {
		rev = READ_MEM32((SYSTEM_BASE+0x3400+0x08));
		if(rev == 0x00)/*A Cut patch RTL865X_CHIP_REV_A = 0x00*/{
			REG32(PCRP6) = (6 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
		}
	}   

#ifdef CONFIG_RTL8214_SUPPORT
	if(ExtP1P4GigaPhyMode)
		rtl865xC_setAsicEthernetMIIMode(0, LINK_RGMII); //wei add for 8652 demo board
	if(ExtP5GigaPhyMode)	
#endif
	{
#ifdef CONFIG_RTL865X_PANAHOST
        rtl865xC_setAsicEthernetMIIMode(RTL8651_MII_PORTNUMBER, LINK_MII_MAC); //Port 5 MII MAC mode
#else
#ifndef CONFIG_RTL8196B
		rtl865xC_setAsicEthernetMIIMode(RTL8651_MII_PORTNUMBER, LINK_RGMII);
#endif
#endif
	}

	/*
		# According to Hardware SD: David & Maxod,			
		Set Port5_GMII Configuration Register.
		- RGMII Output Timing compensation control : 0 ns
		- RGMII Input Timing compensation control : 0 ns
	*/
#ifdef CONFIG_RTL8214_SUPPORT
	if(ExtP1P4GigaPhyMode)	
		rtl865xC_setAsicEthernetRGMIITiming(0, RGMII_TCOMP_0NS, RGMII_RCOMP_0NS); //wei add, for 8652 demo board	
	if(ExtP5GigaPhyMode)
#endif
	{
#ifdef CONFIG_RTL865X_PANAHOST
        rtl865xC_setAsicEthernetRGMIITiming(RTL8651_MII_PORTNUMBER, (1<<4), RGMII_RCOMP_0NS);
		rtl8651_setAsicEthernetMII(GIGA_P5_PHYID, LINK_MII_MAC, TRUE);
#else
#ifndef CONFIG_RTL8196B
		rtl865xC_setAsicEthernetRGMIITiming(RTL8651_MII_PORTNUMBER, RGMII_TCOMP_0NS, RGMII_RCOMP_0NS);
		rtl8651_setAsicEthernetMII(GIGA_P5_PHYID, P5_LINK_RGMII, TRUE);
#endif
#endif
	}

	/*
	 * do not modify PVCR0 ~ PVCR3 (vid) and MSCR (EN_L2/EN_L3/EN_L4) when CONFIG_RTL865X_HW_TABLES
	 * is defined.
	 * they will be set by rtl865x_lightRomeConfig().
	 */
#ifndef CONFIG_RTL865X_HW_TABLES
#ifdef CONFIG_RTL8196_RTL8366
	{
	extern int savOP_MODE_value;
	int32 retval = 0;
	int iport;
	rtl8366rb_vlanConfig_t vlancfg_8366;
		if(savOP_MODE_value==2)
		{
			/* for lan */
			memset(&vlancfg_8366, 0, sizeof(rtl8366rb_vlanConfig_t));
			vlancfg_8366.fid = 0;
			vlancfg_8366.mbrmsk = (RTL8366RB_LANPORT&RTL8366RB_PORTMASK)|RTL8366RB_GMIIPORT;
			vlancfg_8366.untagmsk = vlancfg_8366.mbrmsk&(~RTL8366RB_GMIIPORT);
			vlancfg_8366.vid = RTL_LANVLANID;
			retval = rtl8366rb_setVlan(&vlancfg_8366);
			/*need set pvid??*/
			for(iport=0;iport<8;iport++)
				if  ((1<<iport)&vlancfg_8366.mbrmsk)
				{
					retval = rtl8366rb_setVlanPVID(iport, vlancfg_8366.vid, 0);
				}
		
			/* for wan */
			iport=0;
			memset(&vlancfg_8366, 0, sizeof(rtl8366rb_vlanConfig_t));
			retval = 0;

			vlancfg_8366.fid = 0;
			vlancfg_8366.mbrmsk = (RTL8366RB_WANPORT&RTL8366RB_PORTMASK)|RTL8366RB_GMIIPORT;
			vlancfg_8366.untagmsk = vlancfg_8366.mbrmsk&(~RTL8366RB_GMIIPORT);
			vlancfg_8366.vid = RTL_WANVLANID;
			retval = rtl8366rb_setVlan(&vlancfg_8366);
			/*need set pvid??*/
			for(iport=0;iport<8;iport++)
				if  ((1<<iport)&vlancfg_8366.mbrmsk)
				{
					retval = rtl8366rb_setVlanPVID(iport, vlancfg_8366.vid, 0);
				}
		}
		else
		{
			{
                        memset(&vlancfg_8366, 0, sizeof(rtl8366rb_vlanConfig_t));
                        vlancfg_8366.fid = 0;
                        vlancfg_8366.mbrmsk = ((RTL8366RB_WANPORT|RTL8366RB_LANPORT)&RTL8366RB_PORTMASK)|RTL8366RB_GMIIPORT;
                        vlancfg_8366.untagmsk = vlancfg_8366.mbrmsk&(~RTL8366RB_GMIIPORT);
                        vlancfg_8366.vid = RTL_LANVLANID;
                        retval = rtl8366rb_setVlan(&vlancfg_8366);
                        /*need set pvid??*/
                        for(iport=0;iport<8;iport++)
                                if  ((1<<iport)&vlancfg_8366.mbrmsk)
                                {
                                        retval = rtl8366rb_setVlanPVID(iport, vlancfg_8366.vid, 0);
                                }
                	}
	                {
	                        /* for wan clear it */
	                        int iport;
	                        rtl8366rb_vlanConfig_t vlancfg_8366;

	                        vlancfg_8366.fid = 0;
	                        vlancfg_8366.mbrmsk = 0;
	                        vlancfg_8366.untagmsk = 0;
	                        vlancfg_8366.vid = RTL_WANVLANID;
	                        retval = rtl8366rb_setVlan(&vlancfg_8366);
	                }
			
		}
	}

	REG32(PVCR0) = (LAN_VID << 16) | LAN_VID;
	REG32(PVCR1) = (LAN_VID << 16) | LAN_VID;
	REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
	REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;

#elif defined(CONFIG_RTL8196B_GW_MP) || defined(CONFIG_RTL8196B_AP_ROOT)
	REG32(PVCR0) = (LAN_VID << 16) | LAN_VID;
	REG32(PVCR1) = (LAN_VID << 16) | LAN_VID;
	REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
	REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;

#else
	if(VlanCount==1)    
	{
		//wei add, original all lan use VID=9
		/* Set PVID of all ports to 9 */
		REG32(PVCR0) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR1) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;
	}
	else if(VlanCount==2)
	{
		REG32(PVCR0) = cPVCR[wan_port][0];
		REG32(PVCR1) = cPVCR[wan_port][1];
		REG32(PVCR2) = cPVCR[wan_port][2];
		REG32(PVCR3) = cPVCR[wan_port][3];
	}

#if defined(CONFIG_RTK_GUEST_ZONE) 
	else
	{
		REG32(PVCR0) = (LAN_VID << 16) | WAN_VID;
		REG32(PVCR1) = (LAN3_VID << 16) | LAN2_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN4_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;
	}
#endif

#if defined(CONFIG_RTK_VLAN_SUPPORT)
	else
	{
		REG32(PVCR0) = (LAN4_VID << 16) | WAN_VID;
		REG32(PVCR1) = (LAN2_VID << 16) | LAN3_VID;
		REG32(PVCR2) = (LAN_VID << 16) | LAN_VID;
		REG32(PVCR3) = (LAN_VID << 16) | LAN_VID;		
	}
#endif	
#endif
#endif

#ifdef CONFIG_RTL8214_SUPPORT
	if(ExtP1P4GigaPhyMode)
	{
		//P5 P4 P3 P2 P1 P0, offset: 10,8,6,4,2,0  
		REG32(PITCR)=(0<<10) | (0x01 <<8) | (0x01<<6) | (0x01<<4) | (0x01 <<2)  | (0x00<<0 ); //set P0:reserver,  P1-P4:serdes, P5:RGMII

		REG32(PCRP1) = (CONFIG_RTL8214_PHYID_P1 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
		REG32(PCRP2) = ((CONFIG_RTL8214_PHYID_P1+1) << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
		REG32(PCRP3) = ((CONFIG_RTL8214_PHYID_P1+2) << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
		REG32(PCRP4) = ((CONFIG_RTL8214_PHYID_P1+3) << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
	}
	else
#endif
	{
#ifdef CONFIG_RTL8197B_PANA
        REG32(PCRP1) = EnForceMode & (~ForceLink) & (~EnablePHYIf);
        REG32(PCRP2) = EnForceMode & (~ForceLink) & (~EnablePHYIf);
        REG32(PCRP3) = (3 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
        REG32(PCRP4) = EnForceMode & (~ForceLink) & (~EnablePHYIf);
#else
// advice by alpha
		//REG32(PCRP0) = (0 << ExtPHYID_OFFSET) | AcptMaxLen_16K | IPMSTP_PortST_MASK | EnablePHYIf;
		REG32(PCRP1) = (1 << ExtPHYID_OFFSET) | AcptMaxLen_16K | IPMSTP_PortST_MASK | EnablePHYIf;
		REG32(PCRP2) = (2 << ExtPHYID_OFFSET) | AcptMaxLen_16K | IPMSTP_PortST_MASK | EnablePHYIf;
		REG32(PCRP3) = (3 << ExtPHYID_OFFSET) | AcptMaxLen_16K | IPMSTP_PortST_MASK | EnablePHYIf;
		REG32(PCRP4) = (4 << ExtPHYID_OFFSET) | AcptMaxLen_16K | IPMSTP_PortST_MASK | EnablePHYIf;
#endif

		if(ExtP0GigaPhyMode==0)
		{
			//REG32(PITCR) = REG32(PITCR) | (0<<0);  //default, embedded PHY
			REG32(PCRP0) = (0 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
		}
		else if(ExtP0GigaPhyMode==1)
		{
			//unsigned int temp;

			REG32(PITCR) = REG32(PITCR) | (1<<0);
			rtl865xC_setAsicEthernetRGMIITiming(0, (0<<4), RGMII_RCOMP_2DOT5NS); //wei add, for 8652 demo board	
			//rtl865xC_setAsicEthernetRGMIITiming(0, RGMII_TCOMP_7NS, RGMII_RCOMP_0NS); //wei add, for 8652 demo board	
		
			//8211C auto learning
			//rtl8651_getAsicEthernetPHYReg(GIGA_P0_PHYID, 3, &temp);
			
			if( (SW_P0Mode&0x0f)==0)  // RGMII mode
			{
				rtl865xC_setAsicEthernetMIIMode(0, LINK_RGMII); //wei add for 8652 demo board
			}
			else if( (SW_P0Mode&0x0f)==1)  //MII MAC  mode
			{
				rtl865xC_setAsicEthernetMIIMode(0, LINK_MII_MAC); //wei add for 8652 demo board
			}
			else if( (SW_P0Mode&0x0f)==2)  //MII PHY mode
			{
				rtl865xC_setAsicEthernetMIIMode(0, LINK_MII_PHY); //wei add for 8652 demo board
			}		

			if( (SW_P0Mode&0xf0)==0)  
			{
				REG32(PCRP0) = (GIGA_P0_PHYID<< ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf;
			}
			else if( (SW_P0Mode&0xf0)==0x10)  //GMII / MII mode
			{			
				REG32(PCRP0) = 0 | (GIGA_P0_PHYID<<ExtPHYID_OFFSET) |
							EnForceMode| ForceLink|ForceSpeed10M |ForceDuplex |
							MIIcfg_RXER | AcptMaxLen_16K|EnablePHYIf   ;			
			}						
		}	
		
	}

#ifdef CONFIG_RTL8214_SUPPORT
	if(ExtP5GigaPhyMode)	
#endif
	{
#ifdef CONFIG_RTL865X_PANAHOST
        REG32(PCRP5)= 0 | (GIGA_P5_PHYID<<ExtPHYID_OFFSET) |
							EnForceMode| ForceLink|ForceSpeed100M |ForceDuplex |
							MIIcfg_RXER | 				
							AcptMaxLen_16K|EnablePHYIf;
#else
#ifndef CONFIG_RTL8196B
		REG32(PCRP5) = (GIGA_P5_PHYID<<ExtPHYID_OFFSET)|AcptMaxLen_16K|IPMSTP_PortST_MASK |EnablePHYIf;
#endif
#endif
	}

	/* Set forwarding status */
#ifdef CONFIG_RTL8197B_PANA
	REG32(PCRP0) = (REG32(PCRP0) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
	REG32(PCRP1) = (REG32(PCRP1) & ~STP_PortST_MASK) | STP_PortST_DISABLE;
	REG32(PCRP2) = (REG32(PCRP2) & ~STP_PortST_MASK) | STP_PortST_DISABLE;
	REG32(PCRP3) = (REG32(PCRP3) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
	REG32(PCRP4) = (REG32(PCRP4) & ~STP_PortST_MASK) | STP_PortST_DISABLE;
#else
	REG32(PCRP0) = (REG32(PCRP0) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
	REG32(PCRP1) = (REG32(PCRP1) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
	REG32(PCRP2) = (REG32(PCRP2) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
	REG32(PCRP3) = (REG32(PCRP3) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
	REG32(PCRP4) = (REG32(PCRP4) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
#ifndef CONFIG_RTL8196B
	REG32(PCRP5) = (REG32(PCRP5) & ~STP_PortST_MASK) | STP_PortST_FORWARDING;
#endif		
#endif
	rev = ((READ_MEM32(CRMR)) >> 12) & 0x0f ;/*Runtime determined patch for A cut revison. RLRevID_OFFSET = 12, RLRevID_MASK = 0x0f */
	if ((chip_id != 0x8196) && (rev < 0x01))
	{
		rev = READ_MEM32((SYSTEM_BASE+0x3400+0x08));
		if(rev == 0x00)/*A Cut patch RTL865X_CHIP_REV_A = 0x00*/
		{
			REG32(PCRP6) = (6 << ExtPHYID_OFFSET) | AcptMaxLen_16K | IPMSTP_PortST_MASK |EnablePHYIf;
		}
	}

#if defined(CONFIG_RTL865X_HW_TABLES) || defined(CONFIG_HW_MULTICAST_TBL)
	// Add for ip-multicast forward
	REG32(PCRP0) = (REG32(PCRP0) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;
	REG32(PCRP1) = (REG32(PCRP1) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;	
	REG32(PCRP2) = (REG32(PCRP2) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;
	REG32(PCRP3) = (REG32(PCRP3) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;
	REG32(PCRP4) = (REG32(PCRP4) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;
	#ifndef CONFIG_RTL8196B
	REG32(PCRP5) = (REG32(PCRP5) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;
	#endif
	REG32(PCRP6) = (REG32(PCRP6) & ~IPMSTP_PortST_MASK) | IPMSTP_PortST_FORWARDING;
#endif

#ifdef CONFIG_HW_MULTICAST_TBL
	WRITE_MEM32( FFCR, READ_MEM32( FFCR ) | EN_MCAST | EN_UNUNICAST_TOCPU | EN_UNMCAST_TOCPU);
	WRITE_MEM32(CSCR, (READ_MEM32(CSCR) & ~(ALLOW_L2_CHKSUM_ERR|ALLOW_L3_CHKSUM_ERR|ALLOW_L4_CHKSUM_ERR)) | (EN_ETHER_L3_CHKSUM_REC | EN_ETHER_L4_CHKSUM_REC));
	UPDATE_MEM32(ALECR, 1522, MultiCastMTU_MASK, MultiCastMTU_OFFSET);
#endif

	#ifndef CONFIG_RTL865X_HW_TABLES
	/* Enable L2 lookup engine and spanning tree functionality */
	REG32(MSCR) = EN_L2;
	#endif
    
	REG32(QNUMCR) = P0QNum_1 | P1QNum_1 | P2QNum_1 | P3QNum_1 | P4QNum_1;

	#ifdef CONFIG_RTK_VOIP_865xC_QOS
	rev = rtl865x_QoS_mechanism();
	if(rev!= SUCCESS)
	{
		rtlglue_printf("rtl865x_QoS_mechanism Error.\n");
		return rev;
	}
	#endif

	/* Start normal TX and RX */
	REG32(SIRR) |= TRXRDY;

	/* Init PHY LED style */
#ifdef BICOLOR_LED
#ifdef CONFIG_RTL8196B
#ifdef CONFIG_RTL8197B_PANA
    REG32(0xb8000030) = REG32(0xb8000030) & (~0x00020000); //bit17, port 3 led
#else
	/*
	 * 0xb8000030: System pin mux selection
	 * 	change to LED-SW mode from DBG mode (chip initial value is DBG mode)
	 */
	//REG32(0xb8000030) = 0x0;
#endif
#else
	if(read_gpio_hw_setting())	// 1 means using matrix mode
	{
	 	REG32(LEDCR)  = 0x155500;
	}

	REG32(TCR0) = 0x000002c2;
	REG32(SWTAA) = PORT5_PHY_CONTROL;
	REG32(SWTACR) = ACTION_START | CMD_FORCE;
	while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */
#endif	
#else
	#if defined(BICOLOR_LED_VENDOR_BXXX)
	REG32(LEDCR) |= 0x00080000;

	REG32(PABCNR) &= ~0xc01f0000; /* set port a-7/6 & port b-4/3/2/1/0 to gpio */
	REG32(PABDIR) |=  0x401f0000; /* set port a-6 & port b-4/3/2/1/0 gpio direction-output */
	REG32(PABDIR) &= ~0x80000000; /* set port a-7 gpio direction-input */
	#else /* BICOLOR_LED_VENDOR_BXXX */
	REG32(LEDCR) = 0x00000000;
	REG32(TCR0) = 0x000002c7;
	REG32(SWTAA) = PORT5_PHY_CONTROL;
	REG32(SWTACR) = ACTION_START | CMD_FORCE;
	while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */
	#endif /* BICOLOR_LED_VENDOR_BXXX */
#endif

	#ifdef CONFIG_RTL8196B_TLD
	if ((REG32(CPUSSR) & 0x0000ffff) != _MAGIC_FORM_BOOT) 
	#endif
	{

	/*PHY FlowControl. Default enable*/
	for(port=0;port<MAX_PORT_NUMBER;port++)
	{

		/* Set Flow Control capability. */
#ifdef CONFIG_RTL8214_SUPPORT
		if((port>=1) && (port <=4) && ExtP1P4GigaPhyMode)
			rtl8651_setAsicFlowControlRegister(port+1, TRUE, CONFIG_RTL8214_PHYID_P1+port-1);				
		else if ((port == MAX_PORT_NUMBER-1 ) && ExtP5GigaPhyMode)
			rtl8651_setAsicFlowControlRegister(RTL8651_MII_PORTNUMBER, TRUE, GIGA_P5_PHYID);
		else				
			rtl8651_restartAsicEthernetPHYNway(port+1, port);				
#else

	#ifndef CONFIG_RTL8196B
		if (port == MAX_PORT_NUMBER-1)
			rtl8651_setAsicFlowControlRegister(RTL8651_MII_PORTNUMBER, TRUE, GIGA_P5_PHYID);
		else				
	#endif
			rtl8651_restartAsicEthernetPHYNway(port+1, port);				
#endif
	}
	}



	#if defined(CONFIG_RTL865X_DIAG_LED)
	/* diagnosis led (gpio-porta-6) on */
	REG32(PABDAT) |=  0x40000000; /* pull high by set portA-0(bit 30) as gpio-output-1, meaning: diag led OFF */
	#endif /* CONFIG_RTL865X_DIAG_LED */

#ifdef CONFIG_RTL8214_SUPPORT
	if(ExtP5GigaPhyMode)
#endif
	{
		REG32(MDCIOCR) = 0x96181441;	// enable Giga port 8211B LED
	}

	REG32(FFCR) = EN_UNUNICAST_TOCPU | EN_UNMCAST_TOCPU; // rx broadcast and unicast packet

#ifdef CONFIG_RTL8196_RTL8366
	{
		rtl865xC_setAsicEthernetRGMIITiming(0, RGMII_TCOMP_4NS, RGMII_RCOMP_2NS);
		WRITE_MEM32(PITCR, Port0_TypeCfg_GMII_MII_RGMII);
		WRITE_MEM32(PCRP0, 0xe80367);		/* force mode 1G */
	}
#endif


	#if defined(CONFIG_RTK_GUEST_ZONE) || defined(CONFIG_RTK_VLAN_SUPPORT)
	addACLRule(0);

	if (L2_table_disabled)
	  	REG32(MSCR) |= EN_IN_ACL;
	else 
	  	REG32(MSCR) &= (~EN_IN_ACL);
	#endif

	#ifdef CONFIG_HW_PROTOCOL_VLAN_TBL
	rtl8651_defineProtocolBasedVLAN( IP6_PASSTHRU_RULEID, 0x0, 0x86DD );
	updateProtocolBasedVLAN();
	#endif
	
	return 0;
#endif /* CONFIG_RTL865XC */

}

void swCore_start(void)
{
	REG32(CPUICR) = TXCMD | RXCMD | BUSBURST_32WORDS | MBUF_2048BYTES;
	REG32(CPUIIMR) = RX_DONE_IE_ALL | TX_ALL_DONE_IE_ALL | LINK_CHANGE_IE;	
	REG32(SBFCR2) = 0xd400f8; // adjust internal buffer threshold
	REG32(ELBTTCR) = 0x00000400;/*mark_patch for correcting the Leaky bucket value*/	
}

void swCore_down(void)
{
	REG32(CPUICR) = 0; 
	REG32(CPUIIMR) = 0;        	
	REG32(SIRR) = 0;
}
	
#ifdef _L2_MODE_
void DumpAsicRegisters()
{
    uint32 count;
    uint32* adr;
    uint32 len;
    
    /* Display registers */
    
    count = 0;
    adr = (uint32*) 0xbc802000;
    len = 64;
    printf( "\nAddr: %p -----------------------------------", adr );
    while (len--)
    {
        if (count == 0)
            printf("\n0x%p: ", adr);
        if ( ++count == 4 )
            count = 0;
            
        printf("%08x ", *(adr++));
    }
    
    count = 0;
    adr = (uint32*) 0xbc805000;
    len = 65;
    printf( "\nAddr: %p -----------------------------------", adr );
    while (len--)
    {
        if (count == 0)
            printf("\n0x%p: ", adr);
        if ( ++count == 4 )
            count = 0;
            
        printf("%08x ", *(adr++));
    }
    
    count = 0;
    adr = (uint32*) 0xbc803000;
    len = 64;
    printf( "\nAddr: %p -----------------------------------", adr );
    while (len--)
    {
        if (count == 0)
            printf("\n0x%p: ", adr);
        if ( ++count == 4 )
            count = 0;
            
        printf("%08x ", *(adr++));
    }
    
    count = 0;
    adr = (uint32*) 0xbc050000;
    len = 64;
    printf( "\nAddr: %p -----------------------------------", adr );
    while (len--)
    {
        if (count == 0)
            printf("\n0x%p: ", adr);
        if ( ++count == 4 )
            count = 0;
            
        printf("%08x ", *(adr++));
    }
}

/*
 *  Configuration for L2 Switch Mode
 *
 *  From david's letter.
 *
 *  for_RTL8650A = 1 -- 8650A
 *               = 0 -- 8650B
 */
int32 L2_swCore_config( uint8* gmac, uint32 for_RTL8650A )
{
    uint32 sysCLKRate;
    uint32 SCLK;
    uint32 DIV;
    uint32 SCR;
    uint32 initUART1 = 1;    // Is UART1 existed ?

    printf("\n\n");
    if ( for_RTL8650A )
        printf( "For 8650A\n");
    else
        printf( "For 8650B\n");
        
    // Read High-speed Lexra bus
    uint32 scr = (REG32(SCLKCR) & 0xF0000000) >> 28;
    uint32 mcr = (REG32(SCLKCR) & 0x0F000000) >> 24;

    printf( "System Clock Rate: " );
    switch( scr )
    {
        case 0: printf( "200MHz" ); break;
        case 1: printf( "180MHz" ); break;
        case 2: printf( "170MHz" ); break;
        case 3: printf( "190MHz" ); break;
        case 4: printf( "160MHz" ); break;
        case 5: printf( "150MHz" ); break;
        case 6: printf( "140MHz" ); break;
        case 7: printf( "100MHz" ); break;
        default:printf( "unknown" ); break;
    }

    printf( ", Memory Clock Rate: " );
    switch( mcr )
    {
        case 0: printf( " 50MHz" ); break;
        case 1: printf( "100MHz" ); break;
        case 2: printf( "110MHz" ); break;
        case 3: printf( "120MHz" ); break;
        case 4: printf( "130MHz" ); break;
        case 5: printf( "140MHz" ); break;
        case 6: printf( "150MHz" ); break;
        case 7: printf( "160MHz" ); break;
        default:printf( "unknown" ); break;
    }
    printf( " %08x\n",REG32(SCLKCR));

    // dv "\n [Set](3) : Swith Core MAC, PCI 33 Mhz, all others =0 (0xbc803000):\n"
    // dv "##### to Configure PCI clock (33Mhz) clock divide factor \n "
    // ew 0xBC803004 = 0x0     ;//Write 0 before read to update the current clock value of this register. 
    // dw 0xBC803004
    // ew $sysCLKRate = (@0xBC803004 & 0x00070000) >> 0x10 ; //Read system clock rate and set PCI clock setting factor.
    printf( " [Set](3) : Swith Core MAC, PCI 33 Mhz, all others =0 (0xbc803000):\n" );
    printf( "##### to Configure PCI clock (33Mhz) clock divide factor \n " );
    REG32(MACMR) = REG32(MACMR) & ~SYS_CLK_MASK;
    sysCLKRate = ( REG32(MACMR) & 0x00070000 ) >> 0x10;

    switch( sysCLKRate )
    {
        case 0x0: printf( " --> low system clock = 100MHZ \n" ); SCLK= 0x64; break;
        case 0x1: printf( " --> low system clock =  90MHZ \n" ); SCLK= 0x5A; break;
        case 0x2: printf( " --> low system clock =  85MHZ \n" ); SCLK= 0x55; break;
        case 0x3: printf( " --> low system clock =  96MHZ \n" ); SCLK= 0x60; break;
        case 0x4: printf( " --> low system clock =  80MHZ \n" ); SCLK= 0x50; break;
        case 0x5: printf( " --> low system clock =  75MHZ \n" ); SCLK= 0x4B; break;
        case 0x6: printf( " --> low system clock =  70MHZ \n" ); SCLK= 0x46; break;
        case 0x7: printf( " --> low system clock =  50MHZ \n" ); SCLK= 0x32; break;
        default:  printf( " --> low system clock = unknown\n" ); SCLK= 0xFF; break;
    }

    // if ( @$for_RTL8650A == 0x1) {ew $DIV = (@$SCLK+0x10)/0x21 -1 ; dv "RTL8650A mode\n"} {ew $DIV = 2*@$SCLK/0x21 -1 ; dv "RTL8650B mode\n" }
    if ( for_RTL8650A ) 
    {
        DIV = ( SCLK + 0x10 ) / 0x21 - 1;
        printf( "RTL8650A mode\n" );
    } 
    else
    {
        DIV = 2 * SCLK / 0x21 - 1;
        printf( "RTL8650B mode\n" );
    }

    // ew 0xBC803000 = (@0xBC803000 & 0xffff8fff)|(@$DIV << 0xC) ;
    // dv " Now, sysCLK=@$SCLK Mhz ;DIV = %d \n", @$DIV  
    REG32(MACCR) = ( REG32(MACCR) & 0xffff8fff ) | ( DIV << 0xC );
    printf( " Now, sysCLK=%d Mhz ;DIV = %d \n", SCLK, DIV );

    // dv "\n [Set](1) : LED display Mode (0xbc8020a0):\n" (for 15 LED, single color, Link/SPD/DUP)
    // ew 0xbc8020a0 = 0x000002C7 
    printf( " [Set](1) : LED display Mode (0xbc8020a0):\n" );
    REG32(SWTAA) = PORT5_PHY_CONTROL;
    REG32(TCR0) = 0x000002C7;
    REG32(SWTACR) = CMD_FORCE | ACTION_START; // force add
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */

    /* added for david's reguest */
    REG32(SWTAA) = PORT6_PHY_CONTROL;
    REG32(TCR0) = 0x00000056;
    REG32(SWTACR) = CMD_FORCE | ACTION_START; // force add
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE ); /* Wait for command done */

    // dv "\n [Set](2) : MIB counter (0xbc801070,78,7C):\n"
    // ew 0xbc801070 = 0xff000000 
    // ew 0xbc801078 = 0xff000000 
    // ew 0xbc80107C = 0xff000000 
    // Note : RTL8650A = 0xff000000
    printf( " [Set](2) : MIB counter (0xbc801070,78,7C):\n" );
    REG32(MIB_CONTROL) = 0xff000000;
    if ( for_RTL8650A )
    {
        // SCCR1 and SCCR2 is not available.
    }
    else
    {
        REG32(SCCR1) = 0xff000000;
        REG32(SCCR2) = 0xff000000;
    }

    // dv "\n [Set](4) : init MDC/MDIO (0xbc803018) \n" 
    // ew 0xbc803018 = 0x4ff005f2 
    // RTL8650A = 0x4ff00000 
    printf( " [Set](4) : init MDC/MDIO (0xbc803018) \n" );
    if ( for_RTL8650A )
        REG32(MISCCR) = 0x4ff00000;
    else
        REG32(MISCCR) = 0x4ff005f2;

    // dv "\n [Set](5) : Init SwitchCore MISC, enable L3/L4 Re-calculation, L2 CRC Err allow (0xbc805010)\n"
    // ew 0xbc805010 =  0x00000000 
    // RTL8650A = 0x00000000
    printf( " [Set](5) : Init SwitchCore MISC, enable L3/L4 Re-calculation, L2 CRC Err allow (0xbc805010)\n" );
    if ( for_RTL8650A )
        REG32(CSCR) = 0x00000000;
    else
        REG32(CSCR) = 0x00000000;

    // dv "\n [Set] (6): # Disable flow control, descriptor run out threshold = 496 (0xbc805014)\n"
    // ew 0xbc805014 =  0xfffc05e0 
    // RTL8650A = 0xfffc05e0
    printf( " [Set] (6): # Disable flow control, descriptor run out threshold = 496 (0xbc805014)\n" );
    if ( for_RTL8650A )
        REG32(FCREN) = 0xfffc05e0;
    else
        REG32(FCREN) = 0xfffc05e0;

    // dv "\n [Set] (7): # set flow control per-port reserved threshold FC_On =32, FC_Off =16(0xbc805018)\n"
    // ew 0xbc805018 =  0x20102010 
    printf( " [Set] (7): # set flow control per-port reserved threshold FC_On =32, FC_Off =16(0xbc805018)\n" );
    REG32(FCRTH) = 0x10201020;

    // dv "\n [Set] (8): # set flow control shared threshold = 54 (0xbc805028)\n"
    // ew 0xbc805028 =  0x00360036 
    // RTL8650A = 0x00400040 
    printf( " [Set] (8): # set flow control shared threshold = 54 (0xbc805028)\n" );
    if ( for_RTL8650A )
        REG32(FCPTR) = 0x00400040;
    else
        REG32(FCPTR) = 0x00360036;

    // dv "\n [Set] (9): # enable TTL-1 (0xbc80502c) \n"
    // ew 0xbc80502c =  0x80000000
    printf( " [Set] (9): # enable TTL-1 (0xbc80502c) \n" );
    REG32(TTLCR) = 0x80000000;

    // dv "\n [Set] (10):# enable L2, disable Egress/Ingress ACL ;  disable L3/L4, Spanning tree (0xbc805030) \n"
    // ew 0xbc805030 =  0x00000001
    // RTL8650A =0x0000009F 
    printf( " [Set] (10):# enable L2, disable Egress/Ingress ACL ;  disable L3/L4, Spanning tree (0xbc805030) \n" );
    if ( for_RTL8650A )
        REG32(MSCR) = 0x0000009F;
    else
        REG32(MSCR) = 0x00000001;

    // dv "\n [Set] (10.2):# disable broadcast storm control (0xbc805038) \n"
    // ew 0xbc805038 =  0x00000000
    printf( " [Set] (10.2):# disable broadcast storm control (0xbc805038) \n" );
    REG32(BSCR) = 0x00000000;

    // dv "\n [Set] (11):# enable L2/L4 Aging (0xbc80503c) \n"
    // ew 0xbc80503c =  0xffffffff 
    printf( " [Set] (11):# enable L2/L4 Aging (0xbc80503c) \n" );
    REG32(TEATCR) = 0xfffffffc;
    
    // dv "\n [Set] (12): # enable L4 offset control \n"
    // ew 0xbc805080 =  0x03f00000 
    //printf( "\n [Set] (12): # enable L4 offset control \n" );
    //REG32(OCR) = 0x03f00000;

    // dv "\n [Set] (13): # Init SDRAM timing \n" 
    // ew 0xbd013008 =  0x00000463 
    //printf( " [Set] (13): # Init SDRAM timing \n" );
    //REG32(MTCR1) = 0x00000463;

    // dv "\n [Set] (14): # Init lexra bus timeout \n"
    // ew 0xbd012064 =  0xf0000000 
    printf( " [Set] (14): # Init lexra bus timeout \n" );
    REG32(LTOC) = 0xf0000000;

    // dv "\n [Set] (14): # Init peripheral lexra timing (0xbd012060) \n"
    // ew 0xbd012060 =  0x00000000 
    printf( " [Set] (14): # Init peripheral lexra timing (0xbd012060) \n" );
    REG32(PLTCR) = 0x00000000;

    // dv "\n [Set] (15): # Init TRXRDY \n"
    // ew 0xbc805004 =  0x318f0002 
    printf( " [Set] (15): # Init TRXRDY \n" ); /* BIST ??? */
    REG32(BISTCR) = 0x318f0002;

    // dv "\n [Set] (16):# set Port_based VLAN ID , all PVID=0 (0xbc805078) \n"
    // ew 0xbc805078 =  0x00000000 
    printf( " [Set] (16):# set Port_based VLAN ID , all PVID=0 (0xbc805078) \n" );
    REG32(PVCR) = 0x00000000;

    //dv "\n [set] (17): # Set UART 38400 bps, N,8,1  \n"
    // 1. Cehck system clock value (that is low speed Lexra BUS clock) 
    printf( " [set] (17): # Set UART 38400 bps, N,8,1  \n" );
    switch( ( REG32(MACMR) & 0x00070000 ) >> 0x10 )
    {
        case 0: SCR = 0x5F5E100; printf( " --> system clock = 100MHZ \n" ); break;
        case 1: SCR = 0x55D4A80; printf( " --> system clock =  90MHZ \n" ); break;
        case 2: SCR = 0x510FF40; printf( " --> system clock =  85MHZ \n" ); break;
        case 3: SCR = 0x5B8D800; printf( " --> system clock =  96MHZ \n" ); break;
        case 4: SCR = 0x4C4B400; printf( " --> system clock =  80MHZ \n" ); break;
        case 5: SCR = 0x47868C0; printf( " --> system clock =  75MHZ \n" ); break;
        case 6: SCR = 0x42C1D80; printf( " --> system clock =  70MHZ \n" ); break;
        case 7: SCR = 0x2FAF080; printf( " --> system clock =  50MHZ \n" ); break;
        default:SCR = 0xFFFFFFF; printf( " --> system clock = unknown\n" ); break;
    }
    
    // 2. set Line Control Register : 
    // eb 0xbd01110c = 0x03 ; dv " --> Line Control Parameter = [N,8,1]\n" ; 
    // eb 0xbd01100c = 0x03 ; dv " --> Line Control Parameter = [N,8,1]\n" ; 
    // ew $BR =0x9600  ; // --> Baud Rate = 38400 bps\n  
    // ew $divisor = (@$SCR/(@$BR*0x10))-1 ;// calculate the Divisor Latch value 
    REG8(UART0_BASE+0x0c) = 0x03;
    //printf( " --> Line Control Parameter = [N,8,1]\n" );
    if ( initUART1 )
    {
        REG8(UART1_BASE+0x0c) = 0x03;
        //printf( " --> Line Control Parameter = [N,8,1]\n" );
    }
    uint32 BR = 0x9600; /* 38400 bps */
    uint32 divisor = ( SCR / (BR*0x10) ) - 1;

    // to configure DLL and DLM   
    // ew 0xbd01110c = @0xbd01110c | 0x80000000 ;;  // set DLAB bit =1 
    // ew 0xbd01100c = @0xbd01100c | 0x80000000 ;;  // set DLAB bit =1 
    // eb 0xbd011100 = @$divisor ; 
    // eb 0xbd011000 = @$divisor ; 
    REG32(UART0_BASE+0x0c) |= 0x80000000; // set DLAB bit = 1
    REG8(UART0_BASE+0x00) = divisor;
    if ( initUART1 )
    {
        REG32(UART1_BASE+0x0c) |= 0x80000000; // set DLAB bit = 1
        REG8(UART1_BASE+0x00) = divisor;
    }

    // eb 0xbd011104 = @$divisor >> 8;  
    // eb 0xbd011004 = @$divisor >> 8;  
    // ew 0xbd01110c = @0xbd01110c & 0x7fffffff ;;  // set DLAB =0
    // ew 0xbd01100c = @0xbd01100c & 0x7fffffff ;;  // set DLAB =0
    REG8(UART0_BASE+0x04) = divisor >> 8;
    REG32(UART0_BASE+0x0c) &= 0x7FFFFFFF; // set DLAB bit = 1
    if ( initUART1 )
    {
        REG8(UART1_BASE+0x04) = divisor >> 8;
        REG32(UART1_BASE+0x0c) |= 0x7FFFFFFF; // set DLAB bit = 1
    }

    // ----------------------------------
    // VLAN table Setup :
    // ----------------------------------
    // dv "\n // Write to VLAN table entry 0 : (VLAN 0: without tagging) \n"
    // ew (0xbc800000 + 0x8)   = 0xbc050000 + 0*0x20 ;;   //Note : each entry = 8 Word = 32 byte.
    // ew (0xbc800000 + 0x20)  = 0x11223344 ;; // W0  ,
    // ew (0xbc800000 + 0x24)  = 0x003F0000 ;; // W1  ,MBR= W1:22:17, VID=W1:31:23
    // ew (0xbc800000 + 0x28)  = 0x00000000 ;; // W2
    // ew (0xbc800000 + 0x2C)  = 0xffff3ffd ;; // W3  ,VLANUntag= W3:21:16
    // ew (0xbc800000 + 0x30)  = 0x00000005 ;; // W4
    // ew (0xbc800000 + 0x34)  = 0x00000000 ;; // W5
    // ew (0xbc800000 + 0x38)  = 0x00000000 ;; // W6
    // ew (0xbc800000 + 0x3C)  = 0x00000000 ;; // W7
    // ew (0xbc800000 + 0x0)   = 0x9 ;;    
    printf( " // Write to VLAN table entry 0 : (VLAN 0: without tagging) \n" );
    REG32(SWTAA) = 0xbc050000 + 0*0x20 ;;   //Note : each entry = 8 Word = 32 byte.
    REG32(TCR0)  = 0x11223344 ;; // W0  ,
    REG32(TCR1)  = 0x003F0000 ;; // W1  ,MBR= W1:22:17, VID=W1:31:23
    REG32(TCR2)  = 0x00000000 ;; // W2
    REG32(TCR3)  = 0xffff3ffd ;; // W3  ,VLANUntag= W3:21:16
    REG32(TCR4)  = 0x00000005 ;; // W4
    REG32(TCR5)  = 0x00000000 ;; // W5
    REG32(TCR6)  = 0x00000000 ;; // W6
    REG32(TCR7)  = 0x00000000 ;; // W7
    REG32(SWTACR)= 0x9 ;;        // 
    
    // dv "\n // Write to VLAN table entry 1 : (VLAN 1: with tagging) \n"
    // ew (0xbc800000 + 0x8)   = 0xbc050000 + 1*0x20 ;;   //Note : each entry = 8 Word = 32 byte.
    // ew (0xbc800000 + 0x20)  = 0x11223344 ;; // W0
    // ew (0xbc800000 + 0x24)  = 0x007f0000 ;; // W1  ,MBR= W1:22:17, VID=W1:31:23
    // ew (0xbc800000 + 0x28)  = 0x00000000 ;; // W2
    // ew (0xbc800000 + 0x2C)  = 0xffc03ffd ;; // W3  ,VLANUntag= W3:21:16
    // ew (0xbc800000 + 0x30)  = 0x00000005 ;; // W4
    // ew (0xbc800000 + 0x34)  = 0x00000000 ;; // W5
    // ew (0xbc800000 + 0x38)  = 0x00000000 ;; // W6
    // ew (0xbc800000 + 0x3C)  = 0x00000000 ;; // W7
    // ew (0xbc800000 + 0x0)   = 0x9 ;;        // 
    printf( " // Write to VLAN table entry 1 : (VLAN 1: with tagging) \n" );
    REG32(SWTAA) = 0xbc050000 + 1*0x20 ;;   //Note : each entry = 8 Word = 32 byte.
    REG32(TCR0)  = 0x11223344 ;; // W0  ,
    REG32(TCR1)  = 0x007f0000 ;; // W1  ,MBR= W1:22:17, VID=W1:31:23
    REG32(TCR2)  = 0x00000000 ;; // W2
    REG32(TCR3)  = 0xffc03ffd ;; // W3  ,VLANUntag= W3:21:16
    REG32(TCR4)  = 0x00000005 ;; // W4
    REG32(TCR5)  = 0x00000000 ;; // W5
    REG32(TCR6)  = 0x00000000 ;; // W6
    REG32(TCR7)  = 0x00000000 ;; // W7
    REG32(SWTACR)= 0x9 ;;        // 

    // DumpAsicRegisters();

/*****************************************************************/


    return 0;
}
#endif //_L2_MODE_


#ifdef _HUB_MODE_

uint32 getLinkStatus( void )
{
    uint32 link = 0;
    uint32 i, dummy;

    for( i = 0; i < 5; i++ )
    {
        dummy = REG32(PHY_BASE+(i<<5) + 0x4); /* To get the correct link status, this register must be read twice. */
        if(REG32(PHY_BASE+(i<<5) + 0x4) & 0x4)
        {
            //link is up
            link |= (1<<i);
        }
    }

    return link;
}

/*
 *  HubMode() --
 *
 *  Ocuppy all L2 table entries to force broadcast.
 *
 */
void HubMode(void)
{

}
#endif //_HUB_MODE_


