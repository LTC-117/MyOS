#include "pparser.h"

#include "../config.h"
#include "../status.h"
#include "../memory/memory.h"
#include "../memory/heap/kheap.h"
#include "../kstring/kstring.h"


static int pparser_path_valid_format(const char *file_name)
{
    int len = kstrlen(file_name, MYOS_MAX_PATH);

    if (len > MYOS_MAX_PATH) {
        return -EBADPATH;
    }

    return (len >= 3) && (kisdigit(file_name[0])) && (memcmp((void *)&file_name[1], ":/", 2) == 0);
}


static int pparser_get_drive_by_path(const char **path)
{
    if (!pparser_path_valid_format(*path)) {
        return -EBADPATH;
    }

    int drive_no = kctoi(*path[0]);

    // Add 3 bytes to skip drive number 0:/ 1:/ 2:/
    *path += 3;

    return drive_no;
}


static struct path_root *pparser_create_root(int drive_no)
{
    struct path_root *path_r = kzalloc(sizeof(struct path_root));

    path_r->drive_no = drive_no;
    path_r->first = 0;

    return path_r;
}
