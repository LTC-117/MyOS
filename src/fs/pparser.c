#include "pparser.h"

#include "../config.h"
#include "../status.h"
#include "../memory/memory.h"
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
