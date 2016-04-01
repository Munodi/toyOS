#include <toyOS/kernelLock.hpp>

std::size_t volatile kernelLock::slock;

void kernelLock::lock()
{
    while(__sync_lock_test_and_set(&kernelLock::slock, 1) != 0)
    {
        while(kernelLock::slock != 0)
            ;
    }
}

void kernelLock::unlock()
{
    kernelLock::slock = 0;
}
