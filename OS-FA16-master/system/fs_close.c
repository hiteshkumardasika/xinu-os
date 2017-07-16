#include <xinu.h>
#include <string.h>

#if FS
#include <fs.h>

int fs_close(int fd) {

    oft[fd].state = FSTATE_CLOSED;
    oft[fd].fileptr = 0;
    next_open_fd--;
    return OK;
}

#endif /* FS */


