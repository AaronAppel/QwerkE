#include "QF_ShaderProgram.h"

#include "QF_Log.h"

#include "QF_ShaderFactory.h"
#include "QF_ShaderComponent.h"
#include "QF_ShaderUtilities.h"

namespace QwerkE {

    void ShaderProgram::FindAttributesAndUniforms()
    {
        FindShaderUniformsAndAttributesInComponentStrings(this, m_Attributes, m_Uniforms);
    }

    void ShaderProgram::RecompileShaderType(GLenum type, const char* shaderData)
    {
        // #TODO Deallocate or re-use existing allocation(s)

        ShaderComponent* result = ShaderFactory::GenerateShaderFromData(type, shaderData);

        if (result)
        {
            // #TODO Handle old shader value
            switch (type)
            {
            case GL_VERTEX_SHADER:
                m_VertShader->DestroyShaderData(); // Cleanup old shader data
                result->SetName(m_VertShader->GetName());
                m_VertShader = result;
                break;
            case GL_FRAGMENT_SHADER:
                m_FragShader->DestroyShaderData(); // Cleanup old shader data
                result->SetName(m_FragShader->GetName());
                m_FragShader = result;
                break;
            case GL_GEOMETRY_SHADER:
                m_GeoShader->DestroyShaderData(); // Cleanup old shader data
                result->SetName(m_GeoShader->GetName());
                m_GeoShader = result;
                break;
            }

            ShaderFactory::LinkCreatedShaderProgram(this);

            FindAttributesAndUniforms();

            LOG_INFO("Recompiled shader {0}", m_VertShader->GetName());
        }
    }

    // void SetAttributeData() {} good idea?

    // int
    void ShaderProgram::SetUniformInt1(const char* name, int value)
    {
        glUniform1i(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value);
    }
    void ShaderProgram::SetUniformInt2(const char* name, int value1, int value2)
    {
        glUniform2i(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value1, value2);
    }
    void ShaderProgram::SetUniformInt3(const char* name, int value1, int value2, int value3)
    {
        glUniform3i(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value1, value2, value3);
    }
    void ShaderProgram::SetUniformInt4(const char* name, int value1, int value2, int value3, int value4)
    {
        glUniform4i(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value1, value2, value3, value4);
    }

    // float
    void ShaderProgram::SetUniformFloat1(const char* name, float value)
    {
        glUniform1f(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value);
    }
    void ShaderProgram::SetUniformFloat2(const char* name, float value1, float value2)
    {
        glUniform2f(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value1, value2);
    }
    void ShaderProgram::SetUniformFloat3(const char* name, float value1, float value2, float value3)
    {
        glUniform3f(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value1, value2, value3);
    }
    void ShaderProgram::SetUniformFloat4(const char* name, float value1, float value2, float value3, float value4)
    {
        glUniform4f(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), value1, value2, value3, value4);
    }

    // Matrix
    void ShaderProgram::SetUniformMat4(const char* name, const GLfloat* matrix)
    {
        // #TODO Improve uniform prefixing
        glUniformMatrix4fv(glGetUniformLocation(m_ProgramHandle, DispStrCombine(ShaderFactory::GetUniformPrefix(), name).c_str()), 1, GL_FALSE, matrix);
    }

}
