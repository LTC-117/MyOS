section .asm

extern int21h_handler
extern no_interrupt_handler

global idt_load
idt_load:
    push    ebp
    mov     ebp, esp

    mov     ebx, [ebp+8]
    lidt    [ebx]

    pop     ebp
    ret

global int21h
int21h:
    cli
    pushad

    call    int21h_handler

    popad
    sti
    iret

global no_interrupt
no_interrupt:
    cli
    pushad

    call    no_interrupt_handler

    popad
    sti
    iret

