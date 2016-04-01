#include <toyOS/multiboot.h>
#include <toyOS/physicalMemoryManager.hpp>
#include <toyOS/virtualMemoryManager.hpp>
#include <toyOS/kernelHeap.hpp>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <toyOS/memoryLayout.hpp>
#include <toyOS/bootTerminal.hpp>
#include <toyOS/arch/x86/gdt.hpp>
#include <toyOS/interruptManager.hpp>
#include <toyOS/arch/x86/controlRegisters.hpp>
#include "c++support.hpp"
#include <toyOS/arch/x86/ACPI.hpp>

void callConstructors();
void print_multiboot_info(multiboot_info_t *mbi);

extern "C" unsigned kernel_end;
extern void initIdt();
extern "C" void kmain(unsigned int magic, multiboot_info_t *mbi)
{
    initialiseConstructors();
    /* paging is enabled before kmain is entered */
    bootTerminal::init();
    bootTerminal::prints("kernel default console inited\n");
    if(magic != 0x2BADB002)
    {
        bootTerminal::prints("panic: magic not 0x2BADB002\n");
        while(1){;}
    }

	bootTerminal::prints("kernel length is ");
	bootTerminal::print10((unsigned)(&kernel_end) - 0xC0000000);
	bootTerminal::printc('\n');

    print_multiboot_info(mbi);

    physicalMemoryManager::init(mbi);
    bootTerminal::prints("Physical memory manager inited\n");

    virtualMemoryManager::init();
    bootTerminal::prints("Virtual memory manager inited\n");

    kernelHeap::init();


    #ifdef x86
        gdt::init();
        bootTerminal::prints("GDT inited\n");
    #endif

    physicalMemoryManager::debug();

    findRSDP();
	initIdt();
	
    //interruptManager::init();

    bootTerminal::prints("leaving kmain\n");
	for(;;)
    {
    	asm volatile("hlt");
    }
}

void print_multiboot_info(multiboot_info_t *mbi)
{
    char tmp[20];
    multiboot_mmap_entry* mmap;
    bootTerminal::prints("\nmultiboot info:\n");

	if(mbi->flags & (1 << 0))
	{
		bootTerminal::prints("mem_lower: ");
		bootTerminal::prints(std::utoa(mbi->mem_lower, &(tmp[0]), 10));
		bootTerminal::prints("\n");
		bootTerminal::prints("mem_upper: ");
		bootTerminal::prints(std::utoa(mbi->mem_upper, &(tmp[0]), 10));
		bootTerminal::printc('\n');
	}

	if(mbi->flags & (1 << 1))
	{
		bootTerminal::prints("boot_device: 0x");
		bootTerminal::printh(mbi->boot_device);
		bootTerminal::printc('\n');
	}

    multiboot_uint32_t numberOfMemoryMapEntries = ((mbi->mmap_length) / sizeof(multiboot_memory_map_t));

    mmap = (multiboot_mmap_entry*)(mbi->mmap_addr);

    for(multiboot_uint32_t i = 0; i < numberOfMemoryMapEntries; i++)
    {
        bootTerminal::prints("size: ");
        bootTerminal::prints(std::utoa(mmap[i].size, &(tmp[0]), 10));
        bootTerminal::prints(" type: ");
        bootTerminal::prints(std::utoa(mmap[i].type, &(tmp[0]), 10));
        bootTerminal::prints(" start: ");
        bootTerminal::prints(std::utoa(mmap[i].addr, &(tmp[0]), 10));
        bootTerminal::prints(" length: ");
        bootTerminal::prints(std::utoa(mmap[i].len, &(tmp[0]), 10));
        bootTerminal::printc('\n');
    }
	
	bootTerminal::printc('\n');
    return;

}
/*
std::uintptr_t start_ctors;
std::uintptr_t end_ctors;
void *__dso_handle;

void callConstructors()
{
  // Constructor list is defined in the linker script.
  // The .ctors section is just an array of function pointers.
  // iterate through, calling each in turn.
  std::uintptr_t *iterator = reinterpret_cast<std::uintptr_t*>(&start_ctors);
  while (iterator < reinterpret_cast<std::uintptr_t*>(&end_ctors))
  {
    void (*fp)(void) = reinterpret_cast<void (*)(void)>(*iterator);
    fp();
    iterator++;
  }
}
*/
