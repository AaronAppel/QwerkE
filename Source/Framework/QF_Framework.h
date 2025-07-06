#pragma once

#include <map>
#include <stdint.h>
#include <string>

// #TODO Include paths for external/user use

#include "QF_Enums.h"

namespace QwerkE {

	namespace Framework {

		struct StartUpData
		{
			u16 windowWidth = 1920;
			u16 windowHeight = 1080;

			u16 windowOpenPositionX = 200;
			u16 windowOpenPositionY = 200;

			bool vSyncEnabled = false;
			bool windowAutoFocusOnStart = false;
		};

		// #NOTE Set CLI args 1st. Order dependency with QF_Paths
		void SetCommandLineArgs(unsigned int numberOfArguments, char** commandLineArguments);

		eOperationResult Initialize(const StartUpData& startUpData);

		eOperationResult Shutdown();

		void StartFrame();
		void Update(float deltatime);
		void RenderView(uint16_t viewId = 0);
		void EndFrame();

		const std::map<std::string, const char*>& GetProgramArgumentPairs();

	}

}
