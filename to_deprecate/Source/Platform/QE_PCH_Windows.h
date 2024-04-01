#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#include <string>
#include <stdio.h>
#include <map>
#include <vector>

// #NOTE Include order is Engine, then Framework, then Common

#include "QE_Defines.h"

#include "QF_Platform.h"
#include "QF_LibrariesInclude.h"
#include "QF_LibrariesLoad.h" // #TODO Review include ordering. This avoids library LNK2019 errors

// NOTE: Add additional .h files below, if they rarely change

// #include "QF_PCH_Includes.h"
#include "QC_TypeDefs.h"