#ifndef _Config_Editor_H_
#define _Config_Editor_H_

namespace QwerkE {

    class ConfigEditor
    {
    public:

        void Draw();

    private:
        std::string m_CurrentConfigFile = gc_DefaultStringValue;
    };

}
#endif //!_Config_Editor_H_
