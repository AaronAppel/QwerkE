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
        void Draw(GameObject* entity);

    private:
        unsigned int m_SelectedRenderableIndex = 0;

        bool m_ShowShaderList = true;
        bool m_ShowMaterialList = true;
        bool m_ShowMeshList = true;

        void ShowShaderList(RenderComponent* rComp);
        void ShowMaterialList(RenderComponent* rComp);
        void ShowMeshList(RenderComponent* rComp);

        void ShowRenderComponent(RenderComponent* rComp);
        void ShowLightComponent(LightComponent* rComp);
        void ShowAudioComponent(AudioComponent* rComp);

        GameObject* m_LastEntity = nullptr;
    };

}
