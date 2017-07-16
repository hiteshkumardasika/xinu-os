#include <xinu.h>
#include <future.h>

syscall future_set(future *f, int *value) {
    intmask mask;            /* Saved interrupt mask		*/
    struct procent *prptr;        /* Ptr to process' table entry	*/

    mask = disable();
    if (f->flag == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_EMPTY) {
            f->value = *value;
            f->state = FUTURE_VALID;
            restore(mask);
            return OK;
        } else if (f->state == FUTURE_WAITING) {
            f->value = *value;
            f->state = FUTURE_VALID;
            resume(f->pid);
            restore(mask);
            return OK;
        } else {
            restore(mask);
            return SYSERR;
        }
    } else if (f->flag == FUTURE_SHARED) {
        if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING) {
            f->value = *value;
            f->state = FUTURE_VALID;
            resched_cntl(DEFER_START);
            while (!isempty(f->get_queue)) {
                ready(getfirst(f->get_queue));
            }
            resched_cntl(DEFER_STOP);
            restore(mask);
            return OK;
        }
    } else if (f->flag == FUTURE_QUEUE) {
        if (f->state == FUTURE_EMPTY) {
            prptr = &proctab[getpid()];
            prptr->prstate = PR_WAIT;    /* Set state to waiting	*/
            enqueue(getpid(), f->set_queue);
            resched();
            f->value = *value;
            //f->state = FUTURE_VALID;
            restore(mask);
            return OK;
        } else if (f->state == FUTURE_WAITING) {
            f->value = *value;
            //f->state = FUTURE_VALID;
            if (!isempty(f->get_queue)) {
                ready(getfirst(f->get_queue));
            }
            restore(mask);
            return OK;
        }
    }
}

