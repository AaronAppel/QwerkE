#pragma once

#include "QF_Instrumentor.h"

#define QPROFILING 1

#if QPROFILING
#define PROFILE_SCOPE(scopeName) InstrumentationTimer timer##__LINE__(scopeName)
// NOTE: Could use __FUNCSIG__ to show args for overloaded function, but it is more info than needed currently
#define PROFILE_FUNC() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(scopeName)
#define PROFILE_FUNC(scopeName)
#endif