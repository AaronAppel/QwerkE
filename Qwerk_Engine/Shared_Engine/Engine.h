#ifndef _Engine_H_
#define _Engine_H_

#include "../QwerkE_Framework/Headers/QwerkE_Include.h"

// Try to keep this class properly abstracted so it
// can perform on multiple platforms without needing
// inherited subclasses.

#include "Engine_Defines.h"
#include "Engine_Enums.h" // eEngineMessage

// If only 1 engine should exist at a time then it could be wrapped in
// the QwerkE namespace and have static functions like a singleton or
// service. Or just find another way to prevent multiple instances
// while offering a nice interface.

// What engine functionality should be user accessible?

namespace QwerkE {

    class Editor;
    class Scenes;
    class Window;

	namespace Engine
	{
		void Run(std::map<const char*, const char*> &args);
		void Stop();

		void NewFrame();
		void PollInput();
		void Update(double deltatime);
		void Draw();

		bool StillRunning();
	}

}
#endif // !_Engine_H_
