#pragma once

#include <map>
#include <string>

#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h"

namespace QwerkE {

    class Material;
    struct Texture;

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
