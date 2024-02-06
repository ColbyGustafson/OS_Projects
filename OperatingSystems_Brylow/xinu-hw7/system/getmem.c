/**
 * COSC 3250 - Project 7
 * implements getmem.c file
 * @authors Colby Gustafson Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu noah.bayer@marquette.edu
 */

/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Disable interrupts
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      - Restore interrupts
     *      - return memory address if successful
     */

	lock_acquire(&(freelist.lock));
	
	//initializing
	curr = (struct memblock *)(freelist.head);
	prev = (struct memblock *)&freelist;
	memhead * freehead = &freelist;

	//iterates through freelist
	while(curr != NULL)
	{
		//if curr is optimal size
		if((curr->length) > nbytes)
		{
			//adjusts leftover
			leftover = (struct memblock *)(nbytes + ((ulong)curr));
			leftover->length = (curr->length) - nbytes;			
			freehead->length = freehead->length - nbytes;

			//relink prev/curr
			leftover->next = curr->next;
			prev->next = leftover;
			curr->length = nbytes;

			lock_release(&(freelist.lock));
			return (curr);
		}
		//set up for next iter
		prev = curr;
		curr = curr->next;
	}

    return (void *)SYSERR;
}
