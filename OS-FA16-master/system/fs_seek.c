#include <xinu.h>

#if FS
#include <fs.h>

int fs_seek(int fd, int offset) {
    oft[fd].fileptr+=offset;
    return fd;
}
#endif /* FS */


