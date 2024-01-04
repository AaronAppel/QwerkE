// It is required to add the following preprocessor defines to this project properties.
// Win32Bit/Win64Bit, and DEBUG/RELEASE

// Use the QwerkE_Framework
// Add a runtime editor that has extra functionality like:
// level editor, entity editor, shader editor, scripting, etc...

#include "PCH_Win32.h"

#include <map>

#include "../Engine_Defines.h"
#include "../Engine.h"

#include "QC_ProgramArgs.h"

int main(unsigned int argc, char **argv)
{
	// #TODO Move argument handling code into engine. Pass program args into QwerkE::Engine::Run(argc, argv);
	// std::cout << "-- Qwerk Engine 0.1 (VER_MACRO) --\n" << argc; // #TODO Define a version VER_MACRO

	printf("Program Name Is: %s", argv[0]);

	if (argc == 1)
	{
		printf("\nNo Extra Command Line Argument Passed Other Than Program Name\n");
	}
	else if (argc >= 2)
	{
		printf("\nNumber Of Arguments Passed: %d", argc);
		printf("\n----Following Are The Command Line Arguments Passed----");
		for (int counter = 0; counter < argc; counter++)
			printf("\nargv[%d]: %s", counter, argv[counter]);
	}

	std::map<const char*, const char*> pairs = ArgumentKeyValuePairs(argc, argv);
	for (auto it = pairs.begin(); it != pairs.end(); ++it)
	{
		printf("\nFound pair: %s, %s", it->first, it->second);
	}

	// Save argument key/value pairs for use in engine
	// Save environment variables info or references
	// Save working directory
	const QwerkE::eEngineMessage result = QwerkE::Engine::Run(pairs);
	if (result != QwerkE::eEngineMessage::_QSuccess)
	{
		system("pause");
	}

	return (int)result;
}