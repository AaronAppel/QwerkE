#pragma once

#include <vector>
#include <string>

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Component.h"
#include "QF_Renderable.h"

namespace QwerkE {

    class ShaderProgram;
    class Material;
    class Mesh;

    class RenderComponent : public Component
    {
    public:
        RenderComponent();
        RenderComponent(const char* objectRecipe);
        RenderComponent(const char* shaderName, const char* materialName, const char* meshName);

        void GenerateSchematic();

        void Activate() override;
        void Setup(const char* shaderName, const char* materialName, const char* meshName);

        void AppendEmptyRenderables(int count);
        void AddRenderable(Renderable renderable);

        std::string GetSchematicName() const { return m_SchematicName; }
        std::vector<Renderable>* GetRenderableList() { return &m_Renderables; } // #TODO Change to SeeRenderablesList()
        const std::vector<Renderable>* SeeRenderableList() const { return &m_Renderables; }

        void SetSchematicName(std::string name) { m_SchematicName = name; }
        void SetNameAtIndex(unsigned int index, std::string name);
        void SetShaderAtIndex(unsigned int index, ShaderProgram* shader);
        void SetMaterialAtIndex(unsigned int index, Material* material);
        void SetMeshAtIndex(unsigned int index, Mesh* mesh);

        const std::vector<Renderable>* LookAtRenderableList() { return &m_Renderables; }

    private:
        MIRROR_PRIVATE_MEMBERS

        std::string m_SchematicName = "None";
        std::vector<Renderable> m_Renderables;
    };

}
