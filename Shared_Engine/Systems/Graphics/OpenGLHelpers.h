#ifndef _OpenGLHelpers_H_
#define _OpenGLHelpers_H_


void CheckforGLErrors();
void CheckforGLErrors(char* file, int line);
void CheckforGLErrors(const char* file, int line);

GLuint Load2DTexture(const char* filename, bool flipVertically = 0);
GLuint LoadCubeMapTexture(const char* filename);

char* blue_GetAttributePrefix();
char* blue_GetUniformPrefix();
char* blue_GetTransferPrefix();

#endif // !_OpenGLHelpers_H_