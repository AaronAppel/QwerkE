#include "QF_Debug.h"

// #ifdef _QDebug
#if _DEBUG

// void PrintCliackableOutputWindowError(const char* message...) {} // #TODO Create a formatted output message that's clickable in Visual Studio

// https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c

bool AnArgumentWasNull(int n_args, ...)
{
    va_list ap;
    va_start(ap, n_args);
    int max = va_arg(ap, int);
    for (int i = 0; i < n_args; i++) {
        int arg = va_arg(ap, int);
        if (!arg)
        {
            LOG_ERROR("Invalid/null argument found!");
            return true;
        }
    }
    va_end(ap);
	return false;
}

#endif