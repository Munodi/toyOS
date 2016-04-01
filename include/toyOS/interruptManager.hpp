#ifndef _TOYOS_INTERRUPT_MANAGER_HPP_
#define _TOYOS_INTERRUPT_MANAGER_HPP_

#include <cstdint>
#include <string>
#include <sys/types.h>

class interruptManager
{
    public:
        static void init();
        /// remove the handler associated with the interrupt mask irq.
        static void irqClear(std::uint32_t irq);
        /// Allow the given interrupt to occur
        static void registerIRQ(std::uint32_t irq, pid_t pid);

        static void registerExceptionHandler(std::uint32_t exceptionNumber, pid_t pid);

        static void ExceptionHandler(std::uint32_t interruptNumber, std::uint32_t errorCode);

        static void InterruptRequest(std::uint32_t interruptNumber);

    private:
        interruptManager() {;}
        ~interruptManager() {;}
};

#endif // _TOYOS_INTERRUPT_MANAGER_HPP_
