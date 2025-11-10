#ifndef __LIST_H__
#define __LIST_H__


#include "container_of.h"


struct list_head {
    struct list_head *prev, *next;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


static __inline__ void __list_add(struct list_head *new_item,
                                struct list_head *previous,
                                struct list_head *next)
{
    next->prev = new_item;
    new_item->prev = previous;
    new_item->next = next;
    previous->next = new_item;
}


#endif
