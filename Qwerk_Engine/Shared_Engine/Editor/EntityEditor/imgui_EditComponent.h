#ifndef _imgui_EditComponent_H_
#define _imgui_EditComponent_H_

#include <map>
#include <string>
#include <vector>

namespace QwerkE {

    class GameObject;
    class Resources;
    class ShaderProgram;
    class Material;
    class Texture;
    class RenderComponent;
    class Mesh;

    class imgui_EditComponent
    {
    public:
        imgui_EditComponent();
        ~imgui_EditComponent();

        void Draw(GameObject* entity);

    private:
        bool m_RefreshFlag = 1;

        const std::map<std::string, Material*>* m_Materials = nullptr;
        const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
        const std::map<std::string, Mesh*>* m_Meshes = nullptr;

        std::vector<const char*> m_ShaderStrings;
        std::vector<const char*> m_MatStrings;
        std::vector<const char*> m_MeshStrings;

        int m_RenderableIndex = 0; // Current renderable selected

        bool m_ShowShaderList = true;
        int m_Shaderindex = 0; // deprecated
        bool m_ShowMaterialList = true;
        int m_Materialindex = 0; // deprecated
        bool m_ShowMeshList = true;
        int m_Meshindex = 0; // deprecated

        void ShowShaderMenu(RenderComponent* rComp);
        void ShowMaterialMenu(RenderComponent* rComp);
        void ShowMeshMenu(RenderComponent* rComp);
    };

}
#endif // !_imgui_EditComponent_H_
