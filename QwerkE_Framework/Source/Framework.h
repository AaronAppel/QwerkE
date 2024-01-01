#pragma once

#include <string>

#include "Headers/QwerkE_Enums.h" // eEngineMessage

namespace QwerkE {

	namespace Framework
	{
		eEngineMessage Startup();
		eEngineMessage Startup(std::string configFilePath, std::uint_fast8_t flags);
		eEngineMessage TearDown();

		void Run();
		void Stop();

		void NewFrame();
		void PollInput();
		void Update(double deltatime);
		void Draw();
		void EndFrame();

		bool StillRunning();
	}
}