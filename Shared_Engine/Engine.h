#ifndef _Engine_H_
#define _Engine_H_

// This may need to become an abstract class that gets inherited
// by platform specific implementations

#include "Engine_Enums.h"

struct GLFWwindow;
class Editor;
class SceneManager;

class Engine
{
public:
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
	GLFWwindow* m_Window = nullptr;
	bool m_IsRunning = false;
	SceneManager* m_SceneManager = nullptr;
    Editor* m_Editor = nullptr;
};

#endif // !_Engine_H_