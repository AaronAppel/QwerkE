#ifndef _imgui_MaterialEditor_H_
#define _imgui_MaterialEditor_H_

#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h"

#include <map>
#include <string>

class Material;
class Texture;

namespace QwerkE {

    class MaterialEditor
    {
    public:
        MaterialEditor();
        ~MaterialEditor();

        void Draw(Material* material, bool* isOpen);

    private:
        eMaterialMaps m_CurrentMap = eMaterialMaps::MatMap_Null;
        const std::map<std::string, Texture*>* m_TextureList = nullptr;
    };

}
#endif // _imgui_MaterialEditor_H_
