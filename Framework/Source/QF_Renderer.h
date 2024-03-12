#pragma once

namespace QwerkE {

	namespace Renderer {

		void Initialize();
		void EndFrame();
		void Shutdown();

		int RunBgfxExample(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height);

	}

}