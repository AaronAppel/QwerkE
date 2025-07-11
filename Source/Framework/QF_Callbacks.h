#pragma once

#include <functional>

namespace QwerkE {

	namespace Callbacks {

		// #TODO Review names starting with "CallBack"
		using FramebufferResizedCallback = std::function<void(int width, int height)>;
		using WindowResizedCallback = std::function<void(int width, int height)>;
		using KeyCallback = std::function<void(int key, int scancode, int action, int mode)>;

		using FileDropCallback = std::function<void(const char* const filePath)>;

	}

}
