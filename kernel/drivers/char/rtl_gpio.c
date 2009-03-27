/*
 * FILE NAME rtl_gpio.c
 *
 * BRIEF MODULE DESCRIPTION
 *  GPIO For Flash Reload Default 
 *
 *  Author: Realtek , Inc.  
 *          sc_yang@realtek.com.tw
 *
 * Copyright 2001 MontaVista Software Inc.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE	LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/reboot.h>

#ifdef CONFIG_RTL8186_AP
	#define RTL_GPIO_PABDIR 	0xbd010124
	#define RTL_GPIO_PABDATA 	0xbd010120
	#define RESET_PIN	0
#else
	#define RTL_GPIO_PABDIR 	0xbd010040
	#define RTL_GPIO_PABDATA 	0xbd010044
	#define RTL_WLAN_CR9346		0xbd400050
	#define RTL_WLAN_CONFIG0	0xbd400051
	#define RTL_WLAN_PSR		0xbd40005e
	#define RESET_PIN	10
#endif

#define PROBE_NULL	0
#define PROBE_ACTIVE	1
#define PROBE_RESET	2
#define PROBE_RELOAD	3
#define RTL_R32(addr)   (*(volatile unsigned long *)(addr))
#define RTL_W32(addr, l)   ((*(volatile unsigned long*)(addr)) = (l))
#define RTL_R8(addr)   (*(volatile unsigned char*)(addr))
#define RTL_W8(addr, l)   ((*(volatile unsigned char*)(addr)) = (l))

#ifdef GPIO_DEBUG
/* note: prints function name for you */
#  define DPRINTK(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

static struct timer_list probe_timer;
#ifdef CONFIG_PROC_LEDS
static struct timer_list led_timer0;
static struct timer_list led_timer1;		
#endif
static unsigned int    probe_counter;
static unsigned int    probe_state;

#ifdef CONFIG_PROC_LEDS
int led_flash0[16]={20,1,20,0,20,1,20,0,20,1,20,0,20,1,20,0};//dgreen
int led_flash1[16]={20,0,20,1,20,0,20,1,20,0,20,1,20,0,20,1};//dgreen
int led0_counter=0;
int led1_counter=0;
#endif

#ifdef CONFIG_PROC_COOLER
static struct timer_list cooler_timer;
int cooler_speed=4; // 0 - off, 1 - 25%, 2 - 50%, 3 - 75%, 4 - 100%
int cooler_state=0;
#endif

#ifndef CONFIG_RTL8186_AP
unsigned int led0enable;
#endif

static int reset_flash_default(void *data)
{
	int res = 0;
	char *argv[4], *envp[1] = {NULL};
	int i = 0;
	int reset_default=(int) data;
	argv[i++] = "/bin/fs";
	argv[i++] = "flashrestore";
	argv[i] = NULL;
	if(reset_default){
	    res = call_usermodehelper(argv[0], argv, envp,1);
		if (res)
		    printk("Failed to Reset Default:%i",res);
		else
		    machine_restart(NULL);
	}
	return 0;
}

#ifdef CONFIG_PROC_LEDS
static void rtl_gpio_flash0(unsigned long data)
{

 if(led_flash0[led0_counter+1])
   RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) & ~0x4));
 else
   RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) | 0x4));
 mod_timer(&led_timer0, jiffies + led_flash0[led0_counter]);
 led0_counter+=2;
 if(led0_counter >= 16) led0_counter=0; 
}

static void rtl_gpio_flash1(unsigned long data)
{
 if(led_flash1[led1_counter+1])
   RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) & ~0x8));
 else
   RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) | 0x8));
 mod_timer(&led_timer1, jiffies + led_flash1[led1_counter]);
 led1_counter+=2;
 if(led1_counter >= 16) led1_counter=0;
}
#endif

#ifdef CONFIG_PROC_COOLER
static void rtl_gpio_cooler(unsigned long data)
{
 int on_off=0;
 cooler_state++;
 if(cooler_state >=4) cooler_state=0;
 // Check new cooler state (on|off)
 switch(cooler_speed)
   {
    case 0: on_off = 0;                   break;  // 0%
    case 1: on_off = cooler_state==0?1:0; break;  // 25%
    case 2: on_off = cooler_state%2;      break;  // 50%
    case 3: on_off = cooler_state==0?0:1; break;  // 75%
    case 4: on_off = 1;                   break;  // 100%
    default: on_off = 0;
   }

#ifdef CONFIG_COOLER_PIN_6
  #define COOLER_BIT 0x40 
#endif
#ifdef CONFIG_COOLER_PIN_7
  #define COOLER_BIT 0x80 
#endif
#ifdef CONFIG_COOLER_PIN_8
  #define COOLER_BIT 0x100 
#endif
#ifdef CONFIG_COOLER_PIN_9
  #define COOLER_BIT 0x200
#endif
#ifdef CONFIG_COOLER_PIN_10
  #define COOLER_BIT 0x400 
#endif

 RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) | COOLER_BIT));
 if(on_off)
   RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) & ~(COOLER_BIT)));
 else
   RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) | (COOLER_BIT)));
 mod_timer(&cooler_timer, jiffies + 2);
}
#endif


static void rtl_gpio_timer(unsigned long data)
{
	unsigned int pressed=1;

	// Set GPIOA pin 10(8181),0 (8186)as input pin
	RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) & (~(1 << RESET_PIN))));
#ifdef CONFIG_RTL8186_AP
	// Set GPIOA  ping 2 as output pin for led
	RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) | 0x8));
	RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) | 0x4));	//Erwin
#endif


	if ((RTL_R32(RTL_GPIO_PABDATA) & (1 << RESET_PIN))){
		pressed = 0;
	}
	else
	{
		//printk("Key pressed %d!\n", probe_counter+1);
	}

	if (probe_state == PROBE_NULL)
	{
		if (pressed)
		{
			probe_state = PROBE_ACTIVE;
			probe_counter++;
		}
		else
			probe_counter = 0;
	}
	else if (probe_state == PROBE_ACTIVE)
	{
		if (pressed)
		{
			probe_counter++;
			if ((probe_counter >=2 ) && (probe_counter <=5))
			{
#ifndef CONFIG_RTL8186_AP
				led0enable=1;
				// turn on LED0
				RTL_W8(RTL_WLAN_CR9346, 0xc0);
				RTL_W8(RTL_WLAN_CONFIG0, 0x10);
				RTL_W8(RTL_WLAN_PSR,(RTL_R8(RTL_WLAN_PSR)&0xEF));
#endif
			  		
			}
			else if (probe_counter >= 5)
			{
#ifndef CONFIG_RTL8186_AP
				led0enable=1;
				RTL_W8(RTL_WLAN_CR9346, 0xc0);
				RTL_W8(RTL_WLAN_CONFIG0, 0x10);
				// sparkling LED0
				if (probe_counter & 1)
					RTL_W8(RTL_WLAN_PSR, (RTL_R8(RTL_WLAN_PSR)|0x10));
				else
					RTL_W8(RTL_WLAN_PSR, (RTL_R8(RTL_WLAN_PSR)&0xEF));
#endif
			}
		}
		else
		{
			if (probe_counter < 2)
			{
				probe_state = PROBE_NULL;
				probe_counter = 0;
				//printk("<2 \n");
			}
			else if (probe_counter >=5)
			{
#ifndef CONFIG_RTL8186_AP
				led0enable=1;
#endif
				//printk("Going to Reload Default\n");
				kernel_thread(reset_flash_default, 1, SIGCHLD);
				return;

			}
			else
			{
#ifndef CONFIG_RTL8186_AP
				RTL_W8(RTL_WLAN_PSR , (RTL_R8(RTL_WLAN_PSR)|0x10) );
				RTL_W8(RTL_WLAN_CONFIG0, (RTL_R8(RTL_WLAN_CONFIG0)&(~0x10)) );
#endif
				//printk("2-5sec reset\n");
				kernel_thread(reset_flash_default, 0, SIGCHLD);
				return;
			}
		}

	}
	mod_timer(&probe_timer, jiffies + 100);
}

int __init rtl_gpio_init(void)
{
	//printk("Realtek GPIO Driver for Flash Reload Default\n");
	init_timer (&probe_timer);
	probe_counter = 0;
	probe_state = NULL ;
	probe_timer.expires=jiffies + 100;
        probe_timer.data =(unsigned long) NULL ;
        probe_timer.function = &rtl_gpio_timer;
        mod_timer(&probe_timer, jiffies + 100);	


#ifdef CONFIG_PROC_LEDS
	init_timer (&led_timer0);
	led_timer0.expires=jiffies+10;
        led_timer0.data =(unsigned long) NULL ;
        led_timer0.function = &rtl_gpio_flash0;
        mod_timer(&led_timer0, jiffies+10);	

	init_timer (&led_timer1);
	led_timer1.expires=jiffies+10;
        led_timer1.data =(unsigned long) NULL ;
        led_timer1.function = &rtl_gpio_flash1;
        mod_timer(&led_timer1, jiffies+10);	
#endif

#ifdef CONFIG_PROC_COOLER
	init_timer (&cooler_timer);
	cooler_timer.expires=jiffies+10;
        cooler_timer.data =(unsigned long) NULL ;
        cooler_timer.function = &rtl_gpio_cooler;
        mod_timer(&cooler_timer, jiffies+10);	
#endif
		
	return 0;
}

static void __exit rtl_gpio_exit(void)
{
	//printk("Unload Realtek GPIO Driver \n");
	del_timer_sync(&probe_timer);
#ifdef CONFIG_PROC_LEDS
	del_timer_sync(&led_timer0);
	del_timer_sync(&led_timer1);
#endif	
#ifdef CONFIG_PROC_COOLER
	del_timer_sync(&cooler_timer);
#endif
}

module_exit(rtl_gpio_exit);
module_init(rtl_gpio_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver for Reload default");
