/*
  pmm.cpp (c) George Robertson 2011
    TODO not done yet
*/

#ifdef x86

#include <cstdlib>
#include <cstdint>
#include <toyOS/physicalMemoryManager.hpp>
#include <toyOS/multiboot.h>
#include <toyOS/types.hpp>
#include <toyOS/bootTerminal.hpp>

#include <bitset>

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

void physicalMemoryManager::debug()
{
    char tmp[20];
    /*for(std::size_t i = 0; i < 1024*1024*3/4096; i++)
    {
        if(smallPageBitset.test(i))
            bootTerminal::printc('1');
        else
            bootTerminal::printc('0');
    }*/
    bootTerminal::printc('\n');
	bootTerminal::prints("system memory: ");
	bootTerminal::print10(systemMemory);
	bootTerminal::printc('\n');
	bootTerminal::prints("free memory: ");
	bootTerminal::print10(freeMemory);
	bootTerminal::printc('\n');
	bootTerminal::prints("first unset bit: ");    
	bootTerminal::prints(std::utoa(smallPageBitset._find_first_off(), &(tmp[0]), 10));
	bootTerminal::printc('\n');
}

#endif // x86
