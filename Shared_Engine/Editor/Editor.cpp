#include "Editor.h"

Editor::Editor()
{
	m_SceneGraph = new SceneGraph();
}

Editor::~Editor()
{
	delete m_SceneGraph;
}

void Editor::NewFrame()
{
}

void Editor::Update()
{

}

void Editor::Draw()
{
	m_SceneGraph->Draw();
}
