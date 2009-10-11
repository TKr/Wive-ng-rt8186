/*
 * r2300.c: R2000 and R3000 specific mmu/cache code.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 *
 * with a lot of changes to make this thing work for R3000s
 * Tx39XX R4k style caches added. HK
 * Copyright (C) 1998, 1999, 2000 Harald Koerfgen
 * Copyright (C) 1998 Gleb Raiko & Vladimir Roganov
 */
#include <linux/module.h>	// for wlan driver module, david+2006-1-19
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mmu_context.h>
#include <asm/system.h>
#include <asm/isadep.h>
#include <asm/io.h>
#include <asm/wbflush.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>

#undef DEBUG_CACHE


static void r3k_flush_icache_range(unsigned long start, unsigned long end)
{

   unsigned long flags;
    volatile unsigned int reg;
    save_flags(flags);cli();
    reg=read_32bit_cp0_register(CP0_XCONTEXT);
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    write_32bit_cp0_register(CP0_XCONTEXT, (reg &(~0x2))); //write '0' to bit 0,1
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    write_32bit_cp0_register(CP0_XCONTEXT, (reg | 0x2)); //wirte '1' to bit 0, 1
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    restore_flags(flags);



}

void r3k_flush_dcache_range(unsigned long start, unsigned long end)
{

    unsigned long flags;
    volatile unsigned int reg;
    save_flags(flags);cli();
    reg=read_32bit_cp0_register(CP0_XCONTEXT);
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    write_32bit_cp0_register(CP0_XCONTEXT, (reg & (~0x1))); //write '0' to bit 0,1
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    write_32bit_cp0_register(CP0_XCONTEXT, (reg | 0x1)); //wirte '1' to bit 0, 1
    __asm__ volatile("nop");
    __asm__ volatile("nop");
    restore_flags(flags);

}

static inline unsigned long get_phys_page (unsigned long addr,
					   struct mm_struct *mm)
{
	pgd_t *pgd;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long physpage;

	pgd = pgd_offset(mm, addr);
	pmd = pmd_offset(pgd, addr);
	pte = pte_offset(pmd, addr);

	if ((physpage = pte_val(*pte)) & _PAGE_VALID)
		return KSEG0ADDR(physpage & PAGE_MASK);

	return 0;
}

static inline void r3k_flush_cache_all(void)
{
	r3k_flush_icache_range(KSEG0, KSEG0 );
}
 
static inline void r3k___flush_cache_all(void)
{
	r3k_flush_icache_range(KSEG0, KSEG0 );
}

static void r3k_flush_cache_mm(struct mm_struct *mm)
{
}

static void r3k_flush_cache_range(struct mm_struct *mm, unsigned long start,
				  unsigned long end)
{
}

static void r3k_flush_cache_page(struct vm_area_struct *vma,
				   unsigned long page)
{
}

static void r3k_flush_page_to_ram(struct page * page)
{
	/*
	 * Nothing to be done
	 */
}

static void r3k_flush_icache_page(struct vm_area_struct *vma, struct page *page)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long physpage;

	if (mm->context == 0)
		return;

	if (!(vma->vm_flags & VM_EXEC))
		return;

#ifdef DEBUG_CACHE
	printk("cpage[%d,%08lx]", (int)mm->context, page);
#endif

	physpage = (unsigned long) page_address(page);
	if (physpage)
		r3k_flush_icache_range(physpage, physpage + PAGE_SIZE);
}

static void r3k_flush_cache_sigtramp(unsigned long addr)
{
	
	unsigned long flags;

#ifdef DEBUG_CACHE
	printk("csigtramp[%08lx]", addr);
#endif
	save_and_cli(flags);
    
	r3k___flush_cache_all();
	restore_flags(flags);

#if 0
	flags = read_32bit_cp0_register(CP0_STATUS);

	write_32bit_cp0_register(CP0_STATUS, flags&~ST0_IEC);

	/* Fill the TLB to avoid an exception with caches isolated. */
	asm ( 	"lw\t$0, 0x000(%0)\n\t"
		"lw\t$0, 0x004(%0)\n\t"
		: : "r" (addr) );

	write_32bit_cp0_register(CP0_STATUS, (ST0_ISC|ST0_SWC|flags)&~ST0_IEC);

	asm ( 	"sb\t$0, 0x000(%0)\n\t"
		"sb\t$0, 0x004(%0)\n\t"
		: : "r" (addr) );

	write_32bit_cp0_register(CP0_STATUS, flags);
#endif
}

static void r3k_dma_cache_wback_inv(unsigned long start, unsigned long size)
{
	wbflush();
	r3k_flush_dcache_range(start, start + size);
}

void __init ld_mmu_r23000(void)
{
	_clear_page = r3k_clear_page;
	_copy_page = r3k_copy_page;

	_flush_cache_all = r3k___flush_cache_all;
	___flush_cache_all = r3k___flush_cache_all;
	_flush_cache_mm = r3k_flush_cache_mm;
	_flush_cache_range = r3k_flush_cache_range;
	_flush_cache_page = r3k_flush_cache_page;
	_flush_cache_sigtramp = r3k_flush_cache_sigtramp;
	_flush_page_to_ram = r3k_flush_page_to_ram;
	_flush_icache_page = r3k_flush_icache_page;
	_flush_icache_range = r3k_flush_icache_range;

	//_dma_cache_wback_inv = r3k_dma_cache_wback_inv;


}

// for wlan driver module, david+2006-1-19
EXPORT_SYMBOL(r3k_flush_dcache_range);

