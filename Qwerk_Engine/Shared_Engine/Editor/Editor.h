#ifndef _Editor_H_
#define _Editor_H_

// class Editor handles all code related to the engine
// editor.
// Components include GUI windows, scene graph, shader creator, etc.

#include "SceneGraph/SceneGraph.h"

class SceneGraph;
class ShaderProgram;
class Scenes;
class EntityEditor;

namespace QwerkE {

    // TODO: Add cross panel communication to support features such as
    // drag and drop textures, trigger callbacks, input, etc.
    // Or just stop using editor subclasses and switch to base functions only.
    class Editor // abstract
    {
    public:
        Editor();
        virtual ~Editor();

        virtual void NewFrame() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;

        virtual void ToggleFullScreenScene() = 0;

    protected:
        // panels
        SceneGraph* m_SceneGraph = nullptr;
        void* m_ActionWindow = nullptr; // TODO: Game and Scene windows. Multi scene views
        void* m_EntityEditor = nullptr; // Change routine/component data for an entity
        void* m_MenuBar = nullptr; // TODO: Top menu bar with acces to settings like save, preferences, etc
        void* m_ResourcePanel = nullptr; // TODO: Access to resources and folder structure
        // system access
        Scenes* m_Scenes = nullptr;
    };

}
#endif // !_Editor_H_
