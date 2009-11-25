#include "start.h"
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#ifdef RTL8186
#include <asm/rtl8186.h>
#endif
#ifdef RTL8196
#include <asm/rtl8196.h>
#endif
        
#ifndef boot32        
#define NEWSTACK        (0x80670000)
#else
#define NEWSTACK        (0x809FFF00)
#endif
	.globl old_stack_pointer
	.extern __start_dram
        .text
        .globl __start              
__start:
        /* Check chip version to set divider register */
        li              t1, REG_REVISION
        lw              t0, 0(t1)
        li              t1, 0xf0000000
        and             t0, t0, t1
        srl             t0, t0, 0x1c                    // Right shift 28 bits

        /* Check PLLMNR and SYSCLKR Registers. */
        la      a0, REG_PLLMNR
        lw              t0, 0(a0)
        and             t0, t0, 0x3FFFF                 // You must mask other bits!!
        li      t1, VAL_PLLMNR_8186C
        bne             t0, t1, set_C
        nop

        la      a0, REG_SYSCLKR
        lw              t0, 0(a0)
        and             t0, t0, 0xFFF                   // You must mask other bits!!
        li      t1, VAL_SYSCLKR_8186C
        bne             t0, t1, set_C
        nop

set_C:
        /* Set CPU/MEM Clock and Watch Dog Reset. */
        li              t0, VAL_PLLMNR_8186C
        la              a0, REG_PLLMNR
        sw              t0, 0(a0)
        li              t0, VAL_SYSCLKR_8186C
        la              a0, REG_SYSCLKR
        sw              t0, 0(a0)
        nop

	/* Set CPU/MEM Clock double 2*/
	li  t0, VAL_PLLMNR_8186C
        li  t1, REG_PLLMNR
        sw  t0, 0(t1)
	nop
	li  t0, VAL_SYSCLKR_8186C // CPU/MEM divider register
        li  t1, REG_SYSCLKR
        sw  t0, 0(t1)
        nop

	/* zero bss */
	move s0, zero
	mtc0 s0, $12
	nop
	nop

	/* Modify CPU/SDRAM/FLASH timing */
	li  t0, 0x8a8		 // more aggressive
	li  t1, REG_MTCR1	 // Memory timing configuration register CAS (mtcr1)
	sw  t0, 0(t1)
	nop
	li  t0, VAL_FLASH_TIMING // flash timing
        li  t1, REG_MTCR0	 // Memory timing configuration register RAS (mtcr0)
        sw  t0, 0(t1)
	nop

        /* modify arbiter settings */
        la  t1, REG_TKNR
        la  t0, 0x82828282
        sw  t0, 0(t1)

        /* ack all interrupts at first */
        la  t1, REG_GISR
        la  t0, 0xffffffff
        sw  t0, 0(t1)

        /* flush all cache */
        mtc0    zero, $20
        nop
        nop
        li      t0, 0x3
        mtc0    t0, $20
        nop
        nop
        mtc0    zero, $20

        /*--- initialize and start COP3*/
        mfc0    $8,$12
        nop
        nop
        or      $8,0x80000000
        mtc0    $8,$12
        nop
	nop
	la   a0, 0x90000000
	mtc0 a0, $12
	nop
	nop
	mfc0 t0, $20
	nop
	nop
	ori  t0, 0x1
	mtc0 t0, $20
	nop
	nop 

	la   a0, 0x10000000
	mtc3 a0, $4
	la   a0, 0x10000fff
	nop
	mtc3 a0, $5
	nop
        nop

        la      t0, 0x81000000  
        and     t0, 0x0fffffff 					# translate to physical address
        mtc3    t0, $4                                          # $4: d-ram base
        nop
        addi    t0, t0, 0x1000 - 1				# total 2k bytes used//2003-07-04//TKIP_encryption
        mtc3    t0, $5                                          # $5: d-ram top
        nop

	la s0, _bstart
        la s1, _bend

        move t0, s0

1:
        sw zero, 0(t0)
        addi t0, 4
        bne t0, s1, 1b
	move t0, s1
	addi t0, t0, 4096
 	move sp, t0
 	move a0, t0

        j main
        nop

        .globl exit                
exit:
        j exit
        nop

        .globl start_kernel
start_kernel:
        move t0, a0
        li a0, 0
        li a1, 0
        li a2, 0
		jr t0
       	nop 
