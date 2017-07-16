#include <xinu.h>
#include <prodcons.h>
#include <stdio.h>
#include "../include/prodcons.h"


void consumer(int count,pid32 main_pid) {
    int i;
    for (i = 0; i < count; ++i) {
        wait(consumed);
        printf("Consumed value is %d\n", n);
        signal(produced);
    }
    resume(main_pid);
}
