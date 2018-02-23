#ifndef __OpenGLManager_H__
#define __OpenGLManager_H__

// graphics
#define Wind_CCW 1 // For invert faces

class OpenGLManager
{
public:
	OpenGLManager();
	~OpenGLManager();

	void Init();
	void ResetGLState();
	void ViewPortSize(int x, int y, int width, int height);
	void ClearScreen();
	void SetZBufferLock(bool lock);
	void SetVSynchON(bool VSynchIsON) { m_VSync = VSynchIsON; };  // TODO: Handle VSynch Toggling

	void SetClearColor(vec4 color) { m_ClearColor = color; };

private:
	vec4 m_ClearColor = vec4(0.2f, 0.3f, 0.4f, 1);

	bool m_DepthTestingOn = true;
	bool m_BackfaceCullingOn = true;
	bool m_BlendingOn = true;
    bool m_VSync = true; // TODO: Handle VSync Toggling
};


#endif //__OpenGLManager_H__
