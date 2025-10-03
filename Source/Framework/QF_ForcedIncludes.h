#pragma once

// #NOTE "QF_PCH.h" already included as forced include in premake build file
// Cannot #include "QF_PCH.h" here. Must be done in VS project settings

// For convenience, include commonly used files, types, aliases, etc

#define _QIGNORED(x) (static_cast<void>(x))

#include "QF_CallbackFunction.h" // #TODO Force include? Seems more niche and should be included on a needs basis
#include "QF_Debug.h"
#include "QF_Math.h"
#include "QF_Log.h"
#include "QF_TypeDefs.h"
