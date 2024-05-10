#pragma once

namespace QwerkE {

	namespace CallBacks { // #TODO Capital B?

		// #TODO Review name starting with "CallBack"
		using FramebufferResizedCallback = std::function<void(int width, int height)>;
		using WindowResizedCallback = std::function<void(int width, int height)>;
		using KeyCallback = std::function<void(int key, int scancode, int action, int mode)>;

		using FileDropCallback = std::function<void(const char* const filePath)>;

	}

}
