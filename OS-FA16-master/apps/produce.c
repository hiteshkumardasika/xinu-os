#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
    for (int i = 0; i < count; ++i) {
        wait(produced);
        n++;
        printf("produced is %d\n", n);
        signal(consumed);
    }
}



