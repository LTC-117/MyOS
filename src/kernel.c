#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "disk/streamer.h"
#include "kmath/kmath.h"
#include "kstring/kstring.h"
#include "fs/pparser.h"
#include "fs/file.h"
//#include "myos/container_of.h"
#include "myos/list.h"

#include <stddef.h>
#include <stdint.h>

#include "kernel.h"


static struct paging_4gb_chunk *kernel_chunk;

void kernel_main(void)
{
    terminal_initialize();
    print("Hello, world!\ntest");

    kheap_init();

    disk_search_and_init();

    // initialize interrupt descriptor table
    idt_init();

    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    // Habilita o paging
    enable_paging(kernel_chunk);

    // Enable system interrupts
    enable_interrupts();

    struct disk_stream *stream = dstreamer_new(0);
    dstreamer_seek(stream, 0x201);
    unsigned char c = 0;
    dstreamer_read(stream, &c, 1);
    while (1) {}
}
