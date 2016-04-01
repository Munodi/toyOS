%ifdef x64

global loader           ; making entry point visible to linker
extern kmain            ; kmain is defined elsewhere
extern kernel_end

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

; reserve initial kernel stack space
STACKSIZE equ 0x4000                    ; that's 16k.

%define KERNEL_LMA 0x0000000000100000
%define KERNEL_VMA 0xFFFFFFFF80100000

[BITS 32]

SECTION .text
align 8
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    
loader: 
    mov edx, 0x2badb002
    cmp eax, edx
    je correct_multiboot

    ; this runs only if eax does not have the magic info number
    mov esi, no_magic_str ;incorrect
    mov edi, 0xB8000
    cld
    mov ecx, 14
    rep movsb
    hlt
   
    correct_multiboot:
   
    ; copy multiboot structures to kernel data areas so they will be preserved
    ; copy MBI
    mov esi, ebx
    mov edi, mbi - VIRTUAL_OFFSET
    mov ecx, 90
    cld
    rep movsb
   
    ; copy mmap
    mov edi, mmap - VIRTUAL_OFFSET           ; move address of mmap to edi
    mov esi, [mbi - VIRTUAL_OFFSET + 48]     ; move address of old mmap to esi
    mov ecx, [mbi - VIRTUAL_OFFSET + 44]		; move length of mmap to ecx, note should add check
    cld
    rep movsb
    mov dword [mbi - VIRTUAL_OFFSET + 48], mmap   ; replace old address with new
   
    mov ebx, mbi	; move new mbi to ebx so it is passed to kmain by the upcoming push ebx
    
    xchg bx, bx
	
	; TODO add more here



SECTION .bss
mbi:   resb 90
mmap:  resb 768 ; room for 32 entries
alignb 32
stack:  resb STACKSIZE                       ; reserve 16k stack on a doubleword boundary


%endif ; x64