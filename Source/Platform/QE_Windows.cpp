
#include "QE_PCH_Windows.h"

#include <map>
#include <memory>

#include "QE_ProgramArgs.h"

#include "QE_Defines.h"
#include "QE_Engine.h"

int main(unsigned int argc, char **argv)
{
	std::map<const char*, const char*> pairs;
	ProgramArgsToPairs_Windows(argc, argv, pairs);

	pairs.insert(std::pair<const char*, const char*>(key_ProjectName, "Project1"));
	pairs.insert(std::pair<const char*, const char*>(key_UserName, "User1"));

	if (true) { OutputProgramPairsInfo_Windows(pairs); }

	const QwerkE::eEngineMessage result = QwerkE::Engine::Run(pairs);
	if (result != QwerkE::eEngineMessage::_QSuccess)
	{
		system("pause");
	}

	return (int)result;
}