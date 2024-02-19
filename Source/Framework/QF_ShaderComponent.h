#pragma once

#include <string>

#include "QF_Constants.h"
;
typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace QwerkE {

    class ShaderComponent
    {
    public:
        ShaderComponent() = default;
        ShaderComponent(const char* shaderPath);
        ~ShaderComponent();

        std::string GetName() const { return m_Name; }
        std::string GetType() const { return m_Type; }
        GLenum GetGLShaderType() const { return m_GLShaderType; }
        GLuint GetHandle() const { return m_ShaderHandle; }
        const char* GetStringData() const { return m_ShaderData; }

        void SetName(std::string name) { m_Name = name; }
        void SetType(std::string name) { m_Type = name; }
        void SetGLShaderType(GLenum glType) { m_GLShaderType = glType; }
        void SetHandle(GLuint handle) { m_ShaderHandle = handle; }
        void SetStringData(const char* shaderData) { m_ShaderData = shaderData; }

        void DestroyShaderData();

    private:
        std::string m_Name = gc_DefaultStringValue;
        std::string m_Type = gc_DefaultStringValue; // TODO: Think of making an enum for shader component type, or use GLenum
        GLenum m_GLShaderType = 0;

        const char* m_ShaderData = nullptr;
        GLuint m_ShaderHandle = 0; // gc_NullHandleValue;
    };

}
