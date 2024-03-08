#pragma once

#include <string>

#include "QF_QwerkE_Global_Constants.h"

#include "QF_Asset.h"
#include "QC_Vector.h"

typedef unsigned int GLuint;

namespace QwerkE {

    // Might want to limit how tags are created.
    // They should probably just be created by the resource manager.
    class Asset_Tag
    {
    public:
        std::string GetName() { return m_Name; }

    private:
        std::string m_Name = gc_DefaultStringValue;
        vec3 m_TagColor = vec3(128, 128, 128);
        GLuint m_ThumbnailHandle = 0;
    };

}
