#ifndef __STREAMER_H__
#define __STREAMER_H__


#include "disk.h"


struct disk_stream {
    int pos;
    struct disk *disk;
};


#endif
