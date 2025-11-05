#include "pparser.h"

#include "../config.h"
#include "../status.h"
#include "../memory/memory.h"
#include "../kstring/kstring.h"


static int path_valid_format(const char *file_name)
{
    int len = kstrlen(file_name, MYOS_MAX_PATH);

    if (len > MYOS_MAX_PATH) {
        return -EFSSTR;
    }

    return (len >= 3) && (kisdigit(file_name[0])) && (memcmp((void *)&file_name[1], ":/", 2) == 0);
}
