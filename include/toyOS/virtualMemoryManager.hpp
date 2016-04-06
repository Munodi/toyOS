#pragma once

#include <cstdlib>
#include <cstdint>
#include <toyOS/virtualAddressSpace.hpp>

class VirtualMemoryManager
{
    public:
        //static void init();
        static void mapInKernelSpace(physical_uintptr_t physicalAddress, void* virtualAddress, std::uint32_t flags);
        static std::size_t allocInKernelSpace(void* va, std::size_t size); // allocates room at va of size n bytes, returns how much allocated
        static        void deAllocInKernelSpace(void* va, std::size_t size);    // size must be page aligned
        static std::uint32_t getSmallPageSize();
        static std::uint32_t getLargePageSize();
        static void* addAfterKernel(physical_uintptr_t physicalAddress);

    private:
        VirtualMemoryManager() {;}
        
        static void* afterWholeKernel;		// pointer past anything mapped directly after kernel (multiboot structures, etc), initialised to kernel_end
};

