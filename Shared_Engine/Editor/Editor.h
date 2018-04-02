#ifndef _Editor_H_
#define _Editor_H_

// class Editor handles all code related to the engine
// editor.
// Components include GUI windows, scenegraph, shader creator, etc.

#include "SceneGraph/SceneGraph.h"

class SceneGraph;

class Editor
{
public:
    Editor();
    ~Editor();

    void NewFrame();
    void Update();
    void Draw();

private:
	SceneGraph * m_SceneGraph = nullptr;
};

#endif // !_Editor_H_
