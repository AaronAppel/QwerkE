#pragma once

#include <string>

#include "Libraries/glew/GL/glew.h"

#include "QF_Constants.h"

namespace QwerkE {

    struct Texture
    {
    public:

        Texture() {}
        ~Texture() {}

#ifdef OpenGL
        Texture(GLuint handle) : s_Handle(handle) {}
        GLuint s_Handle = 0;
#else
        unsigned int s_Handle = 0;
#endif

        std::string s_FileName = gc_DefaultStringValue;
    };

}
