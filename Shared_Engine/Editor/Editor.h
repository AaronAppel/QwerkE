#ifndef _Editor_H_
#define _Editor_H_

// class Editor handles all code related to the engine
// editor.
// Components include GUI windows, scene graph, shader creator, etc.

#include "SceneGraph/SceneGraph.h"

class SceneGraph;
class ShaderProgram;
class SceneManager;
class InputManager;
class EntityEditor;

class Editor // abstract
{
public:
	Editor();
	virtual ~Editor();

	virtual void NewFrame() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void DrawSceneList() = 0;

    // Getters
    EntityEditor* GetEntityEditor();

protected:
	// panels
	SceneGraph * m_SceneGraph = nullptr;
	void* m_ActionWindow = nullptr; // TODO: Game and Scene windows. Multi scene views
    EntityEditor* m_EntityEditor = nullptr; // Change routine/component data for an entity
	void* m_MenuBar = nullptr; // TODO: Top menu bar with acces to settings like save, preferences, etc
	void* m_ResourcePanel = nullptr; // TODO: Access to resources and folder structure
	// system access
	SceneManager* m_SceneManager = nullptr;
	InputManager* m_Input = nullptr;

    void DrawShaderEditor(ShaderProgram* shader);
};

#endif // !_Editor_H_
