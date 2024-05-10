#pragma once

// #TODO Include paths for external/user use

#include "QF_Enums.h"

namespace QwerkE {

	namespace Framework {

		eOperationResult Initialize(unsigned int argc, char** argv);
		eOperationResult Shutdown();

		void StartFrame();
		void Update(float deltatime);
		void RenderView(uint16_t viewId = 0);
		void EndFrame();

		std::map<std::string, const char*>& GetProgramArgumentPairs();

	}

}