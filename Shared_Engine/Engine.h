#ifndef _Engine_H_
#define _Engine_H_

// Try to keep this class properly abstracted so it
// can perform on multiple platforms without needing
// inherited subclasses.

#include "Engine_Enums.h" // eEngineMessage

class Editor;
class SceneManager;
class Window;

// If only 1 engine should exist at a time then it could be wrapped in
// the QwerkE namespace and have static functions like a singleton or
// service. Or just find another way to prevent multiple instances
// while offering a nice interface.

// What engine functionality should be user accessible?

class Engine
{
public:
	Engine();
	~Engine();

	void Run();
	void Stop() {} // TODO:

	void NewFrame();
	void Input();
	void Update(double deltatime);
	void Draw();

	bool StillRunning() { return m_IsRunning; };

private:
	// Framework*
	Window* m_Window = nullptr;
	SceneManager* m_SceneManager = nullptr;
	bool m_IsRunning = false;
    Editor* m_Editor = nullptr;
};

#endif // !_Engine_H_
