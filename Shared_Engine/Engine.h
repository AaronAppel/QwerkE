#ifndef _Engine_H_
#define _Engine_H_

// This may need to become an abstract class that gets inherited
// by platform specific implementations

class GLFWwindow;
class Editor;

class Engine
{
public:
	Engine();
	~Engine();

	eEngineMessage Startup();
	eEngineMessage TearDown();

	void Run();

	void NewFrame();
	void Update();
	void Draw();

	bool StillRunning() { return m_IsRunning; };

private:
	GLFWwindow* m_Window = nullptr;
	bool m_IsRunning = false;
    Editor* m_Editor;
};

#endif // !_Engine_H_
