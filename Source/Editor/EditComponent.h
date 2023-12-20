#ifndef _EditComponent_H_
#define _EditComponent_H_

#include <map>
#include <string>
#include <vector>

namespace QwerkE {

    class GameObject;
    class Material;
    class Mesh;
    class PhysicsComponent;
    class RenderComponent;
    class ShaderProgram;

    class EditComponent
    {
    public:
        EditComponent();
        ~EditComponent();

        void Draw(GameObject* entity);

    private:
        bool m_Refresh = 1;

        const std::map<std::string, Material*>* m_Materials = nullptr;
        const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
        const std::map<std::string, Mesh*>* m_Meshes = nullptr;

        std::vector<const char*> m_ShaderStrings;
        std::vector<const char*> m_MatStrings;
        std::vector<const char*> m_MeshStrings;

        unsigned int m_RenderableIndex = 0; // Current renderable selected

        bool m_ShowShaderList = true;
        int m_Shaderindex = 0; // deprecated
        bool m_ShowMaterialList = true;
        int m_Materialindex = 0; // deprecated
        bool m_ShowMeshList = true;
        int m_Meshindex = 0; // deprecated

        void ShowShaderMenu(RenderComponent* rComp);
        void ShowMaterialMenu(RenderComponent* rComp);
        void ShowMeshMenu(RenderComponent* rComp);

        void ShowRenderComponent(RenderComponent* rComp);
        void ShowPhysicsComponent(PhysicsComponent* rComp);

        GameObject* m_LastEntity = nullptr;
    };

}
#endif // _EditComponent_H_
