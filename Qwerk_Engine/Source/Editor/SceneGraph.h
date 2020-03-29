#ifndef _SceneGraph_H_
#define _SceneGraph_H_

// A window that shows entities and their hierarchal relationship.
// Selecting an entity will show its data in the entity editor.

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
#endif // _SceneGraph_H_
