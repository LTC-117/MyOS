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


static const char *pparser_get_path_part(const char **path)
{
    char *result_path_part = kzalloc(MYOS_MAX_PATH);
    int i = 0;

    while (**path != '/' && **path != 0) {
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    if (**path == '/') {
        // Skip the forward slash to avoid problems
        *path += 1;
    }

    if (i == 0) {
        kfree(result_path_part);
        result_path_part = 0;
    }

    return result_path_part;
}


struct path_part *pparser_parse_path_part(struct path_part *last_part, const char **path)
{
    const char *path_part_str = pparser_get_path_part(path);

    if (!path_part_str) {
        return 0;
    }

    struct path_part *part = kzalloc(sizeof(struct path_part));

    part->part = path_part_str;
    part->next = 0x00;

    if (last_part) {
        last_part->next = part;
    }

    return part;
}


void pparser_free(struct path_root *root)
{
    struct path_part *part = root->first;

    while (part) {
        struct path_part *next_part = part->next;
        kfree((void *)part->part);
        kfree(part);
        part = next_part;
    }

    kfree(root);
}
