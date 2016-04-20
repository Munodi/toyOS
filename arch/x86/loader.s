
global loader           ; making entry point visible to linker
extern kmain            ; kmain is defined elsewhere
extern kernel_end


; setting up Multiboot 2 header
MAGIC2		equ		0xe85250d6			; Multiboot2 header magic number
ARCHITECTURE	equ 0					; i386 protected mode = 0
HEADER_LENGTH	equ MultiBoot2HeaderEnd - MultiBoot2Header
CHECKSUM2	equ		0x100000000 - (MAGIC2 + ARCHITECTURE + HEADER_LENGTH)
EAX_MAGIC2	equ		0x36d76289			; Multiboot2 EAX state

; reserve initial kernel stack space
STACKSIZE equ 0x4000                    ; that's 16k.

%define KERNEL_LMA 0x00100000
%define KERNEL_VMA 0xC0100000
%define VIRTUAL_OFFSET 0xC0000000
 
SECTION .text
align 8
MultiBoot2Header:
	dd MAGIC2
	dd ARCHITECTURE
	dd HEADER_LENGTH
	dd CHECKSUM2
	
	; add header tags here as needed
	
	dw 0
	dw 0
	dd 8
	
MultiBoot2HeaderEnd:



loader:
    
    cmp eax, EAX_MAGIC2
    je correct_multiboot2
    
    xchg bx, bx
   
   ; this runs only if eax does not have the magic info number
    mov esi, no_magic_str - VIRTUAL_OFFSET ;incorrect
    mov edi, 0xB8000
    cld
    mov ecx, 14
    rep movsb
    hlt
   
	correct_multiboot2:
    xchg bx, bx
   
    mov esp, stack+STACKSIZE-VIRTUAL_OFFSET     ; set up the stack at
                                                ; a low address
    push ebx
    push eax
   
    ;XCHG BX, BX                          ; bochs breakpoint
   
    ; setup boot paging to map kernel to higher half
    call init_boot_paging_ia32
   
    ; start fetching instructions in kernel space
    lea ecx, [StartInHigherHalf]
    jmp ecx
   
StartInHigherHalf:
   ; unmap the identity-mapped first 8 Mb of physical address space.
   ; It should not be needed anymore.
   mov dword [_kernel_pd], 0
   mov dword [_kernel_pd+4], 0
   mov ecx, _kernel_pd - VIRTUAL_OFFSET
   mov cr3, ecx
   
   ;xchg bx, bx
   
   add esp, VIRTUAL_OFFSET
   call kmain                          ; call kernel proper
   
hang:
   cli
   ;xchg bx, bx
   hlt                                  ; halt machine should kernel return
   jmp   hang

init_boot_paging_ia32:
   
   push ebx     ; save ebx
   ;setup the PD and PTs
   
   ; as the page tables are allocated in the bss section they default
   ; to zero, I will change the entries that hold the 1st mb + the kernel
   
   xor bx, bx
   
   mov eax, kernel_end       ; eax now holds the size
   sub eax, VIRTUAL_OFFSET   ; of the kernel + 1st mb in memory
   shr eax, 12
   ;Commented out 2016, I've aligned kernel_end in linker script to shouldn't need this.inc eax                      ; eax now holds the number of pages needed to store memory from 0 to the end of the kernel
   mov ecx, eax                 ; ecx now holds the number of pages needed to store memory from 0 to the end of the kernel
   mov ebx, _kernel_page_tables - VIRTUAL_OFFSET        ; move the address of the start of the page tables to ebx
   
   ; eax = page table entry to save at address
   ; ebx = page tables pointer(incrementing)
   ; ecx = number of pages(constant)
   ; edx = 
   ; in each loop 4096 will be added to eax
   
   ; initialise eax with pte for to map first address
   mov eax, 3   ; read/write and present
   
   ; loop through adding 4096 to each succesive page table entry, this
   ; works because all the page tables have been preallocated
   .l1:
      mov dword [ebx], eax
      add ebx, 4
      add eax, 4096
   loop .l1
   
   ; set the first few pde's to point to the kernel page table 1 and 2
   mov eax, _kernel_page_tables - VIRTUAL_OFFSET
   and eax, 0xFFFFF000
   or eax, 0x3
   mov dword [_kernel_pd - VIRTUAL_OFFSET], eax
   mov eax,  _kernel_page_tables - VIRTUAL_OFFSET + 4096
   and eax, 0xFFFFF000
   or eax, 0x3
   mov dword [_kernel_pd - VIRTUAL_OFFSET + 4], eax
   
   ; map the higher half of the page directory entries to my tables
   mov ecx, 255         ; number of page tables for kernel space
   mov eax, _kernel_pd - VIRTUAL_OFFSET + 3072	; start of kernel pde's
   mov ebx, _kernel_page_tables - VIRTUAL_OFFSET
   
   ;ecx=counter
   ;eax=pointer to pdes
   ;ebx=pointer to page tables
   ;page table size is 4096 bytes, therefore to get next one, add 4096 to ebx
   
   .l2:
      mov edx, ebx
      and edx, 0xFFFFF000
      or edx, 0x3
      mov dword [eax], edx
      add eax, 4
      add ebx, 4096
   loop .l2
   ; recursively map page directory
   ;xchg bx, bx
   mov eax, _kernel_pd - VIRTUAL_OFFSET
   and eax, 0xFFFFF000
   or eax, 0x3
   mov dword [_kernel_pd - VIRTUAL_OFFSET + 1023 * 4], eax
   
   ; enable paging
   mov eax, _kernel_pd - VIRTUAL_OFFSET
   mov cr3, eax
   
   mov ecx, cr0
   or ecx, 0x80000000
   mov cr0, ecx
   
   ;halt for debugging
   xchg bx, bx
   
   pop ebx      ; restore ebx
   
ret
   
   
SECTION .rodata
no_magic_str db 'e', 0x0F, 'r', 0x0F, 'r', 0x0F, 'o', 0x0F, 'r', 0x0F, ':', 0x0F, '1', 0x0F, 0x0

 
SECTION .bss
alignb 32
stack:  resb STACKSIZE                       ; reserve 16k stack on a doubleword boundary
   
alignb 4096
_kernel_pd:            resb 0x1000
_kernel_page_tables:   resb 0x100000 ; I am creating 256 page tables here
