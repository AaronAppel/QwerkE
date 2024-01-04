#pragma once

#include <map>
#include <string>

namespace QwerkE {

    class ShaderProgram;

    class ShaderEditor
    {
    public:
        ShaderEditor();

        void Draw(bool* isOpen);

        void SetShader(ShaderProgram* shader) { m_Shader = shader; };
        const ShaderProgram* GetShader() { return m_Shader; };

    private:
        const std::map<std::string, ShaderProgram*>* m_ShaderList = nullptr;
        ShaderProgram* m_Shader = nullptr;

        GLuint m_CurrentShaderProgram = 0;
        bool m_ShowShaderList = false;

        const int m_BufferSize = 2000;
        char* m_VertBuffer = new char[m_BufferSize];
        char* m_FragBuffer = new char[m_BufferSize];
        char* m_GeoBuffer  = new char[m_BufferSize]; // #TODO Decide on formatting using an extra space
    };

}
