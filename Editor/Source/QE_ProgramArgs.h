#pragma once

#ifdef _QWINDOWS
#include <map>

void ProgramArgsToPairs_Windows(unsigned int numArgs, char** argArr, std::map<std::string, const char*>& argumentPairs);
void OutputProgramPairsInfo_Windows(const std::map<std::string, const char*>& argumentPairs);
#else
#pragma warning Support platform!
#endif
