#include "QC_HelpersTime.h"

#include "QF_Platform.h"

#ifdef _QWindows
#include <profileapi.h>
#include <winnt.h>
double QC_HelpersTime()
{
	unsigned __int64 freq;
	unsigned __int64 time;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&time);

	double timeseconds = (double)time / freq;

	return timeseconds;
}
#else
double QC_HelpersTime() { return 0.f; }
#endif