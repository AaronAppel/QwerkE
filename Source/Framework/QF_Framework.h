#pragma once

#include <string>

#include "QF_Enums.h"

namespace QwerkE {

	namespace Framework {

		eOperationResult Startup();
		eOperationResult Startup(const std::string configFilePath);
		eOperationResult TearDown();

		void Run();
		void Stop();

		void NewFrame();
		void Update(float deltatime);
		void Draw();

		bool StillRunning();
	}
}
