#ifndef _TOYOS_VIRTUAL_MEMORY_MANAGER_HPP_
#define _TOYOS_VIRTUAL_MEMORY_MANAGER_HPP_

#include <cstdlib>
#include <cstdint>
#include <toyOS/virtualAddressSpace.hpp>

class virtualMemoryManager
{
    public:
        static void init();
        static void mapInKernelSpace(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags);
        static std::size_t allocInKernelSpace(void* va, std::size_t size); // allocates room at va of size n bytes, returns how much allocated
        static        void deAllocInKernelSpace(void* va, std::size_t size);    // size must be page aligned
        static std::uint32_t getSmallPageSize();
        static std::uint32_t getLargePageSize();

    private:
        virtualMemoryManager() {;}
};

#endif // _TOYOS_VIRTUAL_MEMORY_MANAGER_HPP_
