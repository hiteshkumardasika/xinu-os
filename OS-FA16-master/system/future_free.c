#include <xinu.h>

syscall future_free(future *f) {
    f->value = NULL;
    f->state = FUTURE_FREE;
}
