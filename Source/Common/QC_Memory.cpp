#include "QC_Memory.h"

#include "QF_Defines.h"

#if QWERKE_TRACKING_ALLOCATIONS && 0 // Using "Libraries/FlatheadGames/MyMemory.h" instead

// #TODO Find a way to determine when static allocations are happening,
// and when all static allocations have been performed. Could have a
// function call or special object allocation that sets a flag to know
// that dynamic allocations are starting. This might be useful info when
// looking through allocation data. Maybe have a type value for static/dynamic.

// #TODO Track allocation information like file, function, and line info to
// track down unallocated resources. Enforce the use of new allocators that
// require this info and redirect all calls through it, while tracking is enabled.

void* operator new (size_t sizeBytes)
{
	return malloc(sizeBytes);
}

void operator delete (void* memoryLocation)
{
	return free(memoryLocation);
}
#endif
