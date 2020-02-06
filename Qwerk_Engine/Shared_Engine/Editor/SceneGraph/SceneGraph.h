#ifndef _SceneGraph_H_
#define _SceneGraph_H_

// A window that shows entities and their hierarchal relationship
// Selecting an entity will show its data in the entity editor

namespace QwerkE {

    class SceneManager;
    class Resources;
    class Editor;
    class imgui_Editor;

    // TODO: Change name to imgui_SceneGraph
    class SceneGraph
    {
    public:
        SceneGraph(Editor* editor);
        ~SceneGraph();

        void Draw();

    private:
        int m_CurrentList = 0;
        imgui_Editor* m_Editor = nullptr;
        SceneManager* m_SceneManager = nullptr;
    };

}
#endif // !_Editor_H_
