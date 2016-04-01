/*
 a boot module containing servers, drivers and configuration information
 stored as a tar file
 The job of initrd is to make a single task and address space, parse
 the file until it finds the master server and initialise the master
 server.The master server will theuse IPC to map the initrd file into
 its address space while taking it out of the microkernel's heap.
*/
#include <string>
#include <toyOS/multiboot.h>

class initrd
{
    public:
        void init(multiboot_info_t*);
        bool isFilePresent(std::string name);
        std::size_t getFileLength(std::string name);
        void copyFileToMemory(std::string name, void* ptr);
        std::size_t getNumberOfFiles();
        std::string getName(std::size_t);
    private:
        initrd();
        void* moduleVirtualAddress;
};
