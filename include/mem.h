#ifndef MEM_H
#define MEM_H
#include <system.h>

int memcmp(const void* aptr, const void* bptr, size_t size);
void* malloc(size_t size);
uint32_t* GetNextAddr();
#endif