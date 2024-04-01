#pragma once

#ifdef _QDEBUG

#include "QF_Log.h"

#define BREAK __debugbreak();

#define ASSERT(x, msg)   \
if ((x)) { } else { LOG_CRITICAL("Assert! {0}, {1}, in {2}() in {3}({4})", #x, msg, __FUNCTION__, __FILE__, __LINE__); BREAK }

#else
#define BREAK			(void)0
#define ASSERT(x, msg)	(void)0
#endif
