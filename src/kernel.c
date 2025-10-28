#include "kernel.h"

#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"

#include <stddef.h>
#include <stdint.h>


uint16_t *video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;


void println(char *str)
{
    while(*str != '\0') {
        *video_mem++ = *str++;
        *video_mem++ = 0xff;
    }
}


uint16_t terminal_make_char(char c, char color)
{
    return (color << 8) | c;
}


void terminal_putchar(int x, int y, char c, char color)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}


void terminal_write_char(char c, char color)
{
    if(c == '\n') {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, color);
    terminal_col++;

    if(terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }
}


void terminal_initialize(void)
{
    video_mem = (uint16_t *)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;

    for(int y = 0; y < VGA_HEIGHT; y++) {
        for(int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}


void print(const char *str)
{
    for(int i = 0; str[i] != '\0'; i++) {
        terminal_write_char(str[i], 15);
    }
}


static struct paging_4gb_chunk *kernel_chunk;

void kernel_main(void)
{
    terminal_initialize();
    print("Hello, world!\ntest");

    kheap_init();

    // initialize interrupt descriptor table
    idt_init();

    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    enable_paging(kernel_chunk);

    // Enable system interrupts
    enable_interrupts();
}
