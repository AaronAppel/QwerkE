#pragma once

// #TODO Include paths for external/user use

#include "QF_Enums.h"

namespace QwerkE {

	namespace Framework {

		struct StartUpArguments
		{
			StartUpArguments(unsigned int argc, char** argv, uint16_t windowWidth, uint16_t windowHeight) :
				argc(argc), argv(argv), windowStartingWidth(windowWidth), windowStartingHeight(windowHeight)
			{ }

			unsigned int argc = 0; // Command line arguments
			char** argv = nullptr;

			uint16_t windowStartingWidth = 1920; // Window
			uint16_t windowStartingHeight = 1080;
		};

		eOperationResult Initialize(const StartUpArguments& startUpArgs);
		eOperationResult Shutdown();

		void StartFrame();
		void Update(float deltatime);
		void RenderView(uint16_t viewId = 0);
		void EndFrame();

		std::map<std::string, const char*>& GetProgramArgumentPairs();

	}

}