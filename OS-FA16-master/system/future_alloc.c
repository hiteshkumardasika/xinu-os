#include <xinu.h>
#include <future.h>

local int newfuture(void);


future *future_alloc(int future_flags) {

    intmask mask;            /* Saved interrupt mask		*/
    future *f_temp;
    int index;

    mask = disable();

    if (future_flags == NULL) {
        restore(mask);
        return SYSERR;
    }
/*
    f_object = (future *) getmem(sizeof(future));
    if ((int32) f_object == SYSERR) {
        restore(mask);
        return SYSERR;
    }
*/
    if ((index = newfuture()) == SYSERR) {
        restore(mask);
        return SYSERR;
    }

    f_temp = &futab[index];
/*
    if (f_temp->value = (int *) getmem(sizeof(int)) == SYSERR) {
        restore(mask);
        return SYSERR;
    }
    if (f_temp->flag = (int *) getmem(sizeof(int)) == SYSERR) {
        restore(mask);
        return SYSERR;
    }
*/
    f_temp->flag = future_flags;
    //f_temp->set_queue = newqueue();
    //f_temp->get_queue = newqueue();
    f_temp->pid = NULL;

    restore(mask);
    return f_temp;
}


local int newfuture(void) {
    int32 i;            /* Iterate through # entries	*/

    for (i = 0; i < NFUTURE; i++) {
        if (futab[i].state == FUTURE_FREE) {
            futab[i].state = FUTURE_EMPTY;
            return i;
        }
    }
    return SYSERR;

}

