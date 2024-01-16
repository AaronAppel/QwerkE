#include "QC_Memory.h"

void* operator new (size_t sizeBytes)
{
	return malloc(sizeBytes);
}

void operator delete (void* memoryLocation)
{
	return free(memoryLocation);
}