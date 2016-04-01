#ifndef _UTIL_SPINLOCK_HPP_
#define _UTIL_SPINLOCK_HPP_

class spinlock
{
    public:
        spinlock();
        ~spinlock();
        void lock();
        void unlock();
    private:
        bool state;
};

#endif // _UTIL_SPINLOCK_HPP_
