#ifndef _imgui_ShaderEditor_H_
#define _imgui_ShaderEditor_H_

#include <map>
#include <string>

class ShaderProgram;

namespace QwerkE {

    class imgui_ShaderEditor
    {
    public:
        imgui_ShaderEditor();
        ~imgui_ShaderEditor();

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
#endif // _imgui_ShaderEditor_H_
