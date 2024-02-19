#include "QF_ShaderComponent.h"

#include "QF_FileUtilities.h"
#include "QF_Log.h"

#include "QF_ShaderFactory.h"

namespace QwerkE {

    ShaderComponent::ShaderComponent(const char* shaderPath)
    {
        if (FileExists(shaderPath))
        {
            m_Name = FileName(shaderPath, true);
            m_Type = SmartFileExtension(shaderPath).get();

            if (m_Type == "vert") // TODO: Make an enum for this type
            {
                m_ShaderHandle = ShaderFactory::CreateVertexShader(shaderPath);
                m_GLShaderType = GL_VERTEX_SHADER;
            }
            else if (m_Type == "frag")
            {
                m_ShaderHandle = ShaderFactory::CreateFragmentShader(shaderPath);
                m_GLShaderType = GL_FRAGMENT_SHADER;
            }
            else if (m_Type == "geo")
            {
                m_ShaderHandle = ShaderFactory::CreateGeometryShader(shaderPath);
                m_GLShaderType = GL_GEOMETRY_SHADER;
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
        DestroyShaderData();
    }

    void ShaderComponent::DestroyShaderData()
    {
        if (m_ShaderData)
        {
            delete[] m_ShaderData; // #TODO Verify if allocated
        }
    }

}
