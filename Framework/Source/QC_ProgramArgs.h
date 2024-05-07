#pragma once

#include <map>

void ProgramArgsToPairs(unsigned int numArgs, char** argArr, std::map<std::string, const char*>& argumentPairs);
void OutputProgramPairsInfo(const std::map<std::string, const char*>& argumentPairs);
