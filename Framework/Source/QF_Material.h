#pragma once

#include <string>
#include <map>

#include "Libraries/glew/glew.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Enums.h"
#include "QF_Constants.h"

#include "QF_FileUtilities.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Texture.h"

namespace QwerkE {

    class Material
    {
    public:
        Material() = default;

        Material(const std::map<eMaterialMaps, Texture*>& textures)
        {
            m_Components.clear();
            m_Components = textures; // #TODO Check how data is copied. Shallow copy should work.
        }

        Material(Material* material)
        {
            const std::map<eMaterialMaps, Texture*>* othercomponents = material->SeeMaterials();
            m_Components = *othercomponents; // #TODO Check how data is copied. Shallow copy should work.
        }

        int NumberOfMaterials() { return (int)m_Components.size(); }

        const std::map<eMaterialMaps, Texture*>* SeeMaterials() const { return &m_Components; }

        Texture* GetMaterialByType(eMaterialMaps type)
        {
            if (m_Components.find(type) != m_Components.end())
            {
                return m_Components[type];
            }
            else return nullptr;
        }

        void AddTexture(Texture* comp, eMaterialMaps type)
        {
            if (type < (int)eMaterialMaps::MatMap_Max && (int)type >= 0)
            {
                m_Components[type] = comp;
            }
        }

        void SetTexture(Texture* comp, eMaterialMaps type)
        {
            if (comp && (int)type > -1 && type < (int)eMaterialMaps::MatMap_Max)
            {
                m_Components[type] = comp;
            }
        }

        const std::string& GetMaterialName() { return m_Name; }
        void SetMaterialName(std::string name) { m_Name = name; }

    private:
        MIRROR_PRIVATE_MEMBERS

        std::string m_Name = Constants::gc_DefaultStringValue;
        std::map<eMaterialMaps, Texture*> m_Components;

        // #TODO Other data needed by shaders
        // float s_Shine = 0.5f;
        // vec4 s_LightValue = vec4(1,1,1,1);
    };

}
