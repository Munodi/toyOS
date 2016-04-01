/*
 * This class is static as code can only run on the current processor
 * After the kernel heap has been set up, the initrd file has been read
 * and processes loaded, main() will call
 *
 */


#ifndef _TOYOS_PROCESSOR_HPP_
#define _TOYOS_PROCESSOR_HPP_

#include <cstdint>
#include <toyOS/config.hpp>

struct processorInfoBlock
{
    void* stack;
    pid_t currentProcess;
    tid_t currentTread;
    int ticksLeft;
    bool startedWork;
};

class processor
{
    public:
        static std::uint32_t getLogicalProcessorNumber();
        static std::uint32_t logicalProcessorsInSystem;
        static std::uint32_t activatedLogicalProcessors;
        static void initAllCores();
    private:
        static processorInfoBlock _pfi[MAXIMUM_LOGICAL_PROCESSORS];
};

#endif // _TOYOS_PROCESSOR_HPP_
