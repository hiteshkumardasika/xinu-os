#include <xinu.h>
#include <string.h>

#if FS
#include <fs.h>


int fs_create(char *filename, int mode) {
    int flag = 0, i, current_inode_used, current_file_cntr;
    struct inode in;
    struct fsystem fsd;
    struct dirent de;
    //Reading the Super filesys block
    bs_bread(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));

    //error handling
    if (mode == O_CREAT) {
        for (i = 0; i < fsd.root_dir.numentries; i++) {
            if (strcmp(fsd.root_dir.entry[i].name, filename) == 0) {
                printf("File creation failed, file name already exists!\n");
                return SYSERR;
            }
        }
    }
    if (strlen(filename) > FILENAMELEN) {
        printf("Filename length should not exceed %d characters\n", FILENAMELEN);
        return SYSERR;
    }

    //Modifying the filesys entries and book keeping some variables
    current_inode_used = fsd.inodes_used;
    fsd.inodes_used = current_inode_used + 1;
    current_file_cntr = fsd.root_dir.numentries;
    fsd.root_dir.numentries = fsd.root_dir.numentries + 1;

    de.inode_num = current_inode_used;
    strcpy(de.name, filename);
    fsd.root_dir.entry[current_file_cntr] = de;

    //Initializing inode with values
    in.id = current_inode_used;
    in.flags = O_RDWR;
    in.nlink = 1;
    in.device = dev0;
    in.size = 0;

    //Initialising the filetable entry i.e. getting a file descriptor
    oft[current_file_cntr].state = FSTATE_OPEN;
    oft[current_file_cntr].fileptr = 0;
    oft[current_file_cntr].de = &de;
    oft[current_file_cntr].in = in;
    next_open_fd++;
    bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
    fs_put_inode_by_num(dev0, current_inode_used, &in);
    printf("Returning the fd as %d and inode number setting as %d\n", current_file_cntr, de.inode_num);
    return current_file_cntr;
}

#endif /* FS */



