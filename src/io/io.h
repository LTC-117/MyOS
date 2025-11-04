#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

extern unsigned char insb(unsigned short port);
extern unsigned short insw(unsigned short port);

extern void outb(unsigned short port, unsigned char val);
extern void outw(unsigned short port, unsigned short val);


void println(char *str);
uint16_t terminal_make_char(char c, char color);
void terminal_putchar(int x, int y, char c, char color);
void terminal_write_char(char c, char color);
void terminal_initialize(void);
void print(const char *str);

#endif
