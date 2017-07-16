#include <xinu.h>
#include <string.h>

#if FS
#include <fs.h>

int fs_open(char *filename, int flags) {
    struct fsystem fsd;
    struct inode in;
    int flag=0;
    int fd = next_open_fd;
    bs_bread(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
    for (int i = 0; i < fsd.root_dir.numentries; i++) {
        if (strcmp(fsd.root_dir.entry[i].name, filename) == 0) {
            oft[fd].state = FSTATE_OPEN;
            oft[fd].fileptr = 0;
            oft[fd].de = &fsd.root_dir.entry[i];
            fs_get_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &in);
            oft[fd].in = in;
            in.flags = flags;
            fs_put_inode_by_num(0, fsd.root_dir.entry[i].inode_num, &in);
            flag = 1;
        }
    }
    if(!flag){
        printf("No file exists with this filename\n");
        return SYSERR;
    }
    next_open_fd++;
    return fd;
}

#endif /* FS */


