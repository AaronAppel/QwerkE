#ifndef _imgui_Config_Editor_H_
#define _imgui_Config_Editor_H_

namespace QwerkE {

    class ConfigEditor
    {
    public:

        void Draw();

    private:
        std::string m_CurrentConfigFile = gc_DefaultStringValue;
    };

}
#endif // _imgui_Config_Editor_H_
