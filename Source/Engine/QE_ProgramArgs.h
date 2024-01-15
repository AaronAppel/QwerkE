#pragma once

#include <map>

#include "QF_Platform.h"

#ifdef WIN32
void ProgramArgsToPairs_Windows(unsigned int numArgs, char** argArr, std::map<const char*, const char*>& argumentPairs);
void OutputProgramPairsInfo_Windows(const std::map<const char*, const char*>& argumentPairs);
#else
#pragma warning Support platform!
#endif
