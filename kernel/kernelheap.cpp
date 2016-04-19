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

const std::size_t MINIMUM_ALIGNMENT = 8;

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
    virtualMemoryAllocated = VirtualMemoryManager::allocInKernelSpace((void*)startOfRegion, kernelHeap::minimumSize);
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
    // round size up to MINIMUM_ALIGNMENT
    size = (size + MINIMUM_ALIGNMENT - 1) & ~(MINIMUM_ALIGNMENT - 1);

    zoneHeader* header = findZone(size);
    if(header != NULL)
    {
        void* pointer = (void*)header;
        pointer = (void*)((std::uint8_t*)pointer + sizeof(zoneHeader));
        return pointer;
    }
    if(grow(size))
    {
        header = findZone(size);
        void* pointer = (void*)header;
       pointer = (void*)((std::uint8_t*)pointer + sizeof(zoneHeader));
       return pointer;
    }
    return NULL;
}

void kernelHeap::free(void* address)
{
    if(address) // if not NULL
    {
        zoneHeader* header = (zoneHeader*)((std::uintptr_t)address - sizeof(zoneHeader));
        header->used = 0;
        unify(header);
    }
    return;
}

static void unify(zoneHeader* header)
{
    if(header->next and header->next->used == 0)    // unify right
    {
        header->zoneSize = header->zoneSize + sizeof(zoneHeader) + header->next->zoneSize;
        header->next = header->next->next;
        if(header->next)    // if now not at the last zone, set next's header's prev pointer
            header->next->prev = header;    // should draw some diagrams
    }
    if(header->prev and header->prev->used == 0)    // unify left
    {
        zoneHeader *previous = header->prev;

        previous->zoneSize = previous->zoneSize + sizeof(zoneHeader) + header->zoneSize;
        previous->next = header->next;
        if(header->next)    // if *header is not the last zone, set the next zoneHeader's prev pointer
            header->next->prev = previous;
    }
}

static void split(zoneHeader* header, std::size_t size)
{
    // Create next zone after the space used by this allocation
    zoneHeader* nextHeader;
    nextHeader = (zoneHeader*)((std::uint8_t*)header + sizeof(zoneHeader) + size);
    nextHeader->next = header->next;
    nextHeader->prev = header;
    // 2016 I don't think this will work when splitting the last zone, the line after should fix it
    //nextHeader->zoneSize = (std::size_t)nextHeader->next - (std::size_t)nextHeader + sizeof(zoneHeader);
    nextHeader->zoneSize = header->zoneSize - size - sizeof(zoneHeader);
    nextHeader->used = 0;

    // Add nextHeader to the linked list, note the order of these lines is important otherwise nextHeader->prev is set to nextHeader
    if(header->next)
        header->next->prev = nextHeader;
    header->next = nextHeader;
    
    header->zoneSize = size;
    return;
}

static zoneHeader* findZone(std::size_t size)
{
    zoneHeader* header = (zoneHeader*)KERNEL_HEAP_START;

    for(;;)
    {
        if(header->used == 0 and header->zoneSize >= size)   // success! found usable zone
        {
            if(header->zoneSize >= size + sizeof(zoneHeader) + MINIMUM_ALIGNMENT)   // split only if left over chunk will fit another zone note fixed in 2016
                split(header, size);
            header->used = 1;
            return header;
        }
        else if(header->next == NULL)	// new code, might fix bug
        {
        	return NULL;
        }
        else
        {
        	header = header->next;
        }	// end new code

    }
}

static bool grow(std::size_t size)
{
    // fix so that doesn't have trouble if it doesn't allocate enough room acounting for sizeof(zoneHeader)
    size = size + sizeof(zoneHeader);   // there problem fixed

    if(virtualMemoryAllocated + size >= maximumHeapSize or size > maximumHeapSize)  // second check is in case firts overflows as size is to big, should never happen with all normal sized allocations
        return false;
    std::size_t allocatedNow = VirtualMemoryManager::allocInKernelSpace((void*)(KERNEL_HEAP_START + virtualMemoryAllocated), size);
    if(allocatedNow < size) // if unable to allocate enough space, dealloce space it allocated and return NULL
    {
        VirtualMemoryManager::deAllocInKernelSpace((void*)(KERNEL_HEAP_START + virtualMemoryAllocated), allocatedNow);
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
        // note in 2016: won't work references unallocated memory
        zoneHeader* previousHeader = header;
        header = header->next;
        header->next = NULL;
        header->prev = previousHeader;
        header->used = 0;
        header->zoneSize = (virtualMemoryAllocated - (std::size_t)header - sizeof(zoneHeader));
    }

    return true;
}
