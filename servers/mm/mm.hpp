#ifndef _MM_HPP_
#define _MM_HPP_

void* smallMapPhysicalAddressToSharedMemorySection(physical_uintptr_t);

void  smallUnmapPhysicalAddressToSharedMemorySection(void*);

#endif // _MM_HPP_
