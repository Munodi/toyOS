#ifndef _TOYOS_VIRTUAL_ADDRESS_SPACE_HPP_
#define _TOYOS_VIRTUAL_ADDRESS_SPACE_HPP_
#include <toyOS/types.hpp>
#include <cstdint>
#include <cstddef>
#include <sys/types.h>
#include <vector>

// address spzces should be refered to as handles, the handles being the physical address of the top level table

class virtualAddressSpace
{
    public:
        typedef physical_uintptr_t addressSpaceHandle;
        // note that these are not for any particular processor,
        //  but for map, which handles each processor
        // note present, swapped, swappingIn, swappingOut, bss and copyOnWrite are mutually exclusive
        // a copyOnWrite page is marked by a read-only page being in a writable section
        // therefore a page cannot be read-only and copy-on-write, this is like the .code section
        // note that processes and virtualAddressSpaces exist on a 1-1 basis
        const static std::uint32_t present     = 1 << 0;
        const static std::uint32_t largePage   = 1 << 1;
        const static std::uint32_t supervisor  = 1 << 2;
        const static std::uint32_t copyOnWrite = 1 << 3;
        const static std::uint32_t writable    = 1 << 4;
        const static std::uint32_t swapped     = 1 << 5;
        const static std::uint32_t swappingIn  = 1 << 6;
        const static std::uint32_t swappingOut = 1 << 7;
        const static std::uint32_t stackGuard  = 1 << 8;
        const static std::uint32_t bss         = 1 << 9;
        const static std::uint32_t noExecute   = 1 << 10;

        static std::uint32_t getSmallPageSize();
        static std::uint32_t getLargePageSize();
        void map(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags);
        void unmap(void* virtualAddress);
        std::size_t sbrk(std::ssize_t increment);   // if negative it decreases allocated space
        /**
            This can be called as a server system call by the pm.
            It returns the topLevelPageTable of the new virtualAddressSpace or
            0 if it can't create one.
            The pm refers to a virtualAddressSpace by its topLevelPageTable as
            a virtualAddressSpace will always have the same topLevelPageTable
            and they can be stored in a data structure that copies its entries
            such as an std::vector.
        */
        static physical_uintptr_t create();
        /**
            Deletes a virtualAddressSpace, this should be called
            after closing the owning process (ie, flush all buffers,
            et cetera) and unmaping all the pages. As unmap does not
            need to remove all the page tables this function does
            that.
        */
        static void remove(physical_uintptr_t reference);


    private:
        virtualAddressSpace();
        void* findFirstStackGuard();    // used by sbrk so it doesn't run into the stacks
        physical_uintptr_t topLevelPageTable;
        pid_t owningProcess;
        void* heapStart;
        std::size_t heapSize;
        static std::vector<virtualAddressSpace> addressSpaces;
};

#endif // _TOYOS_VIRTUAL_ADDRESS_SPACE_HPP_
