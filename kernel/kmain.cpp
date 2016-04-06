#include <toyOS/multiboot.h>
#include <toyOS/multiboot2.hpp>
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
#include <toyOS/arch/x86/asm.hpp>

void callConstructors();
void print_multiboot_info(multiboot_info_t *mbi);
void print_multiboot2_info(Multiboot2BootInfo *mbi2);

extern "C" unsigned kernel_end;
extern void initIdt();
extern "C" void kmain(std::uint32_t magic, std::uint32_t mbiAddr)	// EAX and EBX
{
	multiboot_info_t *mbi = NULL;
	Multiboot2BootInfo *mbi2 = NULL;
    initialiseConstructors();
    /* paging is enabled before kmain is entered */
    bootTerminal::init();
    bootTerminal::prints("kernel default console inited\n");
    switch(magic)
    {
    	case MULTIBOOT2_BOOTLOADER_MAGIC:
    		mbi2 = (Multiboot2BootInfo*)VirtualMemoryManager::addAfterKernel(mbiAddr);
    		{
    			int bytesLeft = mbi2->total_size - (4096 - unsigned(mbi2) % 4096);
    			unsigned pagesMapped = 1;
    			while(bytesLeft > 0)
    			{
    				bochsBreak();
    				VirtualMemoryManager::addAfterKernel(mbiAddr + 4096 * pagesMapped);
    				++pagesMapped;
    				bytesLeft -= 4096;
    			}
    		}
    		print_multiboot2_info((Multiboot2BootInfo*)mbi2);
    		break;
    	case 0x2BADB002:
    		mbi = (multiboot_info_t*)mbiAddr;
			print_multiboot_info(mbi);
    		break;
    	default:
    		bootTerminal::prints("panic: magic not 0x2BADB002\n");
        	while(1){;}
    }

	//bootTerminal::prints("kernel length is ");
	//bootTerminal::print10((unsigned)(&kernel_end) - 0xC0000000);
	//bootTerminal::printc('\n');

    if(magic == 0x2badb002)
    	physicalMemoryManager::init(mbi);
    else if(magic == MULTIBOOT2_BOOTLOADER_MAGIC)
    	physicalMemoryManager::init(mbi2, mbiAddr);
    bootTerminal::prints("Physical memory manager inited\n");

    //VirtualMemoryManager::init();
    //bootTerminal::prints("Virtual memory manager inited\n");

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

void print_multiboot2_info(Multiboot2BootInfo *mbi2)
{
	bootTerminal::prints("multiboot2 info:\nmbi size: ");
	bootTerminal::print10(mbi2->total_size);
    bootTerminal::printc('\n');
    
    // iterate through multiboot2 information tags, note that tag->size does NOT
    // include padding so 7 is added and the result rounded down to the nearest 8
    for(Multiboot2Tag* tag = mbi2->tags;
    	tag->type != MULTIBOOT2_TAG_TYPES::END;
    	tag = (Multiboot2Tag*) ((std::uint8_t*)tag + (tag->size + 7 & ~7)))
    {
    	/*bootTerminal::print10((unsigned)(tag->type));
    	bootTerminal::printc(':');
    	bootTerminal::print10(tag->size);
    	bootTerminal::printc('\n');*/
    	if(tag->type == MULTIBOOT2_TAG_TYPES::CMDLINE)
    	{
    		Multiboot2Tag_string* tag_s = (Multiboot2Tag_string*)tag;
    		bootTerminal::prints("command line: ");
    		bootTerminal::prints(tag_s->string);
    		bootTerminal::printc('\n');
    	}
    	else if(tag->type == MULTIBOOT2_TAG_TYPES::BOOT_LOADER_NAME)
    	{
    		Multiboot2Tag_string* tag_bln = (Multiboot2Tag_string*)tag;
    		bootTerminal::prints("boot loader name: ");
    		bootTerminal::prints(tag_bln->string);
    		bootTerminal::printc('\n');
    	}
    	else if(tag->type == MULTIBOOT2_TAG_TYPES::MODULE)
    	{
    		Multiboot2Tag_module* tagmod = (Multiboot2Tag_module*)tag;
    		bootTerminal::prints("mod_start: ");
    		bootTerminal::print10(tagmod->mod_start);
    		bootTerminal::prints("mod_end: ");
    		bootTerminal::print10(tagmod->mod_end);
    		bootTerminal::prints("string: ");
    		bootTerminal::prints(tagmod->string);
    		bootTerminal::printc('\n');
    	}
    	else if(tag->type == MULTIBOOT2_TAG_TYPES::BASIC_MEMINFO)
    	{
    		Multiboot2Tag_basicMeminfo* tagbmem = (Multiboot2Tag_basicMeminfo*)tag;
    		bootTerminal::prints("lower mem: ");
			bootTerminal::print10(tagbmem->mem_lower);
			bootTerminal::prints(", upper mem: ");
			bootTerminal::print10(tagbmem->mem_upper);
			bootTerminal::printc('\n');
    	}
    	else if(tag->type == MULTIBOOT2_TAG_TYPES::BOOTDEV)
    	{
    		Multiboot2Tag_bootdev* tagbd = (Multiboot2Tag_bootdev*)tag;
    		bootTerminal::prints("biosdev: ");
			bootTerminal::printh(tagbd->biosdev);
			bootTerminal::prints(", partition: ");
			bootTerminal::printh(tagbd->partition);
			bootTerminal::prints(", sub_partition: ");
			bootTerminal::printh(tagbd->sub_partition);
			bootTerminal::printc('\n');
    	}
    	else if(tag->type == MULTIBOOT2_TAG_TYPES::MMAP)
    	{
    		Multiboot2Tag_mmap* tagmm = (Multiboot2Tag_mmap*)tag;
    		bootTerminal::prints("mmap data\nentry_size: ");
			bootTerminal::print10(tagmm->entry_size);
			bootTerminal::prints("\nentry_version: ");
			bootTerminal::print10(tagmm->entry_version);
			bootTerminal::printc('\n');
			std::size_t numEntries = (tagmm->size - sizeof(*tagmm)) / tagmm->entry_size;
			for(std::size_t i = 0; i < numEntries; ++i)
			{
				Multiboot2_mmapEntry* entry = (Multiboot2_mmapEntry*)((std::uint8_t*)tagmm->entries + i * tagmm->entry_size);
				bootTerminal::prints("base_addr: ");
				bootTerminal::print10(entry->base_addr);	// ignoring for the moment that a 64 bit is coerced to 32 bit
				bootTerminal::prints(", length: ");
				bootTerminal::print10(entry->length);
				bootTerminal::prints(", type: ");
				bootTerminal::print10(entry->type);
				bootTerminal::printc('\n');	
			}
    	}
    }
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
