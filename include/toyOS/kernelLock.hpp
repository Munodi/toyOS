#ifndef _TOYOS_LOCK_MANAGER_HPP
#define _TOYOS_LOCK_MANAGER_HPP

#include <cstdint>
#include <cstddef>

class kernelLock
{
    public:
        static void lock();
        static void unlock();
        static std::uint_fast8_t tryLock();

    private:
        static volatile std::size_t slock;
        kernelLock() {;}   // do nothing
};


#endif // _TOYOS_LOCK_MANAGER_HPP
