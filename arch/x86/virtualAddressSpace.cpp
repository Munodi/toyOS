#ifdef x86
#include <toyOS/virtualAddressSpace.hpp>
#include <cstdint>
#include <cstring>

std::vector<virtualAddressSpace> virtualAddressSpace::addressSpaces;

/**
 * Intel gives us a present, accessed, page size, supervisor, cache disabled,
 *   writable, and write-through flags, as well as 3 bits to use as wanted
 * A page cannot be more than one of bss, swapped and stack guard, and
 * when it is either of these it is not present.
 * When a page is copy-on-write the last bit left is set
 * Macros below
 */

#define PRESENT_NON_PAE   (1<<0)
#define WRITEABLE_NON_PAE (1<<1)
#define USER_NON_PAE      (1<<2)
#define ACCESSED_NON_PAE  (1<<

static bool usingPAE();
static void mapNonPAE(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags);
//static void mapPAE(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags);
static void unmapNonPAE(void* virtualAddress);
//static void unmapPAE(void* virtualAddress);

void virtualAddressSpace::map(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags)
{
    if(!usingPAE())
        mapNonPAE(physicalAddress, virtualAddress, flags);
    else
        /*mapPAE(physicalAddress, virtualAddress, flags)*/;
    return;
}

void virtualAddressSpace::unmap(void* virtualAddress)
{
    if(!usingPAE())
        unmapNonPAE(virtualAddress);
    else
        /*unmapPAE(virtualAddress)*/;
    return;
}

static void mapNonPAE(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags)
{
    // TODO: add PSE code
    std::uint32_t* pageDirectory = (std::uint32_t*)0xFFFFF000;
    std::uint32_t pageDirectoryIndex = (std::uint32_t)virtualAddress / (std::uint32_t)(4096*1024);
    std::uint32_t* pageTables = (std::uint32_t*)0xFFC00000;
    std::uint32_t pageTableIndex = (std::uint32_t)virtualAddress / (std::uint32_t)(4096);
    //if(!(pageDirectory[pageDirectoryIndex] & 1))    // if page table is not present then allocate, initialise, and set pde
    //{
    //
    //}
    ; // TODO fill in
}

/*static void mapPAE(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags)
{
    ; //TODO fill in
}*/

static void unmapNonPAE(void* virtualAddress)
{
    // TODO: add PSE code
    std::uint32_t* pageDirectory = (std::uint32_t*)0xFFFFF000;
    std::uint32_t pageDirectoryIndex = (std::uint32_t)virtualAddress / (std::uint32_t)(4096*1024);
    std::uint32_t* pageTables = (std::uint32_t*)0xFFC00000;
    std::uint32_t pageTableIndex = (std::uint32_t)virtualAddress / (std::uint32_t)(4096);
    if(!(pageDirectory[pageDirectoryIndex] & 1))
        return;     // if pagetable of address is not present then return
    pageTables[pageTableIndex] = 0;
    asm volatile ("invlpg (%0)" : : "a" (virtualAddress));

}

/*static void unmapPAE(void* virtualAddress)
{
    // TODO: fill in;
}*/

static bool usingPAE()
{
    std::uint32_t CR4;
    asm volatile( "mov %%cr4, %0"
                  : "=r"(CR4) );
    return (CR4 & (1<<5));
}

#endif  // x86
