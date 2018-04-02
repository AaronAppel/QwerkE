#include "SceneGraph.h"
#include "../../../Shared_Generic/Libraries/imgui/imgui.h"

SceneGraph::SceneGraph()
{
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Draw()
{
	static bool isOpen = true;
	ImGui::Begin("SceneGraph", &isOpen);

	ImGui::Text("Show Entities Here!");

	ImGui::End();
}