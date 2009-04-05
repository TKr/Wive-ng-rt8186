#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>        
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
	/* zero bss */
	move s0, zero
	mtc0 s0, $12
	nop
	nop

	/* Modify CPU/SDRAM/FLASH timing */
	li  t0, 0x8a8		// more aggressive MEM=123MHz
	li  t1, 0xbd011008      // mem speed adress (mtcr1)
	sw  t0, 0(t1)
	li  t0, 0x88880000	// flash timing
        li  t1, 0xbd011004      // mem speed adress (mtcr0)
        sw  t0, 0(t1)
	nop
	nop
	nop
	nop
	nop

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
