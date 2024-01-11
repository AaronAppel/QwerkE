#pragma once

#include <string>

#include "QF_Enums.h"

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
