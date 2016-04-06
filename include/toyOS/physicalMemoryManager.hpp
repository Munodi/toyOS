/*
  pmm.hpp (c) George Robertson 2016

*/

#pragma once

#include <cstdlib>
#include <cstdint>
#include <toyOS/multiboot.h>
#include <toyOS/multiboot2.hpp>
#include <toyOS/types.hpp>

class physicalMemoryManager
{
    public:
        static void init(multiboot_info_t *mbi);
        static void init(Multiboot2BootInfo *mbi2, std::uint32_t mbi2Phys);
        // returns max memory allowed for in implementation of manager
        static std::uint64_t getMaximumMemory();
        // returns usable memory in system
        static std::uint64_t getSystemMemory();
        static std::uint64_t getFreeMemory();
        static std::uint64_t allocSmallPage();       // returns address, 1 if none available, no system has 1 byte pages so as an address is returned 1 should never be a valid address
        static std::uint64_t allocLargePage();       // returns address, 1 if none available
        static void freeSmallPage(std::uint64_t);    // expects address
        static void freeLargePage(std::uint64_t);    // expects address
        static std::uint32_t getSmallPageSize();
        static std::uint32_t getLargePageSize();
        static void debug();

    private:
        physicalMemoryManager() {;}

};

