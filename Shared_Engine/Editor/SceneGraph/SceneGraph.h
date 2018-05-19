#ifndef _SceneGraph_H_
#define _SceneGraph_H_

// A window that shows entities and their hierarchal relationship
// Selecting an entity will show its data in the entity editor

class SceneManager;
class ResourceManager;
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
	imgui_Editor* m_Editor = nullptr;
	SceneManager* m_SceneManager = nullptr;
	ResourceManager* m_ResourceManager = nullptr;
};

#endif // !_Editor_H_
