/*
 * COSC 3250 - Project 4
 * program creating context switch
 * @authors Colby Gustafson & Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu & noah.bayer@marquette.edu
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

// TODO: Save process state, including at least the 
//	callee-save ("non-volatile") registers.
	str	r3, [sp, #CTX_LR*4]
	str	r4, [sp, #CTX_SP*4]

	ldr	sp, [r0]
	ldr	sp, [r1]

// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create().
	ldr	r0, [sp, #CTX_R0*4]
	ldr	r1, [sp, #CTX_R1*4]
	ldr	r2, [sp, #CTX_R2*4]

// TODO: Jump to next function.
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	link register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.

// Note: This bx lr line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.
	bx	lr

	.end ctxsw
