#include "QE_ProgramArgs.h"

#include <filesystem>
#include <map>
#include <string>

void ProgramArgsToPairs(unsigned int numArgs, char** argArr, std::map<std::string, const char*>& argumentPairs)
{
	argumentPairs.insert(std::pair<std::string, const char*>(key_StartupDir, argArr[0]));

	for (size_t i = 1; i < numArgs; ++i)
	{
		if (argArr[i][0] == '-')
		{
			if (argArr[i + 1])
			{
				argumentPairs.insert(std::pair<std::string, const char*>(argArr[i], argArr[i + 1]));
				++i;
			}
		}
	}
}

void OutputProgramPairsInfo(const std::map<std::string, const char*>& argumentPairs)
{
	if (argumentPairs.find(key_StartupDir) != argumentPairs.end())
	{
		printf("Program startup path: %s\n", argumentPairs.find(key_StartupDir)->second);
	}

	if (argumentPairs.find(key_ApplicationFileName) != argumentPairs.end())
	{
		printf("Program application name: %s\n", argumentPairs.find(key_ApplicationFileName)->second);
	}

	printf("\nNumber Of Arguments Passed: %d", argumentPairs.size());
	printf("\nCommand Line Arguments Passed :\n");
	for (auto it = argumentPairs.begin(); it != argumentPairs.end(); ++it)
	{
		printf("%s, \"%s\"\n", it->first.c_str(), it->second);
	}
}
