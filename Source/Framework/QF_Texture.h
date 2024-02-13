#pragma once

#include <string>

#include "Libraries/glew/GL/glew.h"

#include "QF_Constants.h"

namespace QwerkE {

    struct Texture
    {
        Texture() = default;

#ifdef OpenGL
        Texture(GLuint handle) : s_Handle(handle) {}
        GLuint s_Handle = 0;
#else
#pragma warning "Define graphics library!"
#endif

        void Initialize();

        std::string s_FileName = gc_DefaultStringValue;
    };

}
