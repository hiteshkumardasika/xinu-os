#include <xinu.h>
#include <string.h>


#if FS
#include <fs.h>

int fs_read(int fd, void *buf, int nbytes) {
    int inode_to_read = oft[fd].de->inode_num, i;
    int fileptr = oft[fd].fileptr;
    struct inode in;
    //Reading the Super filesys block
    fs_get_inode_by_num(0, inode_to_read, &in);
    if (nbytes > (in.size - fileptr)) {
        nbytes = in.size - fileptr;
    }
    int blocks = (in.size % MDEV_BLOCK_SIZE == 0) ? in.size / MDEV_BLOCK_SIZE : (in.size / MDEV_BLOCK_SIZE) + 1;
    char data[blocks * MDEV_BLOCK_SIZE];
    //number of blocks that have data
    bs_bread(0, in.blocks[0], 0, data, blocks * MDEV_BLOCK_SIZE);
    memcpy(buf, data + fileptr, nbytes - fileptr);
    oft[fd].fileptr = nbytes;
    fs_put_inode_by_num(0, inode_to_read, &in);
    return nbytes;
}

#endif /* FS */


