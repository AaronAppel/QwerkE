#include "QF_ShaderComponent.h"

#include "QF_FileUtilities.h"
#include "QF_Log.h"

#include "QF_ShaderFactory.h"

namespace QwerkE {

    ShaderComponent::ShaderComponent(const char* shaderPath)
    {
        if (FileExists(shaderPath))
        {
            m_Name = GetFileNameWithExt(shaderPath);
            m_Type = GetFileExtension(shaderPath);

            if (m_Type == "vert") // TODO: Make an enum for this type
            {
                m_ShaderHandle = ShaderFactory::CreateVertexShader(shaderPath);
            }
            else if (m_Type == "frag")
            {
                m_ShaderHandle = ShaderFactory::CreateFragmentShader(shaderPath);
            }
            else if (m_Type == "geo")
            {
                m_ShaderHandle = ShaderFactory::CreateGeometryShader(shaderPath);
            }
            else
            {
                m_ShaderHandle = 0;
            }
        }

        if (!m_ShaderHandle)
        {
            LOG_ERROR("Error compiling Shader Component: {0}", shaderPath);
        }
    }

    ShaderComponent::~ShaderComponent()
    {
        glDeleteShader(m_ShaderHandle); // #TODO Verify if allocated
        delete[] m_ShaderData; // #TODO Verify if allocated
    }

}
