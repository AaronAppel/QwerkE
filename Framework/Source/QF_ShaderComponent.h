#pragma once

#include <string>

#include "QF_Constants.h"
#include "QF_GraphicsHeader.h"

#ifdef _QOPENGL
typedef unsigned int GLenum;
typedef unsigned int GLuint;
#endif

namespace QwerkE {

    enum eShaderComponentTypes
    {
        None = 0,
        Vertex,
        Fragment,
        Geometry,
        eShaderComponentTypes_Max
    };

    class ShaderComponent
    {
    public:
        ShaderComponent() = default;
        ShaderComponent(const char* shaderPath);
        ~ShaderComponent();

        std::string GetName() const { return m_Name; }
        std::string GetType() const { return m_Type; }
#ifdef _QOPENGL
        GLenum GetGLShaderType() const { return m_GLShaderType; }
        GLuint GetHandle() const { return m_ShaderHandle; }
#endif
        const char* GetStringData() const { return m_ShaderData; }

        void SetName(std::string name) { m_Name = name; }
        void SetType(std::string name) { m_Type = name; }
#ifdef _QOPENGL
        void SetGLShaderType(GLenum glType) { m_GLShaderType = glType; }
        void SetHandle(GLuint handle) { m_ShaderHandle = handle; }
#endif
        void SetStringData(const char* shaderData) { m_ShaderData = shaderData; }

        void DestroyShaderData();

    private:
        std::string m_Name = Constants::gc_DefaultStringValue;
        std::string m_Type = Constants::gc_DefaultStringValue; // TODO: Think of making an enum for shader component type, or use GLenum

#ifdef _QOPENGL
        GLenum m_GLShaderType = 0;
        GLuint m_ShaderHandle = gc_NullHandleValue;
#endif
        const char* m_ShaderData = nullptr;
    };

}
