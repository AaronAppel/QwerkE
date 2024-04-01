#ifndef __MyMemory_H__
#define __MyMemory_H__

// Modified by Aaron Appel

// TODO:: add creation and deletion amount tracking
// simple num++ would work

// #include "QF_Platform.h"
// #include "QF_PathDefines.h"

#if defined(_Q32Bit) && QWERKE_TRACKING_ALLOCATIONS

void MyMemory_ValidateAllocations(bool AssertOnAnyAllocation);

void* operator new(size_t size, char* file, unsigned long line);
void* operator new[](size_t size, char* file, unsigned long line);
void operator delete(void* m, char* file, unsigned long line);
void operator delete[](void* m, char* file, unsigned long line);

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* m);
void operator delete[](void* m);

#define new new(__FILE__, __LINE__)

#endif // WIN32

#endif //__MyMemory_H__
