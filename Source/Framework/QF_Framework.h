#pragma once

#include <string>

#include "QF_QwerkE_Enums.h" // eEngineMessage

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
		void Update(float deltatime);
		void Draw();

		bool StillRunning();
	}
}
