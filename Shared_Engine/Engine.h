#ifndef _Engine_H_
#define _Engine_H_

// This may need to become an abstract class that gets inherited
// by platform specific implementations

class Engine
{
public:
	Engine();
	~Engine();

	eEngineMessage Startup();

	void Run();

	void NewFrame();
	void Update();
	void Draw();

	bool StillRunning() { return m_IsRunning; };

private:
	bool m_IsRunning = false;
};

#endif // !_Engine_H_
