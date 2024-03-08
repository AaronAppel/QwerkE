#pragma once

namespace QwerkE {

    class SceneViewer // #TODO Look at simplifying and deprecating class. Move to another editor class
    {
    public:
        SceneViewer();
        ~SceneViewer();

        void Draw();

    private:
        void DrawSceneView();
        void DrawSceneList();

        int m_currentSceneStateIndex = 0;
    };

}
