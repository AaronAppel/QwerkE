#pragma once

typedef unsigned int GLuint;

namespace QwerkE {

    class ShaderComponent;
    class ShaderProgram;

    class ShaderEditor final // #TODO Change for editing any file type as raw text
    {
    public:
        void Draw(bool* isOpen);

        void SetCurrentShader(ShaderProgram* shader) { m_CurrentShader = shader; };
        const ShaderProgram* GetCurrentShader() { return m_CurrentShader; };

        void ToggleIsVisible() { m_IsVisible = !m_IsVisible; }

    private:
        void UpdateTextBuffer();

        bool m_IsVisible = true;

        ShaderProgram* m_CurrentShader = nullptr;
        ShaderComponent* m_CurrentShaderComponent = nullptr;
        int m_CurrentShaderTypeIndex = 0;

        const int m_TextBufferSize = 2000;
        char* m_TextBuffer = new char[m_TextBufferSize];
    };

}
