#pragma once

#include "QF_Enums.h"

namespace QwerkE {

	namespace Renderer {

		eOperationResult Initialize();
		void EndFrame();
		void Shutdown();

		int RunBgfxExample(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height);

	}

}