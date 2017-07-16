#ifndef _FUTURE_H
#define _FUTURE_H

#ifndef    NFUTURE
#define    NFUTURE        4    /* Number of future objects, if not defined	*/
#endif

/* define states */
#define FUTURE_FREE      -1
#define FUTURE_EMPTY      0
#define FUTURE_WAITING    1
#define FUTURE_VALID      2

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1
#define FUTURE_SHARED     2
#define FUTURE_QUEUE      3

extern pid32 main_pid;

typedef struct futent {
    int *value;
    int flag;
    int state;
    pid32 pid;
    qid16 set_queue;
    qid16 get_queue;
    char* fut_msg;
} future;

extern struct futent futab[];

/* Interface for system call */
future *future_alloc(int future_flags);

syscall future_free(future *);

syscall future_get(future *, int *);

syscall future_set(future *, int *);

uint future_prod(future *);

uint future_cons(future *);

int future_netw_cons(future *fut);

#endif
