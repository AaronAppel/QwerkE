#include "stdafx.h"

GameObject::GameObject(Scene* scene)
{
	m_pScene = scene;
	for (int i = 0; i < m_BaseUpdateListSize; i++)
	{
		m_UpdateList.push_back(nullptr); // fill for runtime
	}
	for (int i = 0; i < m_BaseDrawListSize; i++)
	{
		m_DrawList.push_back(nullptr); // fill for runtime
	}
}

GameObject::~GameObject()
{
	for (auto l_LoopVar : m_Components)
	{
		delete l_LoopVar.second;
	}
	m_Components.clear();
	for (uint i = 0; i < m_UpdateList.size(); i++)
	{
		if (m_UpdateList.at(i) != nullptr)
		{
			m_UpdateList.at(i)->CleanUp(); // detach foreign routines
		}
	}
	for (uint i = 0; i < m_UpdateList.size(); i++)
	{
		delete m_UpdateList.at(i); // TODO: Delete or nullptr?
	}
	m_UpdateList.clear();
	for (uint i = 0; i < m_DrawList.size(); i++)
	{
		delete m_DrawList.at(i);
	}
	m_DrawList.clear();
}

void GameObject::Update(double deltatime)
{
	for (uint i = 0; i < m_UpdateList.size(); i++)
	{
		if (m_UpdateList.at(i) != nullptr)
		{
			m_UpdateList.at(i)->Update(deltatime);
		}
	}
	Component* tempComp = nullptr;
}

void GameObject::Draw(GameObject* camera)
{
	for (uint i = 0; i < m_DrawList.size(); i++)
	{
		if (m_DrawList.at(i) != nullptr)
		{
			m_DrawList.at(i)->Draw(camera);
		}
	}
}

void GameObject::AddComponent(Component* component)
{
	if (m_Components.find(component->GetTag()) == m_Components.end())
	{
		m_Components[component->GetTag()] = component;
		component->SetParent(this);
	}
};

void GameObject::AddRoutine(Routine* routine)
{
	routine->SetParent(this);
	routine->Initialize();
}

void GameObject::AddUpdateRoutine(Routine* routine)
{
	for (uint i = 0; i < m_UpdateList.size(); i++)
	{
		if (m_UpdateList.at(i) == nullptr)
		{
			m_UpdateList.at(i) = routine;
			return;
		}
	}
	m_UpdateList.push_back(routine);
}

void GameObject::AddDrawRoutine(Routine* routine)
{
	for (uint i = 0; i < m_DrawList.size(); i++)
	{
		if (m_DrawList.at(i) == nullptr)
		{
			m_DrawList.at(i) = routine;
			return;
		}
	}
	m_DrawList.push_back(routine);
}

void GameObject::RemoveRoutine(Routine* routine) // TODO:: handle removing routines
{
	for (uint i = 0; i < m_UpdateList.size(); i++)
	{
		if (m_UpdateList.at(i) == routine) // pointer comparison
		{
			m_UpdateList.at(i) = nullptr; // routine needs to be delete by creator
		}
	}
	for (uint i = 0; i < m_DrawList.size(); i++)
	{
		if (m_DrawList.at(i) == routine) // pointer comparison
		{
			m_DrawList.at(i) = nullptr; // routine needs to be delete by creator
		}
	}
}

Component* GameObject::GetComponent(eComponentTags tag)
{
	if (m_Components.find(tag) != m_Components.end())
	{
		return m_Components[tag];
	}
		return nullptr;
}

void GameObject::Reset()
{
	for (auto l_LoopVar : m_Components)
	{
		l_LoopVar.second->Reset();
	}
}

void GameObject::Activate()
{
	for (auto l_LoopVar : m_Components)
	{
		l_LoopVar.second->Activate();
	}
}

void GameObject::Deactivate()
{
	for (auto l_LoopVar : m_Components)
	{
		l_LoopVar.second->Deactivate();
	}
}

Routine* GameObject::GetFirstUpdateRoutine(eRoutineTypes type)
{
	Routine* t_ReturnRoutine = nullptr;
	for (uint i = 0; i < m_UpdateList.size(); i++)
	{
		if (m_UpdateList.at(i)->GetRoutineType() == type)
		{
			return m_UpdateList.at(i);
		}
	}
	return t_ReturnRoutine;
}

void GameObject::SetPosition(vec3 position)
{
	m_Position = position;
	// TODO: Update direction vectors
	if (m_Components.find(Component_Physics) != m_Components.end())
	{
		// TODO: Update children or components. Could use listeners or callbacks.
		//((PhysicsComponent*)m_Components[Component_Physics])->SetTransform(vec2(m_Position.x, m_Position.z), m_Rotation.y);
	}
}

void GameObject::SetRotation(vec3 rotation)
{
	m_Rotation = rotation;
	// TODO: Update direction vectors
}