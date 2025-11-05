/* Read and interpret file paths */

#ifndef __PATHPARSER_H__
#define __PATHPARSER_H__


struct path_part {
    const char *part;
    struct path_part *next;
};

struct path_root {
    int drive_no;
    struct path_part *first;
};


/* Example:      0:/test/abc.txt
 *   - 0        -> path root
 *   - test     -> path part no 0
 *   - abc.txt  -> path part no 1
 */

struct path_part *pparser_parse_path_part(struct path_part *last_part, const char **path);
void pparser_free(struct path_root *root);

#endif
