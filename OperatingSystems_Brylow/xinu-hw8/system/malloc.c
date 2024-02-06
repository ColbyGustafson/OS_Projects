/**
 * COSC 3250 - Project 7
 * implements malloc.c file
 * @authors Colby Gustafson Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu noah.bayer@marquette.edu
 */

/**
 * @file malloc.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    struct memblock *pmem, *lastAddr;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

     /** TODO:
      *      1) Make room for accounting info
      *      2) Acquire memory with getmem syscall
      *         - handle possible error (SYSERR) from getmem...
      *      3) Set accounting info in pmem
      *      4) Return proper pointer to base of free memory region
      */

	//making room for accounting info
	pmem->next = pmem;
	pmem->length = size;

	//acquiring memory with getmem syscall
	pmem = getmem(size + sizeof(struct memblock));
	
	//setting accounting info in pmem
	lastAddr = pmem +1;
	
	//returning proper pointer to base of free memory region
	return lastAddr;


    //return NULL;
}
