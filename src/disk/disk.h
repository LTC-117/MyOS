#ifndef __DISK_H__
#define __DISK_H__


#include "../fs/file.h"

typedef unsigned int MYOS_DISK_TYPE;

#define MYOS_DISK_TYPE_REAL     0  /* Represents a real physical hard disk */
#define MYOS_DISK_SECTOR_SIZE   512

struct disk {
    MYOS_DISK_TYPE type;
    int sector_size;

    struct filesystem *filesystem;
};


void disk_search_and_init(void);
struct disk *disk_get(int index);
int disk_read_block(struct disk *idisk, unsigned int lba, int total, void *buf);


#endif
