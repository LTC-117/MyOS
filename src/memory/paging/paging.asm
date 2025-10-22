[BITS 32]

section .asm

global paging_load_directory

paging_load_directory:
    push    ebp
    mov     ebp, esp
    mov     eax, [ebp+8]
    mov     cr3, eax
    pop     ebp
    ret
