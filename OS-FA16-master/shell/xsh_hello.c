/* xsh_hello.c - hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - utility which greets you with hello
 *------------------------------------------------------------------------
 */

shellcmd xsh_hello(int nargs, char *args[]) {

    if (nargs == 2 && strncmp(args[1], "--help", 5) == 0) {
        printf("Usage: %s\n", args[0]);
        printf("Description:\n");
        printf("Utility which greets you with hello\n");
        return OK;
    }

    if (nargs >2){
        printf("Sorry!!Enter only one argument.\nFor Usage --help\n");
        return OK;
    }
    printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
    return OK;
}
