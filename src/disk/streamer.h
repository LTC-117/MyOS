#ifndef __STREAMER_H__
#define __STREAMER_H__


#include "disk.h"


struct disk_stream {
    int pos;
    struct disk *disk;
};


struct disk_stream *dstreamer_new(int disk_id);
int dstreamer_seek(struct disk_stream *stream, int pos);
int dstreamer_read(struct disk_stream *stream, void *out, int total);
void dstreamer_close(struct disk_stream *stream);


#endif
