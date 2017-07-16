#include <xinu.h>
#include <string.h>


#if FS
#include <fs.h>

int fs_write(int fd, void *buf, int nbytes) {
    struct fsystem fsd;
    char data[MDEV_BLOCK_SIZE];
    int block_number = 0;
    int cntr = 0, i;
    int size = 0;
    int block_to_write;
    int inode_num = oft[fd].de->inode_num;
    printf("writing into inode number %d\n",inode_num);
    struct inode in;

    if(fd > next_open_fd){
        printf("Invalid File Descriptor\n");
        return SYSERR;
    }
    //Reading the Super filesys block
    bs_bread(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
    fs_get_inode_by_num(0, inode_num, &in);
    block_to_write = FIRST_FREE_BLOCK + ((inode_num) * INODEBLOCKS);
    i = nbytes;
    while (i >= MDEV_BLOCK_SIZE) {
        memcpy(data,buf + size, MDEV_BLOCK_SIZE);
        block_number = block_to_write + cntr;
        fs_setmaskbit(block_number);
        bs_bwrite(0, block_number, 0, data, MDEV_BLOCK_SIZE);
        size += MDEV_BLOCK_SIZE;
        in.blocks[cntr] = block_number;
        cntr++;
        i -= MDEV_BLOCK_SIZE;
    }
    memcpy(data,buf + size, MDEV_BLOCK_SIZE);
    block_number = block_to_write + cntr;
    fs_setmaskbit(block_number);
    bs_bwrite(0, block_number, 0, data, MDEV_BLOCK_SIZE);
    in.blocks[cntr] = block_number;
    oft[fd].fileptr = nbytes;
    in.size = nbytes;
    fs_put_inode_by_num(0, inode_num, &in);
    bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
    return nbytes;
}

#endif /* FS */



