/**
 * COSC 3250 - Project 7
 * implements kill.c file
 * @authors Colby Gustafson Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu noah.bayer@marquette.edu
 */

/**
 * @file kill.c
 * Provides: kill
 *
 * COSC 3250 Assignment 6
 */

/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/*
 * kill  --  kill a process and remove it from the system
 */
syscall kill(int pid)
{
    pcb *ppcb;                  /* points to process control block for pid */

    if (isbadpid(pid) || (0 == pid)
        || (PRFREE == (ppcb = &proctab[pid])->state))
    {
        return SYSERR;
    }

    ppcb = &proctab[pid];

    --numproc;

    switch (ppcb->state)
    {
    case PRCURR:
        ppcb->state = PRFREE;   /* suicide */
	freemem(ppcb, ppcb->stklen);
        resched();

    case PRREADY:
        remove(pid);

    default:
        ppcb->state = PRFREE;
	freemem(ppcb, ppcb->stklen);
    }

    return OK;
}
