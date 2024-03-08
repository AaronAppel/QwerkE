#pragma once

#include <string>

#ifdef GLEW
#include "Libraries/glew/glew.h"
#endif


#include "QF_Constants.h"

namespace QwerkE {

    struct Texture
    {
        Texture() = default;

#if defined(_QOPENGL) && defined (_QGLEW)
        Texture(GLuint handle) : s_Handle(handle) {}
        GLuint s_Handle = 0;
#else
#pragma warning "Define graphics library!"
#endif

        void Initialize();

        std::string s_FileName = Constants::gc_DefaultStringValue;
    };

}
