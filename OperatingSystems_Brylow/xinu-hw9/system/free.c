/**
 * COSC 3250 - Project 7
 * implements free.c file
 * @authors Colby Gustafson Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu noah.bayer@marquette.edu
 */

/**
 * @file free.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Attempt to free a block of memory based on malloc() accounting information
 * stored in preceding two words.
 *
 * @param ptr
 *      A pointer to the memory block to free.
 */
syscall free(void *ptr)
{
    struct memblock *block;
	ulong numBytes;

    /* TODO:
     *      1) set block to point to memblock to be free'd (ptr)
     *      2) find accounting information of the memblock
     *      3) call freemem syscall on the block with its length
     */

	//setting block to point to be free'd (ptr)
	block = (struct memblock *)ptr;

	//finding accounting information of the memblock
	block--;
	numBytes = block->length;
	kprintf("numBytes: %d\r\n", numBytes);

	//calling freemem syscall on the block with its length
	freemem(block, numBytes); 

    return OK;
}
