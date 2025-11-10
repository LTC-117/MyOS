#ifndef __LIST_H__
#define __LIST_H__


#include "container_of.h"


struct list_head {
    struct list_head *prev, *next;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


static inline void __list_add(struct list_head *new_item,
                                  struct list_head *previous,
                                  struct list_head *next)
{
    next->prev = new_item;
    new_item->prev = previous;
    new_item->next = next;
    previous->next = new_item;
}


static inline void list_add(struct list_head *new_item,
                            struct list_head *head)
{
    __list_add(new_item, head, head->next);
}


static inline void __list_delete(struct list_head *previous,
                               struct list_head *next)
{
    next->prev = previous;
    previous->next = next;
}


static inline void list_delete(struct list_head *entry)
{
    __list_delete(entry->prev, entry->next);
}


static inline int list_empty(struct list_head *head)
{
    return (head->next == head);
}


static inline void list_splice(struct list_head *list,
                               struct list_head *head)
{
    struct list_head *first = list->next;

    // Which means : if list exists!
    if (first != list) {
        struct list_head *last = list->prev;
        struct list_head *now = head->next;

        first->prev = head;
        head->next = first;

        last->next = now;
        now->prev = last;
    }
}


#define list_entry(ptr, type, member)       \
    ( (type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)) )


#endif
