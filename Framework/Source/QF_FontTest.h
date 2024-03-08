#pragma once

#include <string>

#ifdef _QOPENGL
typedef float GLfloat;
#endif

namespace QwerkE {

    class ShaderProgram;

    void LoadFonts();

#ifdef _QOPENGL
    void RenderText(ShaderProgram* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);
#else
#error "Define graphics library!"
#endif

}
