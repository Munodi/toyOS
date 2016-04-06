#ifdef x86
#include <cstdint>
#include <cstdlib>
#include <toyOS/virtualMemoryManager.hpp>
#include <toyOS/physicalMemoryManager.hpp>
#include <toyOS/bootTerminal.hpp>
#include <toyOS/panic.hpp>
#include <toyOS/arch/x86/asm.hpp>

//static std::uint32_t readCR0();
//static void writeCR0(std::uint32_t);
//static bool usingPSE();
extern "C" unsigned kernel_end;

void* VirtualMemoryManager::afterWholeKernel = &kernel_end;

/*void VirtualMemoryManager::init()
{
    ;
}*/

std::size_t VirtualMemoryManager::allocInKernelSpace(void* va, std::size_t size)
{
    std::size_t actuallyAllocated = 0;
    // legacy x86 paging, TODO add PSE handling code
    
    std::uint32_t* pageTables = (std::uint32_t*)0xFFC00000;
    std::uint32_t pageTableOffset = (std::uint32_t)va / (std::uint32_t)(4096);
    std::size_t pagesNeeded;
    // initialise pagesNeeded
    size = size + (std::uint32_t)va % (std::uint32_t)(4096); // add bytes before page-align
    while(size % 4096 != 0)     // make size a multiple of the page size
        ++size;
    pagesNeeded = size / 4096;

    // allocate pages at *pageTables + pageTableOffset of number pagesNeeded
    for(std::size_t i = 0; i < pagesNeeded; i++)
    {
        // make page present if not and allocate frame
        if(!(pageTables[pageTableOffset] & (std::uint32_t)1)) // if present bit is not set
        {
            const std::uint32_t flags = 1 | (1<<1); // present and writable
            std::uint64_t frame = physicalMemoryManager::allocSmallPage();
            if(frame == 1)
                return actuallyAllocated;
            std::uint32_t pte = (std::uint32_t)frame;
            pte = pte & 0xFFFFF000;
            pte = pte | flags;
            pageTables[pageTableOffset] = pte;
            // TODO: invalidate code goes here
        }
        actuallyAllocated = actuallyAllocated + 4096;
        ++pageTableOffset;
    }

    return actuallyAllocated;
}

void VirtualMemoryManager::deAllocInKernelSpace(void* va, std::size_t size)
{
    //assert(size%4096 == 0);
    if(size%4096 != 0)  // will do for now
        return;
    // asuming non-PSE, non-PAE for now

    std::uint32_t* pageTables = (std::uint32_t*)0xFFC00000;
    std::uint32_t pageTableOffset = (std::uint32_t)va / (std::uint32_t)(4096);
    std::size_t pagesToFree = size/4096;
    for(std::size_t i = 0; i < pagesToFree; i++)
    {
        pageTables[pageTableOffset] = pageTables[pageTableOffset] & (~1);
        ++pageTableOffset;
    }
    return;
}   // check through this, written quinkly

// maps a page at afterWholeKernel then adds a page size to afterWholeKernel. Does not depend on physical memory manager
void* VirtualMemoryManager::addAfterKernel(physical_uintptr_t physicalAddress)
{
	// enough page tables have been created in loader.s, just need to set the right pte
    std::uint32_t* pageTables = (std::uint32_t*)0xFFC00000;
    std::uint32_t pageTablesIndex = (std::uint32_t)afterWholeKernel / 4096;

	std::uint32_t pte = (physicalAddress & 0xFFFFF000) | 1;
	
	pageTables[pageTablesIndex] = pte;
	invlpg(afterWholeKernel);
	
	afterWholeKernel += 4096;
	
	return afterWholeKernel - 4096 + (physicalAddress & 0x00000FFF);
}

/*static bool usingPSE()
{
    std::uint32_t CR4;
    asm volatile ( "mov %%cr4, %0"
                  : "=r"(CR4) );
    return (CR4 & (1<<4));
}*/

/*static std::uint32_t readCR0()
{
    std::uint32_t CR0;
        asm volatile ( "mov %%cr0, %0"
                  : "=r"(CR0) );
    return CR0;
}*/

#endif // x86
