#ifndef _imgui_Editor_H_
#define _imgui_Editor_H_

#include "Editor.h"

class imgui_ShaderEditor;
class imgui_SceneViewer;
class ResourceViewer;
class EntityEditor;

class imgui_Editor : Editor
{
public:
	imgui_Editor();
	~imgui_Editor();

	void NewFrame();
	void Update();
	void Draw();

	EntityEditor* GetEntityEditor() { return m_EntityEditor; }

private:
	ResourceViewer* m_ResourceViewer = nullptr;
	imgui_ShaderEditor* m_ShaderEditor = nullptr;
	imgui_SceneViewer* m_SceneViewer = nullptr;
	EntityEditor* m_EntityEditor = nullptr;
};

#endif // !_Editor_H_
