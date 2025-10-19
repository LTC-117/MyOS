#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>


struct idt_desc {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_2;
} __attribute__((__packed__));


struct idtr_desc {
    uint16_t limit;
    uint32_t base;
} __attribute__((__packed__));

void idt_init();
void enable_interrupts();
void disable_interrupts();

#endif
