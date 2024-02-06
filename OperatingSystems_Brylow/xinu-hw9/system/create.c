/**
 * COSC 3250 - Project 7
 * implements create.c file
 * @authors Colby Gustafson Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu noah.bayer@marquette.edu
 */

/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 Assignment 7
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, uint tickets, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong *savargs;		/* pointer to arg saving region */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
	saddr = (ulong *)getmem(ssize);     /* allocate new stack and pid   */
    	saddr = ssize + saddr;
	saddr--;
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];

    /* setup PCB entry for new proc */
    ppcb->state = PRSUSP;

    // TODO: Setup PCB entry for new process.
    strncpy(ppcb->name, name, PNMLEN);
    ppcb->stkbase = (ulong *)(((ulong)saddr) - ssize);
    ppcb->stkptr  = NULL;
    ppcb->stklen  = ssize;
	ppcb->tickets = tickets;

    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 4) * 4;
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

    // TODO: Initialize process context.
    //
    // TODO:  Place arguments into activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.    

    /* In ctxsw(), CTX_SP will get popped into LR, */
    /*  and CTX_LR will get popped into PC.        */
    savargs = saddr;
    for (i = 0; i <= CTX_LR; i++)
    {  *--saddr = 0;  }
	
    saddr[CTX_SP] = (int)userret;
    saddr[CTX_LR] = (int)funcaddr;
    ppcb->stkptr  = saddr;

    /* Place arguments into activation records. */
    va_start(ap, nargs);
    for (i = 0; i < nargs; i++)
    {
	if (i < 4)
		saddr[i] = va_arg(ap, ulong);
	else
		savargs[i - 4] = va_arg(ap, ulong);
    }
    va_end(ap);

	saddr[CTX_SP] = &saddr[CTX_PC+1];
	saddr[CTX_LR] = &userret;
	saddr[CTX_PC] = funcaddr;

	saddr[CTX_IP] = (ARM_MODE_SYS | ARM_F_BIT);

    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    kill(currpid);
}
