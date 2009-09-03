#define __KERNEL_SYSCALLS__

#include <linux/config.h>
#include <linux/proc_fs.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/utsname.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <linux/blk.h>
#include <linux/hdreg.h>
#include <linux/iobuf.h>
#include <linux/bootmem.h>
#include <linux/tty.h>

#ifdef CONFIG_RTL8186_AP
#include <asm/rtl8186.h>
#endif

#include <asm/io.h>
#include <asm/bugs.h>

#if defined(CONFIG_ARCH_S390)
#include <asm/s390mach.h>
#include <asm/ccwcache.h>
#endif

#ifdef CONFIG_PCI
#include <linux/pci.h>
#endif

#ifdef CONFIG_DIO
#include <linux/dio.h>
#endif

#ifdef CONFIG_ZORRO
#include <linux/zorro.h>
#endif

#ifdef CONFIG_MTRR
#  include <asm/mtrr.h>
#endif

#ifdef CONFIG_NUBUS
#include <linux/nubus.h>
#endif

#ifdef CONFIG_ISAPNP
#include <linux/isapnp.h>
#endif

#ifdef CONFIG_X86_LOCAL_APIC
#include <asm/smp.h>
#endif

/*
 * Versions of gcc older than that listed below may actually compile
 * and link okay, but the end product can have subtle run time bugs.
 * To avoid associated bogus bug reports, we flatly refuse to compile
 * with a gcc that is known to be too old from the very beginning.
 */
#if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 91)
#error Sorry, your GCC is too old. It builds incorrect kernels.
#endif

#ifdef CONFIG_DS1820_PIN_6
  #define DS1820_BIT 0x40 
#endif
#ifdef CONFIG_DS1820_PIN_B
  #define DS1820_BIT 0x02 
#endif
#ifdef CONFIG_DS1820_PIN_7
  #define DS1820_BIT 0x80 
#endif
#ifdef CONFIG_DS1820_PIN_8
  #define DS1820_BIT 0x100 
#endif
#ifdef CONFIG_DS1820_PIN_9
  #define DS1820_BIT 0x200
#endif
#ifdef CONFIG_DS1820_PIN_10
  #define DS1820_BIT 0x400 
#endif

#define DS1820Low \
   {\
    RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) | DS1820_BIT));\
    RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) & ~(DS1820_BIT)));\
   }       
#define DS1820High \
   {\
    RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) | DS1820_BIT));\
    RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) | DS1820_BIT));\
   }       
#define DS1820Z \
   {\
    RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) & ~(DS1820_BIT)));\
   }       
#define DS1820State (RTL_R32(RTL_GPIO_PABDATA) & DS1820_BIT)   


//------------------------------------------------------------------------- 
// 64-bit lasered ROM code type definition
struct touchSerial {
   unsigned char bytes[8];
};

#define TOUCH_SERIAL_BIT_SET(touch,bit)     touch.bytes[bit/8] |=  (1 << (bit%8))
#define TOUCH_SERIAL_BIT_CLEAR(touch,bit)   touch.bytes[bit/8] &= ~(1 << (bit%8))
#define TOUCH_SERIAL_BIT(touch,bit)        (touch.bytes[bit/8] & 1 << (bit%8))

// List of registared devices
#define MAX_DS_DEVICES 16
static struct touchSerial touchList[MAX_DS_DEVICES];
static int touchListSize=0;

//------------------------------------------------------------------------- 

//------------------------------------------------------------------------- 
// Send resen and wait for present pulse
// Return 1 if present, 0 - if not
//------------------------------------------------------------------------- 
int TouchReset(void)
{
 DS1820Low;     // Send reset pulse
 udelay(500);   // Wait reset pulse minimum 480 microsec
 DS1820Z;       // Set uptput for input
 udelay(90);    // Wait DS1820 responce
 if(DS1820State) return 0;
 udelay(500);
 return 1;
}
//------------------------------------------------------------------------- 
// Set "1" timeslot
//------------------------------------------------------------------------- 
inline void TouchWrite1(void)
{
 DS1820Low;     // Init timeslot
 udelay(2);     // wait 2 usec (on datasheet - > 1usec)
 DS1820High;    // Set "1" on post
 udelay(60);    // wait 80 usec (on datasheet - > 60, < 120 usec)
 DS1820Z;
 udelay(15);
}
//------------------------------------------------------------------------- 
// Set "0" timeslot
//------------------------------------------------------------------------- 
inline void TouchWrite0(void)
{
 DS1820Low;     // Init timeslot
 udelay(62);    // wait 80 usec (on datasheet - > 60, < 120 usec)
 DS1820Z;
 udelay(15);
}
//------------------------------------------------------------------------- 
// Read bit 
//------------------------------------------------------------------------- 
inline int TouchReadBit(void)
{
 int retval;
 DS1820Low;     // Init timeslot
 udelay(2);     // Wait 2 usec
 DS1820Z;       // Prepare for bit reading
 udelay(20);     // for datasheet - DS1820 set bit on wire after 15 usec for a 45 usec
 retval = DS1820State;
 udelay(100);    // wait for timeslot finish. 
 return retval;
}
//------------------------------------------------------------------------- 
// Write byte to device
//------------------------------------------------------------------------- 
void TouchWriteByte(unsigned char byte)
{
 unsigned char mask=1;
 while(mask)
   {
    if(byte & mask) TouchWrite1();
    else            TouchWrite0();
    mask <<= 1;
   }
}
//------------------------------------------------------------------------- 
// Read byte from device
//------------------------------------------------------------------------- 
unsigned char TouchReadByte(void)
{
 unsigned char mask=1;
 unsigned char readed_byte=0;
 while(mask)
   {
    if(TouchReadBit()) readed_byte |= mask;
    mask <<= 1;
   }
 return readed_byte;  
}
//------------------------------------------------------------------------- 
// Strong pullup
//------------------------------------------------------------------------- 
void TouchPullUp(int state)
{
 if(state) DS1820High
 else      DS1820Z
}
//------------------------------------------------------------------------- 
// Send Search ROM command and fillup list of ibutton devices
//------------------------------------------------------------------------- 
int TouchSearchRom(void)
{
 struct touchSerial foundedDevice; // here stored new founded device;
 struct touchSerial maskDevices;   // in this variables ve set "two-state" bits
 int i,readed;
 
 // At first - unregister all devices
 touchListSize=0;
 // At the begining we don't know, what bits will bi in "two-state" 
 memset(&maskDevices,0,sizeof(maskDevices));

looper: 
  // Clear all bits for new device
  memset(&foundedDevice,0,sizeof(foundedDevice));
  // Start Search ROM loop;
  TouchReset();
  TouchWriteByte(0xF0);
  // Loop for 64 bit lasered rom code
  for(i=0;i<64;i++)
    {
     readed  = (TouchReadBit() ? 2 : 0);
     readed |= (TouchReadBit() ? 1 : 0);
//     printk("readed byte %d",readed);
     switch(readed)
       {
        case 2: // 10 - devices send "1" 
//	  printk(" - in case 2\n");
	  TOUCH_SERIAL_BIT_SET(foundedDevice,i);
	  TouchWrite1();
	  break;
	case 1: // 01 - devices send "0"
//	  printk(" - in case 1\n");
	  TOUCH_SERIAL_BIT_CLEAR(foundedDevice,i);
	  TouchWrite0();
	  break;
	case 0: // 00 - devices send "0" and "1"
//	  printk(" - in case 0\n");
	  // at first check - this bit in mask is "0" - set it
	  // to "1" and set founded bit to "0"
	  // At second check - this bit in mask is "1" - set it 
	  // to "0" and set founded bit to "1"
	  if(TOUCH_SERIAL_BIT(maskDevices,i))
	    {
	     TouchWrite1();
	     TOUCH_SERIAL_BIT_SET  (foundedDevice,i);
	     TOUCH_SERIAL_BIT_CLEAR(maskDevices,i);
	    }
	  else
	    {
	     TouchWrite0();
	     TOUCH_SERIAL_BIT_CLEAR(foundedDevice,i);
	     TOUCH_SERIAL_BIT_SET  (maskDevices,i);
	    }    
	  break;
	case 3: // 11 - bus error
//	  printk(" - in case 3\n");
	  return -1;
       }
    }
  // foundedDevice consist device lasered code. Save it.
  memcpy(touchList+touchListSize,&foundedDevice,sizeof(foundedDevice));
  touchListSize++;
  
  // Check. If any bits in mask sets in '1' - repeat loop for it
  if(touchListSize < MAX_DS_DEVICES)
    for(i=0;i<8;i++) if(maskDevices.bytes[i]) goto looper;
 return 0; 
}
//------------------------------------------------------------------------- 
int TouchDevicesCount(void)
{
 return touchListSize;
}
//------------------------------------------------------------------------- 
unsigned char * TouchDevice(int devno)
{
 return touchList[devno].bytes;
}
//------------------------------------------------------------------------- 
void TouchMatchRom(unsigned char * Lasered)
{
 int i;
 TouchWriteByte(0x55);
 for(i=0;i<8;i++) TouchWriteByte(Lasered[i]);
}
//------------------------------------------------------------------------- 
//------------------------------------------------------------------------- 
//------------------------------------------------------------------------- 

