#pragma once

#include <string>

#include "Libraries/glew/GL/glew.h"

#include "QF_QwerkE_Enums.h"

namespace QwerkE {

    struct MaterialComponent
    {
        MaterialComponent() = default;
        MaterialComponent(std::string name, GLuint handle) :
            s_TextureName(name),
            s_Handle(handle) {} // #TODO Review formatting

        eMaterialMaps s_Type = eMaterialMaps::MatMap_Null;

        std::string s_TextureName = gc_DefaultStringValue;

        GLuint s_Handle = 0;
    };

}
