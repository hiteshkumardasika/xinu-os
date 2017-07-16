#ifndef FS_H
#define FS_H
/* Modes of file*/
#define O_CREAT 11
/* Flags of file*/
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#define FILENAMELEN 32
#define INODEBLOCKS 12
#define INODEDIRECTBLOCKS (INODEBLOCKS - 2)
#define DIRECTORY_SIZE 16

#define MDEV_BLOCK_SIZE 512
#define MDEV_NUM_BLOCKS 512
#define DEFAULT_NUM_INODES (MDEV_NUM_BLOCKS / 4)

#define INODE_TYPE_FILE 1
#define INODE_TYPE_DIR 2
#define NUM_FD 16
#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2
#define FIRST_INODE_BLOCK 2
#define FIRST_FREE_BLOCK 18

#define NUM_FD 16
extern int dev0;
extern int next_open_fd;

struct inode {
    int id;
    short int flags;
    short int nlink;
    int device;
    int size;
    int blocks[INODEBLOCKS];
};

#define FSTATE_CLOSED 0

#define FSTATE_OPEN 1

struct filetable {
    int state;
    int fileptr;
    struct dirent *de;
    struct inode in;
};

extern struct filetable oft[];

struct dirent {
    int inode_num;
    char name[FILENAMELEN];
};

struct directory {
    int numentries;
    struct dirent entry[DIRECTORY_SIZE];
};

struct fsystem {
    int nblocks;
    int blocksz;
    int ninodes;
    int inodes_used;
    int freemaskbytes;
    char *freemask;
    struct directory root_dir;
};

struct filetable oft[NUM_FD];

/* file and directory functions */
int fs_open(char *filename, int flags);

int fs_close(int fd);

int fs_create(char *filename, int mode);

int fs_seek(int fd, int offset);

int fs_read(int fd, void *buf, int nbytes);

int fs_write(int fd, void *buf, int nbytes);

/* filesystem functions */
int fs_mkfs(int dev, int num_inodes);

int fs_mount(int dev);

/* filesystem internal functions */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in);

int fs_put_inode_by_num(int dev, int inode_number, struct inode *in);

int fs_setmaskbit(int b);

int fs_clearmaskbit(int b);

int fs_getmaskbit(int b);

/*
  Block Store functions
  bread, bwrite,
  bput, bget write entire blocks (macro with offset=0, len=blocksize)
 */
int bs_mkdev(int dev, int blocksize, int numblocks);

int bs_bread(int bsdev, int block, int offset, void *buf, int len);

int bs_bwrite(int bsdev, int block, int offset, void *buf, int len);

/* debugging functions */
void fs_printfreemask(void);

void fs_print_fsd(void);

#endif
