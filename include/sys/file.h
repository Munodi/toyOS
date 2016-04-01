#ifndef _SYS_FILE_H_
#define _SYS_FILE_H_

#include <sys/vnode.h>

struct file
{
    uint64_t offset;
};

#endif // _SYS_FILE_H_
