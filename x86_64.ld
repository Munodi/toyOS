ENTRY(loader)
OUTPUT_FORMAT(elf64-x86-64)
KERNEL_VMA = 0xFFFFFFFF80100000;
KERNEL_LMA = 0x0000000000100000;

SECTIONS
{
    . = KERNEL_VMA;

    .text ALIGN (0x1000) : AT(ADDR(.text) - KERNEL_VMA + KERNEL_LMA)
	{
        *(.text*)
        *(.gnu.linkonce.t*)
    }
    
    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - KERNEL_VMA + KERNEL_LMA)
	{
        start_ctors = .;
        *(.ctor*)
        end_ctors = .;
        
        start_dtors = .;
        *(.dtor*)
        end_dtors = .;
        
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - KERNEL_VMA + KERNEL_LMA)
	{
        *(.data*)
        *(.gnu.linkonce.d*)
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - KERNEL_VMA + KERNEL_LMA)
	{
        _sbss = .;
        *(.bss*)
        *(.COMMON*)
	    *(.gnu.linkonce.b*)
        _ebss = .;
    }
   
    /DISCARD/ :
    {
        *(.comment)
		*(.eh_frame) /* You should discard this unless you're implementing runtime support for C++ exceptions. */
	}

    kernel_end = .;
}
