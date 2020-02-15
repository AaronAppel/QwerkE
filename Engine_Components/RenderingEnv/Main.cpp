#include "Main.h"

// Reference: https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/

#include<stdio.h>

#include "../QwerkE_Framework/Systems/Misc/ProgramArgs.h"

int main(unsigned int argc, char **argv)
{
	// TODO: Move argument handling code into engine
	// Greeting
	// std::cout << "-- Qwerk Engine 0.1 --\n" << argc;

	unsigned int counter;
	printf("Program Name Is: %s", argv[0]);
	if (argc == 1)
		printf("\nNo Extra Command Line Argument Passed Other Than Program Name\n");
	if (argc >= 2)
	{
		printf("\nNumber Of Arguments Passed: %d", argc);
		printf("\n----Following Are The Command Line Arguments Passed----");
		for (counter = 0; counter < argc; counter++)
			printf("\nargv[%d]: %s", counter, argv[counter]);
	}

	// Look for key arguments
	/*if (ArgumentExists(argc, argv, key_ProjectName))
	{
		printf("\nFound key argument: %s", key_ProjectName);
	}*/

	// Look for key/value argument pairs
	std::map<const char*, const char*> pairs = ArgumentKeyValuePairs(argc, argv);
	for (auto it = pairs.begin(); it != pairs.end(); ++it)
	{
		printf("\nFound pair: %s, %s", it->first, it->second);
	}

	// Save argument key/value pairs for use in engine
	// Save environment variables info or references
	// Save working directory

#ifdef Qwerk_Engine
    QwerkE::Engine::Run(pairs);
#else
    QwerkE::Framework::Startup();

    QwerkE::Framework::Run();

    QwerkE::Framework::TearDown();
#endif // Qwerk_Engine

    return 0;
}
