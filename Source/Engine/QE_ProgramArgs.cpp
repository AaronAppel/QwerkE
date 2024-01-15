#include "QE_ProgramArgs.h"

#include "QF_FileUtilities.h"

#include "QE_Defines.h"

#ifdef WIN32
void ProgramArgsToPairs_Windows(unsigned int numArgs, char** argArr, std::map<const char*, const char*>& argumentPairs)
{
	argumentPairs.insert(std::pair<const char*, const char*>(key_StartupDir, argArr[0]));
	argumentPairs.insert(std::pair<const char*, const char*>(key_ApplicationFileName, FindFileName(argArr[0])));

	for (size_t i = 1; i < numArgs; ++i)
	{
		if (argArr[i][0] == '-')
		{
			if (argArr[i + 1])
			{
				argumentPairs.insert(std::pair<const char*, const char*>(argArr[i], argArr[i + 1]));
				++i;
			}
		}
	}
}

void OutputProgramPairsInfo_Windows(const std::map<const char*, const char*>& argumentPairs)
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
		printf("%s, \"%s\"\n", it->first, it->second);
	}
}
#endif
