ORG     0x7c00                   ; Segmento inicial. Ou seja, todo segmento tem offset a partir daqui.

BITS    16


CODE_SEG equ gdt_code - gdt_start   ; Nos dá os offsets da seção gdt
DATA_SEG equ gdt_data - gdt_start

; ------------------------------------------------------------------------------
; ---------------------------- BIOS Parameter Block ----------------------------
; ------------------------------------------------------------------------------

jmp short start
nop

; FAT16 Header
OEMIdentifier       db 'MYOS OEM'
BytesPerSector      dw 0x200
SectorsPerCluster   db 0x80
ReservedSectors	    dw 200		    ; Reserved sectors before FAT
FATCopies		    db 0x02		    ; Often this value is 2
RootDirEntries	    dw 0x40		    ; 64 Root directory entries
NumSectors	        dw 0x0000		; If 0, there are more than 65535 sectors
MediaType		    db 0xF8		    ; Fixed disk → Harkdisk
SectorsPerFAT	    dw 0x0100		; Sectors used by each FAT table
SectorsPerTrack	    dw 0x20		    ; TODO: Look up? BIOS might change those
NumberOfHeads	    dw 0x40	        ; Does this even matter?
HiddenSectors	    dd 0x00
SectorsBig	        dd 0x773594

; Extended BPB (DOS 4.0)
DriveNumber	        db 0x80		    ; 0 for removable, 0x80 for HD
WinNTBit		    db 0x00		    ; WinNT uses this
Signature		    db 0x29		    ; dos 4.0 EBPB signature
VolumeID		    dd 0x0000D105	; Volume ID
VolumeIDString	    db 'MYOS BOOT  '; Volume ID
SystemIDString	    db 'FAT16   '	; File system type

; ------------------------------------------------------------------------------
; ------------------- Label de início da execução do código --------------------
; ------------------------------------------------------------------------------

start:
    jmp 0:step2 ; Estabelece offset 0x7c0 pra start


; --------------------------- Reading into memory ------------------------------

 ; AH = 02h
 ; AL = number of sectors to read (must be nonzero)
 ; CH = low eight bits of cylinder number
 ; CL = sector number 1-63 (bits 0-5)
 ; high two bits of cylinder (bits 6-7, hard disk only)
 ; DH = head number
 ; DL = drive number (bit 7 set for hard disk)
 ; ES:BX -> data buffer

; ------------------------------------------------------------------------------

step2:
    cli ; clear interrupts
    mov         ax, 0x00   ; Atribui ao 'ax' o valor do offset de 'start'
    mov         ds, ax      ; Usa 'ax' pra armazenar o offset nos segmentos DS e ES.
    mov         es, ax
    mov         ss, ax      ; Endereça o valor ao stack de fato.
    mov         sp, 0x7c00  ; Coloca o stack pointer para apontar para a região 0x7c00,
                            ; onde o bootloader deve ser colocado de fato
    sti ; enables interrupts

.load_protected:
    cli
    lgdt [gdt_descriptor]
    mov     eax, cr0
    or      eax, 0x1
    mov     cr0, eax
    jmp     CODE_SEG:load32 ; switches to code selector and jumps to 'load32'

; ------------------------------------------------------------------------------
; ---------------------------------- GDT ---------------------------------------
; ------------------------------------------------------------------------------

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:       ; CS deve apontar para isso
    dw 0xffff   ; Limite do segmento (primeiros 16 bits: 0 a 15)
    dw 0        ; Base (primeiros 16 bits)
    db 0        ; Base (bits 16 a 23)
                ;               P  S E R
    db 0x9a     ; Access Byte = 1001 1010
    db 11001111b; High 4 bit flags and low 4 bit flags
    db 0        ; Base 24-31 bits

; offset 0x10
gdt_data:       ; DS, SS, ES, FS, GS
    dw 0xffff   ; Limite do segmento (primeiros 16 bits: 0 a 15)
    dw 0        ; Base (primeiros 16 bits)
    db 0        ; Base (bits 16 a 23)
                ;               P  S E R
    db 0x92     ; Access Byte = 1001 1010
    db 11001111b; High 4 bit flags and low 4 bit flags
    db 0        ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start


[BITS 32]

load32:
    mov     eax, 1          ; representa o setor inicial de onde queremos carregar o kernel (0 é o bootloader, então usamos 1)
    mov     ecx, 100        ; representa a quantidade de setores que queremos carregar
    mov     edi, 0x0100000  ; contém o endereço onde queremos carregar o kernel.
    call    ata_lba_read
    jmp     CODE_SEG:0x0100000

ata_lba_read:
    mov     ebx, eax ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr     eax, 24     ; 32 - 24 = 8 -> shift eax register 24 to the right to get the highest 8 bits of the lba
    or      eax, 0xe0   ; Select the master drive
    mov     dx, 0x1f6
    out     dx, al      ; al is 8 bits and contains the highest 8 bits of the lba
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov     eax, ecx
    mov     dx, 0x1f2
    out     dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov     eax, ebx    ; Restore the backup LBA
    mov     dx, 0x1f3
    out     dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov     dx, 0x1f4
    mov     eax, ebx ; Restore the backup LBA
    shr     eax, 8
    out     dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov     dx, 0x1f5
    mov     eax, ebx ; Restore the backup LBA
    shr     eax, 16
    out     dx, al
    ; Finished sending upper 16 bits of the LBA

    mov     dx, 0x1f7
    mov     al, 0x20
    out     dx, al

    ; Read all sectors into memory
.next_sector:
    push    ecx

; Check if we need to read
.try_again:
    mov     dx, 0x1f7
    in      al, dx  ; Read from port 0x1f7 into the al register
    test    al, 8   ; Test the al register to see if a bit was set in this bit mask (we look for 8)
    jz      .try_again

; We need to read 256 words at a time
    mov     ecx, 256
    mov     dx, 0x1f0
    ; Reads a word from the I/O port specified in 'dx' into the memory location specified in es:(e)di (0x0100000)
    rep     insw    ; repeats the insw instruction 256 times.
    pop     ecx     ; restores the ecx we saved before
    loop    .next_sector
    ; End of reading sectors into memory
    ret


; ------------------------------------------------------------------------------
; -------------------------- Declaração das variáveis --------------------------
; ------------------------------------------------------------------------------

; Atalho de repetição no NASM. Ele preenche memória com um valor (db 0 → 0x00) várias vezes.
;   • ($ - $$) calcula quantos bytes já foram escritos desde o início do segmento ($$ é o início do ORG).
;   • 510-($-$$) significa: “preencher com zeros até atingir 510 bytes totais”.
times 510-($ - $$) db 0

; Por que 510?
;   • Um setor de boot tem 512 bytes.
;   • Os últimos 2 bytes são reservados para a assinatura do boot (0xAA55).
; Ou seja, a word (2 bytes) '0xaa55' é a assinatura obrigatória do bootloader
; que a BIOS procura no final do setor de 512 bytes.
; Sem ela, a BIOS não vai tentar dar boot nesse setor.
dw 0xAA55

buffer: ; apenas para referência, não deve ser usado pra armazenar variáveis.

; Com o que foi feito nessa aula, consegui compreender como utilizar interrupts
; da BIOS pra realizar leitura em disco. Acredito que dessa forma poderei
; futuramente utilizar dessa mesma técnica pra realizar verificações de endereços
; de memória, procurando pelo executável do sistema operacional, por exemplo.
; 
; PS !!!!!
; Estudar com mais profundidade sobre o que está acontecendo aqui, e tentar
; novamente sintetizar o que é o assembly e como ele funciona. Terminar de ler o
; LLP.
