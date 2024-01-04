#pragma once

namespace QwerkE {

    class EntityEditor;

    class SceneGraph
    {
    public:
        SceneGraph(EntityEditor* entityEditor);
        ~SceneGraph();

        void Draw();

    private:
        int m_CurrentList = 0;
        EntityEditor* m_EntityEditor = nullptr;
    };

}
