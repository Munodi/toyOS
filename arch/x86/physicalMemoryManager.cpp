/*
  pmm.cpp (c) George Robertson 2016
    TODO not done yet
*/

#ifdef x86

#include <cstdlib>
#include <cstdint>
#include <toyOS/physicalMemoryManager.hpp>
#include <toyOS/multiboot.h>
#include <toyOS/multiboot2.hpp>
#include <toyOS/types.hpp>
#include <toyOS/bootTerminal.hpp>

#include <bitset>

// TODO: will not work properly on a machine with > 4gb ram

const std::uint64_t maximumMemory = 32;     // 1 << maximumMemory
const std::uint32_t smallPageSize = 12;     // 1 << smallPageSize
const std::uint32_t largePageSize = 22;     // 1 << largePageSize

static std::bitset<(1 << (maximumMemory - smallPageSize))> smallPageBitset;
static std::bitset<(1 << (maximumMemory - largePageSize))> largePageBitset;

std::uint64_t systemMemory = 0;
std::uint64_t freeMemory = 0;

std::uint32_t physicalMemoryManager::getSmallPageSize()
{
    return (1 << smallPageSize);
}

std::uint32_t physicalMemoryManager::getLargePageSize()
{
    return (1 << largePageSize);
}

std::uint64_t physicalMemoryManager::getMaximumMemory()
{
    return (1ULL << maximumMemory);
}

std::uint64_t physicalMemoryManager::getSystemMemory()
{
    return systemMemory;
}

std::uint64_t physicalMemoryManager::getFreeMemory()
{
    return freeMemory;
}

std::uint64_t physicalMemoryManager::allocSmallPage()
{
    // TODO: add locks
    std::size_t bit;
    bit = smallPageBitset._find_first_off();
    if(bit == (std::size_t)-1)
        return 1;
    smallPageBitset.set(bit);
	freeMemory -= (1 << smallPageSize);
    return (std::uint64_t)(bit * getSmallPageSize());
}

extern "C" unsigned kernel_end;

void physicalMemoryManager::init(multiboot_info_t *mbi)
{
    smallPageBitset.set();
    largePageBitset.set();

    char tmp[20];

    std::uint64_t freeFrame;    // freeFrame is the address of memory area after frame aligning
    std::int64_t bytesLeft;     // bytesLeft is the length of memory area after frame aligning
    multiboot_mmap_entry* mmap;
    mmap = (multiboot_mmap_entry*)(mbi->mmap_addr);
    multiboot_uint32_t numberOfMemoryMapEntries = ((mbi->mmap_length) / sizeof(multiboot_memory_map_t));

    // mark unused bits according to the memory map
    for(multiboot_uint32_t i = 0; i < numberOfMemoryMapEntries; i++)
    {
        if(mmap[i].type != 1)
            continue;
        bytesLeft = std::int64_t(mmap[i].len);
        freeFrame = mmap[i].addr;
        while((freeFrame % getSmallPageSize()) != 0)
        {
            --bytesLeft;
            ++freeFrame;
        }
        if(bytesLeft <= 0)
            continue;       // memory area is smaller than page frame size
        do
        {
            smallPageBitset.reset(freeFrame / getSmallPageSize());
            freeFrame = freeFrame + getSmallPageSize();
            bytesLeft = bytesLeft - getSmallPageSize();
            systemMemory += 4096;
            freeMemory += 4096;
        }while(bytesLeft > 0);




    }

    // mark lower 1mb and kernel
    {
        std::uint32_t kernelLength = (unsigned)(&kernel_end) - 0xC0000000;
        kernelLength = kernelLength / getSmallPageSize();
        bootTerminal::prints(std::utoa(kernelLength, &(tmp[0]), 10));
        bootTerminal::printc('\n');
        for(std::uint32_t i = 0; i < kernelLength; i++)
        {
            smallPageBitset.set(i);
			freeMemory -= (1 << smallPageSize);
        }

    }

    // TODO: modules


    return;

}

void physicalMemoryManager::init(Multiboot2BootInfo *mbi2, std::uint32_t mbi2Phys)
{
	// first set all page frames as used, then unset page frames that are
	// available, finally set all pages that are used
	smallPageBitset.set();
    largePageBitset.set();
	
    std::uint64_t freeFrame;    // freeFrame is the address of memory area after frame aligning
    std::int64_t bytesLeft;     // bytesLeft is the length of memory area after frame aligning
    
	
	Multiboot2Tag_mmap* mmapTag = nullptr;
    // iterate through multiboot2 information tags to find the memory map, note
    // that tag->size does NOT include padding so 7 is added and the result
    // rounded down to the nearest 8
    for(Multiboot2Tag* tag = mbi2->tags;
    	tag->type != MULTIBOOT2_TAG_TYPES::END;
    	tag = (Multiboot2Tag*) ((std::uint8_t*)tag + (tag->size + 7 & ~7)))
    {
    	if(tag->type == MULTIBOOT2_TAG_TYPES::MMAP)
    	{
    		mmapTag = (Multiboot2Tag_mmap*)tag;
    		break;
    	}
    }
    
    std::uint32_t numEntries = (mmapTag->size - sizeof(*mmapTag)) / mmapTag->entry_size;
    // mark unused bits according to the memory map
    for(std::size_t i = 0; i < numEntries; i++)
    {
    	Multiboot2_mmapEntry* mmapEntry = (Multiboot2_mmapEntry*)((std::uint8_t*)mmapTag->entries + i * mmapTag->entry_size);
        if(mmapEntry->type != 1)
            continue;
        bytesLeft = mmapEntry->length;
        freeFrame = mmapEntry->base_addr;
        while((freeFrame % getSmallPageSize()) != 0)
        {
            --bytesLeft;
            ++freeFrame;
        }
        if(bytesLeft <= 0)
            continue;       // memory area is smaller than page frame size
        do
        {
            smallPageBitset.reset(freeFrame / getSmallPageSize());
            freeFrame = freeFrame + getSmallPageSize();
            bytesLeft = bytesLeft - getSmallPageSize();
            systemMemory += 4096;
            freeMemory += 4096;
        }while(bytesLeft > 0);




    }
	
    // mark lower 1mb and kernel
    {
        std::uint32_t kernelLength = (unsigned)(&kernel_end) - 0xC0000000;
        kernelLength = kernelLength / getSmallPageSize();
        bootTerminal::print10(kernelLength);
        bootTerminal::printc('\n');
        for(std::uint32_t i = 0; i < kernelLength; i++)
        {
            smallPageBitset.set(i);
			freeMemory -= (1 << smallPageSize);
        }
    }
    
    // mark mbi
	for(std::uint32_t frame = mbi2Phys & 0xFFFFF000;
		frame < mbi2Phys + mbi2->total_size;
		frame += 4096)
	{
		smallPageBitset.set(frame / 4096);
		// the if is needed because the code above marks all memory less
		// than 1mb as used, it can be removed if above code is changed
		if(frame >= (unsigned)(&kernel_end) - 0xC0000000)
			freeMemory -= 4096;
	}

    // TODO: set pages that the modules are in


    return;
}

void physicalMemoryManager::debug()
{
    /*for(std::size_t i = 0; i < 1024*1024*3/4096; i++)
    {
        if(smallPageBitset.test(i))
            bootTerminal::printc('1');
        else
            bootTerminal::printc('0');
    }*/
    bootTerminal::printc('\n');
	bootTerminal::prints("system memory: ");
	bootTerminal::print10(systemMemory / 1024);
	bootTerminal::prints("kb\n");
	bootTerminal::prints("free memory: ");
	bootTerminal::print10(freeMemory / 1024);
	bootTerminal::prints("kb\n");
	bootTerminal::prints("first unset bit: ");    
	bootTerminal::print10(smallPageBitset._find_first_off());
	bootTerminal::printc('\n');
}

#endif // x86
