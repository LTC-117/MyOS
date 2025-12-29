#ifndef __MYOS_CONTAINER_OF_H__
#define __MYOS_CONTAINER_OF_H__

#include <stddef.h>
//#include <assert.h>


#define typeof_member(T, m)     typeof(((T *)0)->m)

#define container_of(ptr, type, member) ({                          \
        void *_member_ptr = (void *)(ptr);                          \
        static_assert(__same_type(*(ptr), ((type *)0)->member) ||   \
                      __same_type(*(ptr), void),                    \
                      "Pointer type mismatch in container_of()");   \
        ((type *)(_member_ptr - offsetof(type, member))); })

/* 
 * @brief
 * This implements the container_of_const() the same way the linux kernel does.
 * Prefer this one instead of the above
 */
#define containter_of_const(ptr, type, member)                      \
    _Generic(ptr,                                                   \
            const typeof(*(ptr)) *: ((const type *)container_of(ptr, type, member)),\
            default: ((type *)container_of(ptr, type, member))      \
    )

#endif
