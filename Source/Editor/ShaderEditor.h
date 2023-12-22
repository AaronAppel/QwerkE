#pragma once

#include <map>
#include <string>

namespace QwerkE {

    class ShaderProgram;

    class ShaderEditor
    {
    public:
        ShaderEditor();
        ~ShaderEditor();

        void NewFrame();
        void Update();
        void Draw(bool* isOpen);

        void SetShader(ShaderProgram* shader) { m_Shader = shader; };
        const ShaderProgram* GetShader() { return m_Shader; };

    private:
        const std::map<std::string, ShaderProgram*>* m_ShaderList = nullptr;
        ShaderProgram* m_Shader = nullptr;
    };

}
