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

class Editor
{
public:
    Editor();
    ~Editor();

    void NewFrame();
    void Update();
    void Draw();

	void DrawSceneList();
    void DrawShaderEditor(ShaderProgram* shader);

private:
	SceneGraph * m_SceneGraph = nullptr;
	SceneManager * m_SceneManager = nullptr;
	InputManager * m_Input = nullptr;
};

#endif // !_Editor_H_
