#pragma once

#include <map>
#include <string>

#define ProgramArgKey_ExePath "key_ExePath"

void ProgramArgsToPairs(unsigned int numArgs, char** argArr, std::map<std::string, const char*>& argumentPairs);
void OutputProgramPairsInfo(const std::map<std::string, const char*>& argumentPairs);
