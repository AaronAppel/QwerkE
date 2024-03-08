#pragma once

#include <string>

#include "Libraries/glew/glew.h"

#include "QF_Enums.h"

namespace QwerkE {

    struct MaterialComponent
    {
        MaterialComponent() = default;
        MaterialComponent(std::string name, GLuint handle) :
            s_TextureName(name),
            s_Handle(handle) {} // #TODO Review formatting

        eMaterialMaps s_Type = eMaterialMaps::MatMap_Null;

        std::string s_TextureName = Constants::gc_DefaultStringValue;

        GLuint s_Handle = 0;
    };

}
