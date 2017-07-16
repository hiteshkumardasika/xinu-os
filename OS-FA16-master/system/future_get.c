#include <xinu.h>
#include "future.h"

syscall future_get(future *f, int *value) {

    intmask mask;            /* Saved interrupt mask		*/
    struct procent *prptr;        /* Ptr to process' table entry	*/
    mask = disable();
    if (f->flag == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_VALID) {
            *value = f->value;
            f->state = FUTURE_EMPTY;
            restore(mask);
            return OK;
        } else if (f->state == FUTURE_EMPTY) {
            f->state = FUTURE_WAITING;
            f->pid = getpid();
            suspend(f->pid);
            *value = f->value;
            restore(mask);
            return OK;
        } else {
            restore(mask);
            return SYSERR;
        }
    } else if (f->flag == FUTURE_SHARED) {
        if (f->state == FUTURE_VALID) {
            *value = f->value;
            restore(mask);
            return OK;
        } else if (f->state == FUTURE_EMPTY) {
            f->state = FUTURE_WAITING;
            prptr = &proctab[getpid()];
            prptr->prstate = PR_WAIT;    /* Set state to waiting	*/
            enqueue(getpid(), f->get_queue);
            resched();
            *value = f->value;
            restore(mask);
            return OK;
        } else if (f->state == FUTURE_WAITING) {
            prptr = &proctab[currpid];
            prptr->prstate = PR_WAIT;    /* Set state to waiting	*/
            enqueue(getpid(), f->get_queue);
            resched();
            *value = f->value;
            restore(mask);
            return OK;
        } else {
            restore(mask);
            return SYSERR;
        }

    } else if (f->flag == FUTURE_QUEUE) {
/*
        if (f->state == FUTURE_VALID) {
            *value = f->value;
            restore(mask);
            return OK;
*/
        if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING) {
            if (!isempty(f->set_queue)) {
                ready(getfirst(f->set_queue));
            } else {
                f->state = FUTURE_WAITING;
                prptr = &proctab[currpid];
                prptr->prstate = PR_WAIT;    /* Set state to waiting	*/
                enqueue(getpid(), f->get_queue);
                resched();
            }
            *value = f->value;
            restore(mask);
            return OK;
        }

    } else {
        restore(mask);
        return SYSERR;
    }
}

