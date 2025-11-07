#ifndef __LIST_H__
#define __LIST_H__


struct list_head {
    struct list_head *prev, *next;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


#endif
