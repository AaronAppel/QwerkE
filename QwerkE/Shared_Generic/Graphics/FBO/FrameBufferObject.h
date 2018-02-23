#ifndef __FrameBufferObject_H__
#define __FrameBufferObject_H__

class FrameBufferObject
{
public:
	FrameBufferObject();
	~FrameBufferObject();

	void Init();
	void Bind();
	void UnBind();

	void Reset() { textureColorbuffer = 0; };

	GLuint GetTextureID() { return textureColorbuffer; };

private:
	GLuint framebuffer;
	GLuint textureColorbuffer;
	GLuint rbo;
};

#endif //__FrameBufferObject_H__
