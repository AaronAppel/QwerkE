#include "SceneGraph.h"
#include "../../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../../Scene/SceneManager.h"
#include "../../Systems/ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Systems/ServiceLocator.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Components/RenderComponent.h"

#include <map>
#include <string>

SceneGraph::SceneGraph()
{
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_ResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Draw()
{
	static bool isOpen = true;
	ImGui::Begin("SceneGraph", &isOpen);

	std::map<std::string, GameObject*> entities = m_SceneManager->GetCurrentScene()->GetObjectList();
	std::map<std::string, GameObject*>::iterator thing;

	static int shaderIndex = 0;
	static int matIndex = 0;
	const char* shaders[3] = { "LitMaterial", "Basic3D", "TestShader" };
	const char* materials[3] = { "container", "UV_Map", "null" };

	for (thing = entities.begin(); thing != entities.end(); thing++)
	{
		RenderComponent* rComp = (RenderComponent*)thing->second->GetComponent(Component_Render);
		ImGui::Text(thing->second->GetName().c_str());
		if (ImGui::ListBox("Shaders", &shaderIndex, shaders, 3, 3))
		{
			rComp->SetShader(m_ResourceManager->GetShader(shaders[shaderIndex]));
		}
		if (ImGui::ListBox("Materials", &matIndex, materials, 3, 3))
		{
			rComp->SetMaterial(m_ResourceManager->GetMaterial(materials[matIndex]));
		}

		float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
		ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
		rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));
	}

	ImGui::End();
}