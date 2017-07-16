/*Global variable for producer consumer*/
extern int n; /*this is just declaration*/

/*function Prototype*/
void consumer(int count,pid32 main_pid);
void producer(int count);

/* Semaphore Declarations */
extern sid32 produced, consumed;
extern pid32 main_pid;
