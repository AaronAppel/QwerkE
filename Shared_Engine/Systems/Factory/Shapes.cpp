#include "Factory.h"
#include "../../Scene/Scene.h"
#include "../../../Shared_Generic/Utilities/Helpers.h"
#include "../../../Shared_Engine/Engine_Enums.h"
#include "../../GameObject/Components/Component.h"
#include "../../GameObject/Components/RenderComponent.h"
#include "../../Systems/ResourceManager.h"
#include "../../GameObject/Routines/RenderRoutine.h"
#include "../../Systems/Graphics/OpenGLHelpers.h"

/* Shapes */
GameObject* Factory::CreateCube(Scene* scene, vec3 position)
{
	GameObject* t_Cube = new GameObject(scene);
	t_Cube->SetName("Cube" + std::to_string(helpers_GetUniqueID()));
	t_Cube->SetTag(eGameObjectTags::GO_Tag_Cube);
	t_Cube->SetRenderOrder(50);
	RenderComponent* renderComp = new RenderComponent();
	// Assign assets from ResourceManager()
	renderComp->SetMesh(m_pResources->GetMesh("Cube")); // CreCube
	// renderComp->SetModel(m_pResources->GetModel("CubeModel"));
	renderComp->SetShader(m_pResources->GetShader("Basic3D"));
	renderComp->SetColour(vec4(0,1,0,1));
	//renderComp->SetMaterial(m_pResources->GetMaterial("container"));

	renderComp->SetColour(vec4(RandFloatInRange(0.0f, 1.0f), RandFloatInRange(0.0f, 1.0f), RandFloatInRange(0.0f, 1.0f), 1));
	t_Cube->AddComponent(renderComp);

	RenderRoutine* renderRoutine = new RenderRoutine();
	t_Cube->AddRoutine((Routine*)renderRoutine);
	t_Cube->SetPosition(position);

	if (scene->AddObjectToScene(t_Cube))
	{
		m_Created++;
		return t_Cube;
	}

	// delete t_Cube;
	return t_Cube;
}

GameObject* Factory::CreatePlane(Scene* scene, vec3 position)
{
	GameObject* t_Plane = new GameObject(scene);
	t_Plane->SetName("Plane" + std::to_string(helpers_GetUniqueID()));
	t_Plane->SetTag(eGameObjectTags::GO_Tag_Plane);
	t_Plane->SetRenderOrder(50);
	RenderComponent* renderComp = new RenderComponent();
	renderComp->SetMesh(m_pResources->GetMesh("Plane"));

	renderComp->SetShader(m_pResources->GetShader("LitMaterial"));
	renderComp->SetMaterial(m_pResources->GetMaterial("container"));

	renderComp->SetColour(vec4(RandFloatInRange(0.0f, 1.0f), RandFloatInRange(0.0f, 1.0f), RandFloatInRange(0.0f, 1.0f), 1)); // random intial colour
	t_Plane->AddComponent(renderComp);

	RenderRoutine* renderRoutine = new RenderRoutine();
	t_Plane->AddRoutine((Routine*)renderRoutine);
	t_Plane->SetPosition(position);

	if (scene->AddObjectToScene(t_Plane))
	{
		m_Created++;
		return t_Plane;
	}

	delete t_Plane;
	return nullptr;
}