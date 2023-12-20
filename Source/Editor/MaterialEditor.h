#ifndef _MaterialEditor_H_
#define _MaterialEditor_H_

#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h"

#include <map>
#include <string>

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
#endif // _MaterialEditor_H_
