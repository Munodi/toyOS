#ifndef _TOYOS_KERNEL_HEAP_HPP_
#define _TOYOS_KERNEL_HEAP_HPP_

#include <toyOS/memoryLayout.hpp>
#include <cstdint>
#include <cstddef>

// maybe I don't need multiple kernel heaps, they would only be of use if the kernel is pre-emptive anyway
// Decided, am making static, actually if I want multiple cores to run the kernel at once I need multiple heaps

class kernelHeap
{
    public:
        static void init();
        static void* malloc(std::size_t);
        static void  free(void*);
    private:
        static const std::size_t minimumSize = (4 * 1024 * 1024);
};

#endif // _TOYOS_KERNEL_HEAP_HPP_
