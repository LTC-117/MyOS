#ifndef __PAGING_H__
#define __PAGING_H__


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef enum {
    PAGING_SIZE               =     0b10000000,  /* PS  : If set, the PDE maps to a page that is 4 MiB in size. Otherwise, it maps to a 4 KiB page table. */
    PAGING_DIRTY              =     0b01000000,  /* D   : If set, the page has been written to.*/
    PAGING_ACCESSED           =     0b00100000,  /* A   : If set, a PDE or PTE was read during virtual address translation. */
    PAGING_CACHE_DISABLED     =     0b00010000,  /* PCD : If set, the page will not be cached. If not, it will be. */
    PAGING_WRITE_THROUGH      =     0b00001000,  /* PWT : If set, write-through caching is enabled. If not, write-back is enabled. */
    PAGING_ACCESS_FROM_ALL    =     0b00000100,  /* U/S : If set, all can access the page. If not, supervisor only. */
    PAGING_IS_WRITEABLE       =     0b00000010,  /* R/W : If set, the page is writeable. If not, it's read only. */
    PAGING_IS_PRESENT         =     0b00000001   /* P   : If set, the page is in memory. */
} PagingFlags;


#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_PAGE_SIZE                4096


struct paging_4gb_chunk {
    uint32_t *directory_entry;
};


struct paging_4gb_chunk *paging_new_4gb(uint8_t flags);
uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk);
void paging_switch(uint32_t *directory);
extern void enable_paging(void *page_directory);
bool paging_is_aligned(void *addr);
int paging_get_indexes(void *virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out);
int paging_set(uint32_t *directory, void *virt, uint32_t val);

#endif
