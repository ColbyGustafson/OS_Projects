/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:

// TODO: Save callee-save ("non-volatile") registers.
// TODO: Restore callee-save ("non-volatile") registers.
// TODO: Jump to next function.
	sub	sp, sp, #60
	str	r0, [sp, #CTX_R0*4]
	str	r1, [sp, #CTX_R1*4]
	str	r2, [sp, #CTX_R2*4]
	str	r3, [sp, #CTX_R3*4]
	str	r4, [sp, #CTX_R4*4]
	str	r5, [sp, #CTX_R5*4]
	str	r6, [sp, #CTX_R6*4]
	str	r7, [sp, #CTX_R7*4]
	str	r8, [sp, #CTX_R8*4]
	str	r9, [sp, #CTX_R9*4]
	str	r10, [sp, #CTX_R10*4]
	str	r11, [sp, #CTX_R11*4]
	str	lr, [sp, #CTX_SP*4]
	str	lr, [sp, #CTX_LR*4]


	str	sp, [r0]
	ldr	sp, [r1]

	ldr	r0, [sp, #CTX_R0*4]
	ldr	r1, [sp, #CTX_R1*4]
	ldr	r2, [sp, #CTX_R2*4]
	ldr	r3, [sp, #CTX_R3*4]
	ldr	r4, [sp, #CTX_R4*4]
	ldr	r5, [sp, #CTX_R5*4]
	ldr	r6, [sp, #CTX_R6*4]
	ldr	r7, [sp, #CTX_R7*4]
	ldr	r8, [sp, #CTX_R8*4]
	ldr	r9, [sp, #CTX_R8*4]
	ldr	r10, [sp, #CTX_R10*4]
	ldr	r11, [sp, #CTX_R11*4]

	ldr	lr, [sp, #CTX_SP*4]
	ldr	ip, [sp, #CTX_LR*4]

	add	sp, sp, #60

	bx	ip

	.end ctxsw
