#include "QF_PrintFunctions.h"

// #TODO Deprecate file. Maybe move methods to a QC_* file, or extend to a more fully featured logging utility file

#include "QF_Defines.h"

// #TODO Wrap in #ifdefs

// #include <cstdio> // vsnprintf_s
#include <fstream> // vsnprintf_s
#include <cstdarg> // va_start, va_end
#include <debugapi.h> // OuputDebugStringA(buffer)

void OutputPrint(const char* message, ...)
{
#define MAX_MESSAGE 1024
	char buffer[MAX_MESSAGE];
	va_list arg;
	va_start(arg, message);
	vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, message, arg);
	va_end(arg);

	// vsnprintf_s might do this, but docs are unclear
	buffer[MAX_MESSAGE - 1] = 0; // #TODO Should this be '\0'?
	OutputDebugStringA(buffer);
}

void ConsolePrint(const char* message)
{
	fprintf(stderr, message);
}
