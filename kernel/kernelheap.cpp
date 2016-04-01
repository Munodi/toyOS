#include <toyOS/kernelHeap.hpp>
#include <toyOS/memoryLayout.hpp>
#include <toyOS/virtualMemoryManager.hpp>
#include <toyOS/physicalMemoryManager.hpp>
#include <toyOS/types.hpp>
#include <toyOS/bootTerminal.hpp>
#include <toyOS/panic.hpp>
#include <cstdlib>
#include <cstdint>

/*
    TODO: look over this code
*/

static const std::uintptr_t startOfRegion = KERNEL_HEAP_START;
static std::size_t virtualMemoryAllocated = 0;
const std::size_t maximumHeapSize = (KERNEL_HEAP_END - KERNEL_HEAP_START);

struct zoneHeader
{
    std::size_t zoneSize;
    std::size_t used;       // 1 if used, else 0
    zoneHeader *prev, *next;
};  // pad to 8 byte align

// functions used internally to make stuff easier
static inline void  unify(zoneHeader* header);
static inline void  split(zoneHeader* header, std::size_t size);    // splits the zone so that the pointer passed points to a smaller zone, NOT then second
static zoneHeader*  findZone(std::size_t size); // returns pointer to a free zone,this function can break zones up into smaller zones, returns NULL if none able to be allocated
static bool         grow(std::size_t size);     // grows the heap at least size bytes, returns false on failure

void kernelHeap::init()
{
    virtualMemoryAllocated = virtualMemoryManager::allocInKernelSpace((void*)startOfRegion, kernelHeap::minimumSize);
    zoneHeader* head = (zoneHeader*)KERNEL_HEAP_START;
    head->prev = NULL;
    head->next = NULL;
    head->used = 0;
    head->zoneSize   = virtualMemoryAllocated - sizeof(zoneHeader);
    bootTerminal::prints("first zone size: ");
	bootTerminal::print10(head->zoneSize);
	bootTerminal::prints(", used: ");
    bootTerminal::print10(head->used);
	bootTerminal::printc('\n');
}

void* kernelHeap::malloc(std::size_t size)
{
    zoneHeader* header = findZone(size);
    if(header != NULL)
    {
        void* pointer = (void*)header;
        pointer = pointer + sizeof(zoneHeader);
        return pointer;
    }
    if(grow(size))
    {
        header = findZone(size);
        void* pointer = (void*)header;
       pointer = pointer + sizeof(zoneHeader);
       return pointer;
    }
    return NULL;
}

void kernelHeap::free(void* address)
{
    zoneHeader* header = (zoneHeader*)((std::uintptr_t)address - sizeof(zoneHeader));
    header->used = 0;
    unify(header);
    return;
}

static void unify(zoneHeader* header)
{
    if(header->used == 0 and header->next != NULL and header->next->used == 0)  // unify right
    {
        header->zoneSize = header->zoneSize + sizeof(zoneHeader) + header->next->zoneSize;
        header->next = header->next->next;
        if(header->next != NULL)
            header->next->prev = header;    // should draw some diagrams
    }
    ;   // unify left code goes here
}

static void split(zoneHeader* header, std::size_t size)
{
    zoneHeader* nextHeader;
    nextHeader = (zoneHeader*)((std::size_t)header + sizeof(zoneHeader) + size);
    nextHeader->next = header->next;
    nextHeader->prev = header;
    nextHeader->zoneSize = (std::size_t)nextHeader->next - (std::size_t)nextHeader + sizeof(zoneHeader);
    nextHeader->used = 0;
    header->next = nextHeader;
    header->zoneSize = size;
    return;
}

static zoneHeader* findZone(std::size_t size)
{
    while(size%8 != 0)
        ++size;
    zoneHeader* header = (zoneHeader*)KERNEL_HEAP_START;

    for(;;)
    {
        if(header->used == 0 and header->zoneSize >= size)   // success! found usable zone
        {
            if(size > header->zoneSize + sizeof(zoneHeader) + 16)   // split only if left over chunk will fit 16 bytes
                split(header, size);
            header->used = 1;
            return header;
        }

    }
}

static bool grow(std::size_t size)
{
    // fix so that doesn't have trouble if it doesn't allocate enough room acounting for sizeof(zoneHeader)
    size = size + sizeof(zoneHeader);   // there problem fixed

    if(virtualMemoryAllocated + size >= maximumHeapSize or size > maximumHeapSize)  // second check is in case firts overflows as size is to big, should never happen with all normal sized allocations
        return false;
    std::size_t allocatedNow = virtualMemoryManager::allocInKernelSpace((void*)(KERNEL_HEAP_START + virtualMemoryAllocated), size);
    if(allocatedNow < size) // if unable to allocate enough space, dealloce space it allocated and return NULL
    {
        virtualMemoryManager::deAllocInKernelSpace((void*)(KERNEL_HEAP_START + virtualMemoryAllocated), allocatedNow);
        return NULL;
    }
    virtualMemoryAllocated = virtualMemoryAllocated + allocatedNow; // if no problem increase heap size

    // find last zoneHeader
    zoneHeader* header = (zoneHeader*)KERNEL_HEAP_START;
    while(header->next != NULL)
        header = header->next;

    // if last is unused then increase size
    if(header->used == 0)
    {
        header->zoneSize = (virtualMemoryAllocated - (std::size_t)header - sizeof(zoneHeader));
    }
    else    // else last is used and should create new zone
    {
        header->next = (header + sizeof(zoneHeader) + header->zoneSize);    // point old last zoneHeader to new zone we are about to create
        zoneHeader* previousHeader = header;
        header = header->next;
        header->next = NULL;
        header->prev = previousHeader;
        header->used = 0;
        header->zoneSize = (virtualMemoryAllocated - (std::size_t)header - sizeof(zoneHeader));
    }

    return true;
}
