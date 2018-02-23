#include "pch.h"

// OpenGLHelpers.cpp
void CheckforGLErrors()
{
	GLenum error = glGetError();
	if (error != 0)
		OutputMessage("\nglGetError: %i-> ", error);
	if (error == GL_INVALID_VALUE)
		OutputMessage("Invalid value used.", error);
	if (error == GL_INVALID_OPERATION)
		OutputMessage("Invalid operation.", error);
}

GLuint LoadTexture(const char* filename)
{
	unsigned char* pngbuffer;
	unsigned int width, height;
	long filesize;
	unsigned char* filebuffer = (unsigned char*)LoadCompleteFile(filename, &filesize);
	unsigned int result = lodepng_decode32(&pngbuffer, &width, &height, filebuffer, filesize);
	assert(result == 0);

	Flip32BitImageVertically(pngbuffer, width, height);

	GLuint texhandle = 0;
	glGenTextures(1, &texhandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texhandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	// cleanup
	free(pngbuffer);
	delete[] filebuffer;
	return texhandle;
}