#pragma once

// #NOTE "QF_PCH.h" already included as forced include in premake build file
// Cannot #include "QF_PCH.h" here. Must be done in VS project settings

// For convenience, include commonly used files, types, aliases, etc

#define _QIGNORED(x) (static_cast<void>(x))

#include "QC_CallbackFunction.h" // #TODO Force include? Seems more niche and should be included on a needs basis
#include "QC_Math.h"
#include "QC_TypeDefs.h"

#include "QF_Debug.h"
#include "QF_Log.h"
