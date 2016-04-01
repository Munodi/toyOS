#ifndef _TOYOS_PROCESS_HPP_
#define _TOYOS_PROCESS_HPP_

#include <toyOS/virtualAddressSpace.hpp>
#include <sys/types.h>
#include <toyOS/essentialServers.hpp>
#include <queue>
#include <toyOS/registers.hpp>

enum microkernelProgramType {
    server,
    driver,
    application
};

class processControlBlock
{
    public:
        enum state{
            loading,    // when not started yet
            wainingForChild,
            waitingForPageToBeSwappedIn,
            waitingForMessage,
            handlingPageFault,
            runnable,
            running
        };

        /* these functions are used by the PM for implementing syscalls
            eg fork calls create which creates a process with a unique pid,
                and a state of loading, it then uses messages the MM which
                sets up a virtualAddressSpace COW mapped to the frames of
                the process which calls fork. The PM


        */
        static pid_t create();                  // called by pm
        static void incrementPriority(pid_t);   // called by pm
        static void decrementPriority(pid_t);   // called by pm
        static virtualAddressSpace getVirtualAddressSpace();   // called by pm
        static void setState(pid_t, processControlBlock::state);   // called by pm

    private:
        std::int_fast8_t priority
        int mode;
        microkernelProgramType programType;
        pid_t pid;
        gid_t gid;
        uid_t uid;
        pid_t ppid;
        virtualAddressSpace::addressSpaceHandle addressSpace;
        std::queue<message> messageQueue;
        static std::vector<processControlBlock> allProcesses;
        pid_t choosePid();          // decides a pid for a new process
};

#endif // _TOYOS_PROCESS_HPP_
