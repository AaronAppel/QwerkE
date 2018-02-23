#include "stdafx.h"

extern int g_WindowWidth;
extern int g_WindowHeight;

Scene::Scene(GameCore* pGame) :
	m_pGameCore(pGame)
{
	m_pSceneManager = m_pGameCore->GetSceneManager();

	//m_pMenuManager = new MenuManager();

	//float t_ScreenToWorld = WORLD_TO_SCREEN_RATIO / 2.0f; // 2:1 screen ratio. / 2 to change from screen to clip space

	//m_ViewWindowSize = vec2(g_WindowWidth * t_ScreenToWorld, g_WindowHeight * t_ScreenToWorld);
}

Scene::~Scene()
{
	int size = m_LightList.Size();
	for (int i = size - 1; i > -1; i--)
	{
		m_SceneDrawList.Remove(i); // Lights are drawn
		m_pGameObjects.erase(m_LightList.At(i)->GetName().c_str()); // delete
		delete m_LightList.At(i);
		m_LightList.Remove(i);
	}
	m_LightList.Clear();

	size = m_CameraList.Size();
	for (int i = size - 1; i > -1; i--)
	{
		delete m_CameraList.At(i);
		m_CameraList.Remove(i);
	}
	m_CameraList.Clear();

	for (auto object : m_pGameObjects)
	{
		delete object.second;
	}
	m_pGameObjects.clear();
}

void Scene::OnWindowResize(unsigned int width, unsigned int height)
{
	// update camera view and projection matrices
	for (int i = 0; i < m_CameraList.Size(); i++)
	{
		// ((CameraComponent*)m_CameraList.At(i)->GetComponent(Component_Camera))->Setup();
		((CameraComponent*)m_CameraList.At(i)->GetComponent(Component_Camera))->SetViewportSize(vec2((float)width, (float)height));
	}
}

void Scene::Initialize()
{
	//m_pViewMatrix->CreateLookAtViewLeftHanded(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 0, 0));
}

void Scene::ResetScene()
{
}

void Scene::p_Update(double deltatime)
{
	Controller* t_Controller = debug_GetPlayerController();
	CameraComponent* t_ActiveCamera = ((CameraComponent*)m_CameraList.At(m_CurrentCamera)->GetComponent(Component_Camera));
	// Temporary active camera control
	// Mouse
	{
		vec2 t_MouseDelta = t_Controller->GetMousePosDelta();
		t_ActiveCamera->ProcessMouseMovement(t_MouseDelta.x, t_MouseDelta.y);
		// m_Zoom
		t_ActiveCamera->ProcessMouseScroll(t_Controller->GetMouseScrollDelta());
	}

	// Keyboard
	if (t_Controller->IsButtonPressed(eGameButtons::Button_MoveForward))
	{
		t_ActiveCamera->ProcessKeyboard(eCamera_Movement::FORWARD, (float)deltatime);
	}
	if (t_Controller->IsButtonPressed(eGameButtons::Button_MoveBackward))
	{
		t_ActiveCamera->ProcessKeyboard(eCamera_Movement::BACKWARD, (float)deltatime);
	}
	if (t_Controller->IsButtonPressed(eGameButtons::Button_MoveLeft))
	{
		t_ActiveCamera->ProcessKeyboard(eCamera_Movement::LEFT, (float)deltatime);
	}
	if (t_Controller->IsButtonPressed(eGameButtons::Button_MoveRight))
	{
		t_ActiveCamera->ProcessKeyboard(eCamera_Movement::RIGHT, (float)deltatime);
	}
	if (t_Controller->IsButtonPressed(eGameButtons::Button_MoveUp))
	{
		t_ActiveCamera->ProcessKeyboard(eCamera_Movement::UP, (float)deltatime);
	}
	if (t_Controller->IsButtonPressed(eGameButtons::Button_MoveDown))
	{
		t_ActiveCamera->ProcessKeyboard(eCamera_Movement::DOWN, (float)deltatime);
	}

	// update all of the Scene objects in the list.
	for (auto object : m_pGameObjects)
	{
		object.second->Update(deltatime);
	}
}

void Scene::Draw(GameObject* camera)
{
	// TODO:: Define draw behaviour (Highest first vs lowest first)
	for (int i = m_SceneDrawList.Size() - 1; i >= 0; i--)
	{
		m_SceneDrawList.At(i)->Draw(camera); // TODO:: Control render order
	}
}

bool Scene::AddCamera(GameObject* camera)
{
	if (true) // TODO: What conditions would prevent camera insertion?
	{
		m_CameraList.PushBack(camera);
		return true;
	}
	return false;
}

void Scene::RemoveCamera(GameObject* camera)
{
	if (m_CameraList.Size() < 2) { return; } // Need 1 camera
	for (int i = 0; i < m_CameraList.Size(); i++)
	{
		if (m_CameraList.At(i) == camera) // Pointer comparison
		{
			m_CameraList.Remove(i);
			// Delete
			return;
		}
	}
}

void Scene::SetupCameras()
{
	for (int i = 0; i < m_CameraList.Size(); i++)
	{
		((CameraComponent*)m_CameraList.At(i)->GetComponent(Component_Camera))->Setup();
		((CameraComponent*)m_CameraList.At(i)->GetComponent(Component_Camera))->SetTargetPosition(vec3(0, 0, 0)); // Give initial target location
	}
}

bool Scene::AddLight(GameObject* light)
{
	if (true) // TODO: What conditions would prevent light insertion?
	{
		m_LightList.PushBack(light);
		m_SceneDrawList.PushBack(light);
		return true;
	}
	return false;
}
void Scene::RemoveLight(GameObject* light)
{
	if (m_LightList.Size() < 2) { return; }
	for (int i = 0; i < m_LightList.Size(); i++)
	{
		if (m_LightList.At(i) == light) // Pointer comparison
		{
			m_LightList.Remove(i);
			m_SceneDrawList.Remove(i); // Lights are drawn
			// TODO: Deallocate memory
			return;
		}
	}
}

void Scene::SetupLights()
{
	for (int i = 0; i < m_LightList.Size(); i++)
	{
	}
}

bool Scene::AddObjectToScene(GameObject* object)
{
	if (m_pGameObjects.find(object->GetName()) == m_pGameObjects.end()) // Name unique?
	{
		m_pGameObjects[object->GetName()] = object;
		// TODO: Sort by draw order
		m_SceneDrawList.PushBack(object);
		return true; // Success
	}
	return false; // Failure
}

void Scene::RemoveObjectFromScene(GameObject* object)
{
	// TODO: Fix implementation
	// Check object tag
	if (m_pGameObjects.find(object->GetName().c_str()) != m_pGameObjects.end())
	{
		// exists?
		if (object->GetTag() == eGameObjectTags::GO_Tag_Player)
		{
			// Player specific
		}
		m_SceneDrawList.Remove(object);
		m_pGameObjects.erase(object->GetName());
		delete object; // TODO:: Should deletion be handled differently?
	}
}

void Scene::RemoveAllObjectsFromScene()
{
	m_LightList.Clear();
	m_CameraList.Clear();
	m_pGameObjects.clear();
}
// Setters
void Scene::SetIsPaused(bool isPaused)
{
	m_IsPaused = isPaused;
	if (m_IsPaused)
	{
		m_UpdateFunc = &Scene::p_Paused;
	}
	else
	{
		m_UpdateFunc = &Scene::p_Update;
	}
}

void Scene::SaveScene()
{
	DataManager* t_pDataManager = m_pGameCore->GetDataManager();
	t_pDataManager->SaveScene(this, m_LevelFileDir);
}

void Scene::LoadScene()
{
	DataManager* t_pDataManager = m_pGameCore->GetDataManager();
	t_pDataManager->LoadScene(this, m_LevelFileDir);
	SetupCameras();
}