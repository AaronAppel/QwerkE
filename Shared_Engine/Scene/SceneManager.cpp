#include "stdafx.h"

SceneManager::SceneManager()
{
	//m_pController = new PlayerController();
}

SceneManager::~SceneManager()
{
	//delete m_pController;
	/*while (m_EventQueue.size() != 0)
	{
		Event* pEvent = m_EventQueue.front();
		m_EventQueue.pop();
		delete pEvent;
	}*/
	for (int i = 0; i < Scene_Max; i++)
	{
		Scene* temp = m_Scenes[(eSceneTypes)i];
		delete temp;
	}
}

void SceneManager::Initialize(GameCore* gamecore)
{
	m_pGameCore = gamecore;

	// *Do not enable scenes in here*
	// instantiate and LoadScene() (splash, main menu, game etc...)
	// test screen
	Scene* temp = new TestScene(m_pGameCore);
	temp->Initialize();
	m_CurrentScene = temp; // Set current
	m_Scenes[temp->GetSceneID()] = temp;
}

void SceneManager::ResetScene(eSceneTypes type)
{
	m_Scenes[type]->ResetScene();
}

void SceneManager::ResetAll()
{
	for (int i = 0; i < Scene_Max; i++)
	{
		Scene* temp = m_Scenes[(eSceneTypes)i];
		temp->ResetScene();
	}
}

void SceneManager::EnableScene(eSceneTypes type)
{
	m_Scenes[type]->SetIsEnabled(true);
}

void SceneManager::DisableScene(eSceneTypes type)
{
	m_Scenes[type]->SetIsEnabled(false);
}

void SceneManager::DisableAll()
{
	for (int i = 0; i < Scene_Max; i++)
	{
		Scene* temp = m_Scenes[(eSceneTypes)i];
		temp->SetIsEnabled(false);
	}
}

void SceneManager::Update(double deltatime) // update SceneTypes from bottom up (Max-)
{
	//this->ProcessEvents();
	for (int i = Scene_Max - 1; i >= 0; i--)
	{
		Scene* temp = m_Scenes[(eSceneTypes)i];
		if (m_IsRunning) // Add step-through and pause/play button functionality in debug mode
		if (temp->GetIsEnabled())
		{
			temp->Update(deltatime);
		}
	}
}

void SceneManager::Draw() // draw SceneTypes from top down (0+)
{
	// TODO:: might want to only draw specific scenes from gamecore
	for (int i = 0; i < Scene_Max; i++)
	{
		Scene* temp = m_Scenes[(eSceneTypes)i];
		if (temp->GetIsEnabled())
		{
			temp->Draw();
		}
	}
}

void SceneManager::DrawScene(eSceneTypes scene)
{
	m_Scenes[scene]->Draw();
}

//void SceneManager::QueueEvent(Event* pEvent)
//{
//	m_EventQueue.push(pEvent);
//}

//void SceneManager::ProcessEvents()
//{
//	while (m_EventQueue.size() != 0)
//	{
//		Event* pEvent = m_EventQueue.front();
//		m_EventQueue.pop();
//
//		this->OnEvent(pEvent);
//		delete pEvent;
//	}
//}

//void SceneManager::OnEvent(Event* pEvent)
//{
//#if _WIN32
//#endif // _WIN32
//	// other event types...
//}