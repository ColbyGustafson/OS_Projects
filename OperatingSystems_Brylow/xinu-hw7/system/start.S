/**
 * @file start.S
 *
 * Initialization code for Embedded Xinu on the Raspberry Pi.
 *
 * Embedded Xinu, Copyright (C) 2013.  All rights reserved.
 *
 * -----------------------------------------------------------------------------------------
 * 
 * Specific for Raspberry Pi 3 (BCM2837) - Memory Layout (Not to scale)
 * 1GB OF RAM (PHYSICAL MEMORY) AVAILABLE. 1GB == 0x3FFFFFFF
 * +----------+
 * |	      |
 * | IO SPACE | -> 0x3F000000 to 0x3FFFFFFF reserved for IO (GPIO, UART, SYS TIMER, USB CORE)
 * |          |
 * +----------+
 * |          |
 * |          |
 * |   HEAP   | -> getmem allocates from here
 * |          |
 * |          |
 * +----------+
 * | OS STACK | -> becomes the null process' stack
 * +----------+
 * |   BSS    | -> needed for C environment
 * +----------+
 * |   TEXT   | -> XINU code
 * |          |
 * +----------+
 * | RESERVED | -> interrupt handler and vectors go here
 * +----------+
 */

#include <arm.h>     /* For ARM_MODE_SYS */

#define NULLSTK 8192

.section .init
	.globl _start

	/* _start:  Entry point of the Xinu kernel.  This will be the very first
	/* byte of the kernel image and on the Raspberry Pi 3 (32-bit mode) will be loaded by
	/* the GPU at address 0x8000.  */

	.func _start
_start:
	/* Save the pointer to the atags (ARM boot tags). The Pi 3 *does* pass atags,
	 * and does so into address 0x100. We access it there, just like the Pi 1.
	 * The standard protocol for Aarch32 (ARMv8-A) is to pass the atags pointer
	 * into register r2. We then store it in r3, to be accessed by C code which
	 * parses the atags. */
	ldr 	r3, =atags_ptr
	cmp	r2, #0
	moveq   r2, #0x100
	str 	r2, [r3]

	/* Check if processor is already in SYSTEM mode,
	 * if it is, jump to reset handler. */
	mrs	r0, cpsr
	and	r0, r0, #0b11111
	cmp	r0, #ARM_MODE_SYS
	beq	reset_handler
	

       /* Upon boot, the device loads into HYP (hypervisor) mode, which does not have permission to
	* access the CPS opcode. Therefore, to enter the most privelaged mode,
	* SYSTEM mode (11111), we first need to enter SUPERVISOR mode. This allows more privelage,
	* so that the "cps" opcode may be used to switch to SYSTEM mode.
	* First clear bits of CPSR, then enter supervisor mode. Increment the program counter
        * by four to skip a 32-bit word. Upon exception return, the CPSR is changed, and
	* we are in SUPERVISOR mode. This continues at reset handler, where SYSTEM mode is set. */
	
	
	/* Go from monitor mode to hypervisor mode */
	/* Required to use exception return method */
	/* Continues execution at line "b reset_handler" */
	mrs	r0, cpsr
//	bic	r0, r0, #ARM_MODE_SYS
	orr	r0, r0, #ARM_MODE_SYS
	msr	spsr_cxsf, r0
	add	r0, pc, #4
	msr	ELR_hyp, r0
	eret
        
	/* Continue at reset handler. */
	b reset_handler
	.endfunc

/* ARM exception vector table.  This is copied to VBAR register. */
/* Vector table has to be 32-byte aligned. */
.balign	0x20
.globl _vectors
_vectors:
	ldr pc, reset_addr	  /* Reset handler */
	ldr pc, undef_addr	  /* Undefined instruction handler */
	ldr pc, swi_addr	  /* Software interrupt handler */
	ldr pc, prefetch_addr	  /* Prefetch abort handler */
	ldr pc, abort_addr	  /* Data abort handler */
	ldr pc, reserved_addr	  /* Reserved */
	ldr pc, irq_addr	  /* IRQ (Interrupt request) handler */
	ldr pc, fiq_addr	  /* FIQ (Fast interrupt request) handler */

reset_addr:     .word reset_handler
undef_addr:     .word undef_handler
swi_addr:       .word swi_handler
prefetch_addr:  .word prefetch_handler
abort_addr:     .word abort_handler
reserved_addr:  .word reset_handler
irq_addr:       .word irq_handler
fiq_addr:       .word fiq_handler

_endvectors:

.section .text

	/* reset_handler: Reset handler routine executed to start up the kernel,
	 * when the ARM processor is reset, or (currently) when an unhandled
	 * exception occurs.  */
	.func reset_handler
reset_handler:

	/* After entering SUPERVISOR mode, enter SYSTEM mode with all interrupts
	 * disabled so the ARM processor is in a known state. */
//	cpsid	if, #ARM_MODE_SYS
	cpsid	aif
	/* Copy the ARM exception table from .init section to address 0,
	 * including the absolute address table.  Here we assume it is exactly
	 * 16 words.  */
	ldr	r1, =_vectors
	mcr	p15, 0, r1, c12, c0, 0	/* Write to VBAR */

	/* Clear the .bss section of the kernel.  */
	ldr r0, =_bss
	ldr r1, =_end
  	mov r2, #0
    	mov r3, #0
	mov r4, #0
	mov r5, #0
  	b bssloopa

bssloopb:
	stmia r0!, {r2-r5}

bssloopa:
	cmp r0, r1 	/* check that we still haven't hit the end of bss yet */
	blo bssloopb	/* if still below, go backwards and loop */

//	ldr	r3, =core_init_sp

	/* Put the null thread's stack directly after the kernel image.  */
	add sp, r1, #NULLSTK	/* Core 0 stack pointer */
//	str	sp, [r3, #0]	
	
//	add	r0, sp, #NULLSTK	/* Core 1 stack pointer */
//	str	r0, [r3, #4]

//	add	r1, r0, #NULLSTK	/* Core 2 stack pointer */
//	str	r1, [r3, #8]

//	add	r2, r1, #NULLSTK	/* Core 3 stack pointer */
//	str	r2, [r3, #12]


	/* The remaining memory available to the ARM will be Xinu's "memheap"
	 * region, which is used for dynamic memory allocation.  Set its
	 * starting address.  */
	ldr r0, =memheap
//  str r2, [r0]		/* use r2 because this is the last stack allocated for core 3 */
	str	sp,	[r0]

	/* Move to ARM register r0 from coprocessor register p15, at op2 5, which is the MPIDR. */
	/* If left shift is zero, start. If not, wait for interrupts in a loop. */
	/* Refer to ARM Cortex-A53 documentation, Section 4.4.1, Table 4-122: */
	/* http://infocenter.arm.com/help/topic/com.arm.doc.ddi0500d/DDI0500D_cortex_a53_r0p2_trm.pdf */
//	mrc p15, 0, r0, c0, c0, 5
//	mov	r1, #3
//	and	r1, r0, r1
//	cmp	r1, #0
	
	/* Branch to nulluser function, located in xinu/system/initialize.c
	/* The function will initialize the system and become the null thread. */
//	beq	nulluser
	b	nulluser
	.endfunc


.globl dmb
dmb:
	.func dmb
	dmb
	mov	pc, lr
	.endfunc
