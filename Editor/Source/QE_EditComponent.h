#pragma once

namespace QwerkE {

    class GameObject;

    class EditComponent
    {
    public:
        void Draw(GameObject* entity);

    private:
        unsigned int m_SelectedRenderableIndex = 0;

        bool m_ShowShaderList = true;
        bool m_ShowMaterialList = true;
        bool m_ShowMeshList = true;

        GameObject* m_LastEntity = nullptr;
    };

}
