#include <xinu.h>
#include <prodcons.h>
#include <stdio.h>

int n;

sid32 produced, consumed;
pid32 main_pid;

shellcmd xsh_prodcons(int nargs, char *args[]) {
    int count = 20;
    int future_flag = 0;
    produced = semcreate(0);
    consumed = semcreate(1);
    future *f_exclusive, *f_shared, *f_queue;

    main_pid = getpid();
    if (nargs == 2 && strncmp(args[1], "--help", 5) == 0) {
        printf("Usage: %s\n", args[0]);
        printf("Description:\n");
        printf("Small Simulation of producer consumer problem\n");
        printf("One argument needed: A Maximum value till which numbers are produced and consumed\n");
        return OK;
    }

    if (nargs == 2) {
        if (strncmp(args[1], "-f", 3) == 0) {
            future_flag = 1;
        } else if (atoi(args[1]) == 0) {
            printf("Wrong Arguments given ...please use --help for help details");
            return OK;
        } else {
            count = atoi(args[1]);
        }
    }
    if (nargs > 2) {
        printf("Sorry!!Enter only two arguments.\nFor Usage --help\n");
        return OK;
    }
    if (nargs == 1) {
        printf("Too few arguments!!..check --help\n");
        return OK;
    }

    if (!future_flag) {
        resume(create(consumer, 1024, 20, "consumer", 2, count, main_pid));
        resume(create(producer, 1024, 20, "producer", 1, count));
        suspend(main_pid);
        return OK;
    } else {
        f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
        f_shared = future_alloc(FUTURE_SHARED);
        f_queue = future_alloc(FUTURE_QUEUE);

        if (f_exclusive == SYSERR || f_shared == SYSERR || f_queue == SYSERR) {
            printf("error in futures!!");
            return SYSERR;
        }

        // Test FUTURE_EXCLUSIVE
        resume(create(future_cons, 1024, 20, "fcons1", 1, f_exclusive));
        resume(create(future_prod, 1024, 20, "fprod1", 1, f_exclusive));

        // Test FUTURE_SHARED
        resume(create(future_cons, 1024, 20, "fcons2", 1, f_shared));
        resume(create(future_cons, 1024, 20, "fcons3", 1, f_shared));
        resume(create(future_cons, 1024, 20, "fcons4", 1, f_shared));
        resume(create(future_cons, 1024, 20, "fcons5", 1, f_shared));
        resume(create(future_prod, 1024, 20, "fprod2", 1, f_shared));

        // Test FUTURE_QUEUE
        resume(create(future_cons, 1024, 20, "fcons6", 1, f_queue));
        resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
        resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
        resume(create(future_cons, 1024, 20, "fcons7", 1, f_queue));
        resume(create(future_prod, 1024, 20, "fprod3", 1, f_queue));
        resume(create(future_prod, 1024, 20, "fprod4", 1, f_queue));
        resume(create(future_prod, 1024, 20, "fprod5", 1, f_queue));
        resume(create(future_prod, 1024, 20, "fprod6", 1, f_queue));

        suspend(main_pid);
        kprintf("%d", getprio(main_pid));
        future_free(f_exclusive);
        future_free(f_shared);
        future_free(f_queue);
        return OK;
    }
}