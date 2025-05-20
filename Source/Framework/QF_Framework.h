#pragma once

#include <map>
#include <stdint.h>
#include <string>

// #TODO Include paths for external/user use

#include "QF_Enums.h"

namespace QwerkE {

	namespace Framework {

		// #NOTE Set CLI args 1st. Order dependency with QF_Paths
		void SetCommandLineArgs(unsigned int numberOfArguments, char** commandLineArguments);

		eOperationResult Initialize(uint16_t windowWidth = 1920, uint16_t windowHeight = 1080);

		eOperationResult Shutdown();

		void StartFrame();
		void Update(float deltatime);
		void RenderView(uint16_t viewId = 0);
		void EndFrame();

		const std::map<std::string, const char*>& GetProgramArgumentPairs();

	}

}