/*
 * Copyright (c) Cl�ent Ballabriga, 2005 - GPL
 * Copyright (c) Guylhem Aznar, 2005 - GPL
 * Copyright (c) Dr. Morg, 2006 - GPL
 *
 * Based on Madsuk/Rohde work on a MMC driver for the WRT54G.
 *
 * This is an ugly hack of a driver. I am surprised if it ever works!
 * So please use a real driver or contribute one to the 2.4/2.6 mmc framework
 */

#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/blkpg.h>
#include <linux/hdreg.h>
#include <linux/major.h>

#ifdef CONFIG_RTL8186_AP
#include <asm/rtl8186.h>
#endif
#ifdef CONFIG_RTL8196_AP
#include <asm/rtl8196.h>
#endif

#include <asm/uaccess.h>
#include <asm/io.h>

/*
 * *******************************************************************
 *
 *                This is the only configurable part.
 *
 * *******************************************************************
 *
 */

#define DEBUG 0
#define DEVICE_NAME "mmc"
#define DEVICE_NR(device) (MINOR(device))
#define DEVICE_ON(device)
#define DEVICE_OFF(device)
#define MAJOR_NR 121

/* Let that include where it is or compilation fails on INIT_REQUEST/CURRENT */

#include <linux/blk.h>

MODULE_AUTHOR("Dr. Morg");
MODULE_DESCRIPTION("Driver for MMC/SD-Cards in SPI mode by GPIO");
MODULE_SUPPORTED_DEVICE("RTL8186");
MODULE_LICENSE("GPL");

#define GPIO_SD_DO  6
#define GPIO_SD_CS  7
#define GPIO_SD_CLK 8
#define GPIO_SD_DI  9

/*
 * *******************************************************************
 *
 *               Do not change anything below !
 *
 * *******************************************************************
 *
 */


/* GPIO states */
#define LOW 0
#define HIGH 1

#define INPUT 0
#define OUTPUT 1

#define PRESENT 1
#define ABSENT 0

typedef unsigned int uint32;
typedef unsigned long u32_t;
typedef unsigned short u16_t;
typedef unsigned char u8_t;

/* we have only one device */
static int hd_sizes[1 << 6];
static int hd_blocksizes[1 << 6];
static int hd_hardsectsizes[1 << 6];
static int hd_maxsect[1 << 6];
static struct hd_struct hd[1 << 6];

static struct timer_list mmc_timer;

/* start with no card */
static int mmc_media_detect = 0;
static int mmc_media_changed = 1;

extern struct gendisk hd_gendisk;

/*
 * *******************************************************************
 *
 *             Begin GPIO hardware access functions.
 *
 * *******************************************************************
 *
 */

void gpio_setdir(int num, int dir)
{
    if (dir == 1) {
	  RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) | (1 << num)));
    } else {
	  RTL_W32(RTL_GPIO_PABDIR, (RTL_R32(RTL_GPIO_PABDIR) & (~(1 << num))));
    }
}


int gpio_getdir(int num)
{
    return ((RTL_R32(RTL_GPIO_PABDIR) & (1 << num)) ? 1 : 0);
}

static int gpio_read(int num)
{
	int what;
    
	what=((RTL_R32(RTL_GPIO_PABDATA) & (1 << num)) ? 1 : 0);
	
    return (what);
}

static int gpio_write(int num, int val)
{
	int check;

    if (val == 1) {
	  RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) | (1 << num)));
    } else {
	  RTL_W32(RTL_GPIO_PABDATA, (RTL_R32(RTL_GPIO_PABDATA) & (~(1 << num))));
    }
#ifdef DEBUG
    check=gpio_read(num);
    if (check != val)
    {
	  printk ("Error while write to %d: found %d after writing %d\n",num, check, val);
	return (1);
    }
    else return(0);
#endif

}

/*
 * *******************************************************************
 *
 *             Begin SPI hardware access functions.
 *
 * *******************************************************************
 *
 */
static int mmc_spi_media_detect(void)
{
// FIXME: add card detection/test by SPI

    return 1;
}

static int mmc_spi_hardware_init(void)
{
    //unsigned char gpio_outen;

    printk("mmc: GPIO init\n");

    /* Remap directions */
    gpio_setdir(GPIO_SD_CLK, OUTPUT);
    gpio_setdir(GPIO_SD_DI, OUTPUT);
    gpio_setdir(GPIO_SD_DO, INPUT);
    gpio_setdir(GPIO_SD_CS, OUTPUT);

    printk("mmc: initialising MMC\n");

    /* Start */
    gpio_write(GPIO_SD_CLK, LOW);
    gpio_write(GPIO_SD_DI, LOW);
    gpio_write(GPIO_SD_CS, LOW);
    return 0;
}

/* return what has been read, write the parameter */

static unsigned char mmc_spi_readwrite(unsigned char data_out)
{
    int i;
    unsigned char result = 0;

    for (i = 0; i < 8; i++) {
	if (data_out & (0x01 << (7 - i)))
	    gpio_write(GPIO_SD_DI, HIGH);
	else
	    gpio_write(GPIO_SD_DI, LOW);

	gpio_write(GPIO_SD_CLK, HIGH);

	result <<= 1;

	if (gpio_read(GPIO_SD_DO) == 1)
	    result |= 1;

	gpio_write(GPIO_SD_CLK, LOW);
    }

    return (result);
}

static int mmc_spi_card_init(void)
{
    unsigned char result = 0;
    short i, j;
    unsigned long flags;

    save_flags(flags);
    cli();

    printk("GPIO_SD_CS dir: %u\n", gpio_getdir(GPIO_SD_CS));
    printk("GPIO_SD_DI dir: %u\n", gpio_getdir(GPIO_SD_DI));
    printk("GPIO_SD_DO dir: %u\n", gpio_getdir(GPIO_SD_DO));
    printk("GPIO_SD_CLK dir: %u\n", gpio_getdir(GPIO_SD_CLK));
    
    printk("mmc: card init 1/2\n");
    gpio_write(GPIO_SD_CS, HIGH);	

	// write check
	if(gpio_read(GPIO_SD_CS) != HIGH)
		printk("mmc: write HIGH to GPIO_SD_CS faild! (mmc_spi_card_init)\n");

    for (i = 0; i < 20; i++)
	mmc_spi_readwrite(0xff);

    gpio_write(GPIO_SD_CS, LOW);

	// write check
	if(gpio_read(GPIO_SD_CS) != LOW)
		printk("mmc: write LOW to GPIO_SD_CS faild! (mmc_spi_card_init)\n");

    mmc_spi_readwrite(0x40);
    for (i = 0; i < 4; i++)
	mmc_spi_readwrite(0x00);
    mmc_spi_readwrite(0x95);
    for (i = 0; i < 8; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0x01)
	    break;
    }
    gpio_write(GPIO_SD_CS, HIGH);
    mmc_spi_readwrite(0xff);
    if (result != 0x01) {
        printk("mmc: card init %d error\n", result);
	restore_flags(flags);
	return (1);
    }

    printk("mmc: card init 2/2\n");
    for (j = 0; j < 10000; j++) {
	gpio_write(GPIO_SD_CS, LOW);

	mmc_spi_readwrite(0x41);
	for (i = 0; i < 4; i++)
	    mmc_spi_readwrite(0x00);
	mmc_spi_readwrite(0xff);
	for (i = 0; i < 8; i++) {
	    result = mmc_spi_readwrite(0xff);
	    if (result == 0x00)
		break;
	}
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	if (result == 0x00) {
	    restore_flags(flags);
	    printk("mmc: card init 3/3\n");
	    return (0);
	}
    }
    restore_flags(flags);

    return (2);
}


static int mmc_spi_card_config(void)
{
    unsigned char result = 0;
    short i;
    unsigned char csd[32];
    unsigned int c_size;
    unsigned int c_size_mult;
    unsigned int mult;
    unsigned int read_bl_len;
    unsigned int blocknr = 0;
    unsigned int block_len = 0;
    unsigned int size = 0;

    gpio_write(GPIO_SD_CS, LOW);
    for (i = 0; i < 4; i++)
	mmc_spi_readwrite(0xff);
    mmc_spi_readwrite(0x49);
    for (i = 0; i < 4; i++)
	mmc_spi_readwrite(0x00);
    mmc_spi_readwrite(0xff);
    for (i = 0; i < 8; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0x00)
	    break;
    }
    if (result != 0x00) {
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	return (1);
    }
    for (i = 0; i < 8; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0xfe)
	    break;
    }
    if (result != 0xfe) {
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	return (2);
    }
    for (i = 0; i < 16; i++) {
	result = mmc_spi_readwrite(0xff);
	csd[i] = result;
    }
    for (i = 0; i < 2; i++) {
	result = mmc_spi_readwrite(0xff);
    }
    gpio_write(GPIO_SD_CS, HIGH);
    mmc_spi_readwrite(0xff);
    if (result == 0x00)
	return (3);

    c_size = csd[8] + csd[7] * 256 + (csd[6] & 0x03) * 256 * 256;
    c_size >>= 6;
    c_size_mult = csd[10] + (csd[9] & 0x03) * 256;
    c_size_mult >>= 7;
    read_bl_len = csd[5] & 0x0f;
    mult = 1;
    mult <<= c_size_mult + 2;
    blocknr = (c_size + 1) * mult;
    block_len = 1;
    block_len <<= read_bl_len;
    size = block_len * blocknr;
    size >>= 10;

    for (i = 0; i < (1 << 6); i++) {
	hd_blocksizes[i] = 1024;
	hd_hardsectsizes[i] = block_len;
	hd_maxsect[i] = 256;
    }
    hd_sizes[0] = size;
    hd[0].nr_sects = blocknr;


    printk("Size = %d, hardsectsize = %d, sectors = %d\n",
	   size, block_len, blocknr);

    return 0;
}


/*
 * *******************************************************************
 *
 *             End of SPI hardware access functions.
 *
 * *******************************************************************
 */


static int mmc_write_block(unsigned int dest_addr, unsigned char *data)
{
    unsigned int address;
    unsigned char result = 0;
    unsigned char ab0, ab1, ab2, ab3;
    int i;

    address = dest_addr;

    ab3 = 0xff & (address >> 24);
    ab2 = 0xff & (address >> 16);
    ab1 = 0xff & (address >> 8);
    ab0 = 0xff & address;
    gpio_write(GPIO_SD_CS, LOW);
    for (i = 0; i < 4; i++)
	mmc_spi_readwrite(0xff);
    mmc_spi_readwrite(0x58);
    mmc_spi_readwrite(ab3);		/* msb */
    mmc_spi_readwrite(ab2);
    mmc_spi_readwrite(ab1);
    mmc_spi_readwrite(ab0);		/* lsb */
    mmc_spi_readwrite(0xff);
    for (i = 0; i < 8; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0x00)
	    break;
    }
    if (result != 0x00) {
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	return (1);
    }

    mmc_spi_readwrite(0xfe);
    for (i = 0; i < 512; i++)
	mmc_spi_readwrite(data[i]);
    for (i = 0; i < 2; i++)
	mmc_spi_readwrite(0xff);

    for (i = 0; i < 1000000; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0xff)
	    break;
    }
    if (result != 0xff) {
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	return (3);
    }
    gpio_write(GPIO_SD_CS, HIGH);
    mmc_spi_readwrite(0xff);
    return (0);
}

static int mmc_read_block(unsigned char *data, unsigned int src_addr)
{
    unsigned int address;
    unsigned char result = 0;
    unsigned char ab0, ab1, ab2, ab3;
    int i;

    address = src_addr;

    ab3 = 0xff & (address >> 24);
    ab2 = 0xff & (address >> 16);
    ab1 = 0xff & (address >> 8);
    ab0 = 0xff & address;

    gpio_write(GPIO_SD_CS, LOW);
    for (i = 0; i < 4; i++)
	mmc_spi_readwrite(0xff);
    mmc_spi_readwrite(0x51);
    mmc_spi_readwrite(ab3);		/* msb */
    mmc_spi_readwrite(ab2);
    mmc_spi_readwrite(ab1);
    mmc_spi_readwrite(ab0);		/* lsb */

    mmc_spi_readwrite(0xff);
    for (i = 0; i < 8; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0x00)
	    break;
    }
    if (result != 0x00) {
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	return (1);
    }
    for (i = 0; i < 100000; i++) {
	result = mmc_spi_readwrite(0xff);
	if (result == 0xfe)
	    break;
    }
    if (result != 0xfe) {
	gpio_write(GPIO_SD_CS, HIGH);
	mmc_spi_readwrite(0xff);
	return (2);
    }
    for (i = 0; i < 512; i++) {
	result = mmc_spi_readwrite(0xff);
	data[i] = result;
    }
    for (i = 0; i < 2; i++) {
	result = mmc_spi_readwrite(0xff);
    }
    gpio_write(GPIO_SD_CS, HIGH);
    mmc_spi_readwrite(0xff);

    return (0);
}

static void mmc_request(request_queue_t * q)
{
    unsigned int mmc_address;
    unsigned char *buffer_address;
    int nr_sectors;
    int i;
    int cmd;
    int result, code;

    (void) q;
    while (1) {
	code = 1;		// Default is success
	INIT_REQUEST;
	mmc_address =
	    (CURRENT->sector +
	     hd[MINOR(CURRENT->rq_dev)].start_sect) * hd_hardsectsizes[0];
	buffer_address = CURRENT->buffer;
	nr_sectors = CURRENT->current_nr_sectors;
	cmd = CURRENT->cmd;
	if (((CURRENT->sector + CURRENT->current_nr_sectors +
	      hd[MINOR(CURRENT->rq_dev)].start_sect) > hd[0].nr_sects)
	    || (mmc_media_detect == 0)) {
	    code = 0;
	} else if (cmd == READ) {
	    spin_unlock_irq(&io_request_lock);
	    for (i = 0; i < nr_sectors; i++) {
		result = mmc_read_block(buffer_address, mmc_address);
		if (result != 0) {
		    printk("mmc: error %d in mmc_read_block\n", result);
		    code = 0;
		    break;
		} else {
		    mmc_address += hd_hardsectsizes[0];
		    buffer_address += hd_hardsectsizes[0];
		}
	    }
	    spin_lock_irq(&io_request_lock);
	} else if (cmd == WRITE) {
	    spin_unlock_irq(&io_request_lock);
	    for (i = 0; i < nr_sectors; i++) {
		result = mmc_write_block(mmc_address, buffer_address);
		if (result != 0) {
		    printk("mmc: error %d in mmc_write_block\n", result);
		    code = 0;
		    break;
		} else {
		    mmc_address += hd_hardsectsizes[0];
		    buffer_address += hd_hardsectsizes[0];
		}
	    }
	    spin_lock_irq(&io_request_lock);
	} else {
	    code = 0;
	}
	end_request(code);
    }
}


static int mmc_open(struct inode *inode, struct file *filp)
{
    int device;
    (void) filp;
    mmc_media_detect = mmc_spi_media_detect();

    if (mmc_media_detect == 0)
	return -ENODEV;

#if defined(MODULE)
    MOD_INC_USE_COUNT;
#endif
    return 0;
}

static int mmc_release(struct inode *inode, struct file *filp)
{
    (void) filp;
    fsync_dev(inode->i_rdev);
    invalidate_buffers(inode->i_rdev);

#if defined(MODULE)
    MOD_DEC_USE_COUNT;
#endif
    return 0;
}

static int mmc_revalidate(kdev_t dev)
{
    int target, max_p, start, i;

    mmc_media_detect = mmc_spi_media_detect();

    if (mmc_media_detect == 0)
	return -ENODEV;

    target = DEVICE_NR(dev);

    max_p = hd_gendisk.max_p;
    start = target << 6;
    for (i = max_p - 1; i >= 0; i--) {
	int minor = start + i;
	invalidate_device(MKDEV(MAJOR_NR, minor), 1);
	hd_gendisk.part[minor].start_sect = 0;
	hd_gendisk.part[minor].nr_sects = 0;
    }

    grok_partitions(&hd_gendisk, target, 1 << 6, hd_sizes[0] * 2);

    return 0;
}

static int mmc_ioctl(struct inode *inode, struct file *filp,
		     unsigned int cmd, unsigned long arg)
{
    if (!inode || !inode->i_rdev)
	return -EINVAL;

    switch (cmd) {
    case BLKGETSIZE:
	return put_user(hd[MINOR(inode->i_rdev)].nr_sects,
			(unsigned long *) arg);
    case BLKGETSIZE64:
	return put_user((u64) hd[MINOR(inode->i_rdev)].
			nr_sects, (u64 *) arg);
    case BLKRRPART:
	if (!capable(CAP_SYS_ADMIN))
	    return -EACCES;

	return mmc_revalidate(inode->i_rdev);
    case HDIO_GETGEO:
	{
	    struct hd_geometry *loc, g;
	    loc = (struct hd_geometry *) arg;
	    if (!loc)
		return -EINVAL;
	    g.heads = 4;
	    g.sectors = 16;
	    g.cylinders = hd[0].nr_sects / (4 * 16);
	    g.start = hd[MINOR(inode->i_rdev)].start_sect;
	    return copy_to_user(loc, &g, sizeof(g)) ? -EFAULT : 0;
	}
    default:
	return blk_ioctl(inode->i_rdev, cmd, arg);
    }
}


static int mmc_check_media_change(kdev_t dev)
{
    (void) dev;
    if (mmc_media_changed == 1) {
	mmc_media_changed = 0;
	return 1;
    } else
	return 0;
}

static struct block_device_operations mmc_bdops = {
  open:mmc_open,
  release:mmc_release,
  ioctl:mmc_ioctl,
/* FIXME: add media change support
 *	check_media_change: mmc_check_media_change,
 *	revalidate: mmc_revalidate,
 */
};

static struct gendisk hd_gendisk = {
  major:MAJOR_NR,
  major_name:DEVICE_NAME,
  minor_shift:6,
  max_p:1 << 6,
  part:hd,
  sizes:hd_sizes,
  fops:&mmc_bdops,
};

static int mmc_init(void)
{
    int result;

    result = mmc_spi_hardware_init();

    if (result != 0) {
	printk("mmc: error %d in mmc_spi_hardware_init\n", result);
	return -1;
    }

    result = mmc_spi_card_init();
    if (result != 0) {
	// Give it an extra shot
	result = mmc_spi_card_init();
	if (result != 0) {
	    printk("mmc: error %d in mmc_card_init\n", result);
	    return -1;
	}
    }

    memset(hd_sizes, 0, sizeof(hd_sizes));
    result = mmc_spi_card_config();
    if (result != 0) {
	printk("mmc: error %d in mmc_card_config\n", result);
	return -1;
    }


    blk_size[MAJOR_NR] = hd_sizes;

    memset(hd, 0, sizeof(hd));
    hd[0].nr_sects = hd_sizes[0] * 2;

    blksize_size[MAJOR_NR] = hd_blocksizes;
    hardsect_size[MAJOR_NR] = hd_hardsectsizes;
    max_sectors[MAJOR_NR] = hd_maxsect;

    hd_gendisk.nr_real = 1;

    register_disk(&hd_gendisk, MKDEV(MAJOR_NR, 0), 1 << 6,
		  &mmc_bdops, hd_sizes[0] * 2);

    return 0;
}

static void mmc_exit(void)
{
    blk_size[MAJOR_NR] = NULL;
    blksize_size[MAJOR_NR] = NULL;
    hardsect_size[MAJOR_NR] = NULL;
    max_sectors[MAJOR_NR] = NULL;
    hd[0].nr_sects = 0;
}

static void mmc_check_media(void)
{
    int old_state, new_state;
    int result;

    old_state = mmc_media_detect;
    new_state = mmc_spi_media_detect();

    if (old_state != new_state) {
	mmc_media_changed = 1;
	if (new_state == PRESENT) {
	    result = mmc_init();
	    if (result != 0)
		printk("mmc: error %d in mmc_init\n", result);
	} else {
	    mmc_exit();
	}
    }
}

static int __init mmc_driver_init(void)
{
    int result;

    result = devfs_register_blkdev(MAJOR_NR, DEVICE_NAME, &mmc_bdops);
    if (result < 0) {
	printk(KERN_WARNING "mmc: can't get major %d\n", MAJOR_NR);
	return result;
    }

    blk_init_queue(BLK_DEFAULT_QUEUE(MAJOR_NR), mmc_request);
    mmc_check_media();
    read_ahead[MAJOR_NR] = 8;
    add_gendisk(&hd_gendisk);


    return 0;
}

static void __exit mmc_driver_exit(void)
{
    int i;
    del_timer(&mmc_timer);

    for (i = 0; i < (1 << 6); i++)
	fsync_dev(MKDEV(MAJOR_NR, i));

    blk_cleanup_queue(BLK_DEFAULT_QUEUE(MAJOR_NR));
    del_gendisk(&hd_gendisk);
    devfs_unregister_blkdev(MAJOR_NR, DEVICE_NAME);
    mmc_exit();
}

module_init(mmc_driver_init);
module_exit(mmc_driver_exit);
