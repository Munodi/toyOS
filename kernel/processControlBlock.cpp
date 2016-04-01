/*#include <toyOS/processControlBlock.hpp>
#include <toyOS/essentialServers.hpp>

pid_t processControlBlock::choosePid()
{
    static pid = 1;
    std::size_t i = 0;
    for(;;)
    {
        for(i = 0; i < processControlBlock::allProcesses; i++)
        {
            if(processControlBlock::allProcesses[i].pid == pid)
            {
                ++pid;
                break;
            }
            return pid;
        }
    }
}
*/
