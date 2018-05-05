#ifndef _Engine_H_
#define _Engine_H_

// Try to keep this class properly abstracted so it
// can perform on multiple platforms without needing
// inherited subclasses.

#include "Engine_Enums.h" // eEngineMessage

class Editor;

// If only 1 engine should exist at a time then it could be wrapped in
// the QwerkE namespace and have static functions like a singleton or
// service. Or just find a nother way to prevent multiple instances
// while offering a nice interface.

class Engine
{
public: // External API
	Engine();
	~Engine();

	eEngineMessage Startup();
	eEngineMessage TearDown();

	void Run();

	void NewFrame();
	void Input();
	void Update(double deltatime);
	void Draw();

	bool StillRunning() { return m_IsRunning; };

private:
	// Framework*
	bool m_IsRunning = false;
    Editor* m_Editor = nullptr;
};

#endif // !_Engine_H_
