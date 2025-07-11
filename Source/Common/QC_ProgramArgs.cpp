#include "QC_ProgramArgs.h"

#include <filesystem>
#include <map>
#include <string>

void ProgramArgsToPairs(unsigned int numArgs, char** argArr, std::map<std::string, const char*>& argumentPairs)
{
	argumentPairs.insert(std::pair<std::string, const char*>(ProgramArgKey_ExePath, argArr[0]));

	for (size_t i = 1; i < numArgs; ++i)
	{
		if (argArr[i][0] == '-' && argArr[i + 1])
		{
			argumentPairs.insert(std::pair<std::string, const char*>(argArr[i], argArr[i + 1]));
			++i;
		}
	}
}

void OutputProgramPairsInfo(const std::map<std::string, const char*>& argumentPairs)
{
	if (argumentPairs.find(ProgramArgKey_ExePath) != argumentPairs.end())
	{
		printf("Program .exe path: %s\n", argumentPairs.find(ProgramArgKey_ExePath)->second);
	}

	printf("\nNumber Of Arguments Passed: %lld", argumentPairs.size());
	printf("\nArguments Keys and Values:\n");
	for (auto it = argumentPairs.begin(); it != argumentPairs.end(); ++it)
	{
		printf("%s, \"%s\"\n", it->first.c_str(), it->second);
	}
}
