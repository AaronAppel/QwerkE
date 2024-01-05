#pragma once

#include <string>

#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h" // eEngineMessage

namespace QwerkE {

	namespace Framework
	{
		eEngineMessage Startup();
		eEngineMessage Startup(const std::string configFilePath);
		eEngineMessage TearDown();

		void Run();
		void Stop();

		void NewFrame();
		void PollInput();
		void Update(double deltatime);
		void Draw();

		bool StillRunning();
	}
}
