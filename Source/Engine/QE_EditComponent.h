#pragma once

#include <map>
#include <string>
#include <vector>

namespace QwerkE {

    class AudioComponent;
    class GameObject;
    class LightComponent;
    class Material;
    class Mesh;
    class PhysicsComponent;
    class RenderComponent;
    class ShaderProgram;

    class EditComponent
    {
    public:
        EditComponent();

        void Draw(GameObject* entity);

    private:
        const std::map<std::string, Material*>* m_Materials = nullptr;
        const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
        const std::map<std::string, Mesh*>* m_Meshes = nullptr;

        unsigned int m_RenderableIndex = 0; // Current renderable selected

        bool m_ShowShaderList = true;
        bool m_ShowMaterialList = true;
        bool m_ShowMeshList = true;

        void ShowShaderMenu(RenderComponent* rComp);
        void ShowMaterialMenu(RenderComponent* rComp);
        void ShowMeshMenu(RenderComponent* rComp);

        void ShowRenderComponent(RenderComponent* rComp);
        void ShowPhysicsComponent(PhysicsComponent* rComp);
        void ShowLightComponent(LightComponent* rComp);
        void ShowAudioComponent(AudioComponent* rComp);

        GameObject* m_LastEntity = nullptr;
    };

}
