#include "QF_GameObject.h"

#include "QF_Component.h"
#include "QF_Enums.h"
#include "QF_Routine.h"
#include "QF_Scene.h"

namespace QwerkE {

    GameObject::GameObject(Scene* scene)
    {
        m_pScene = scene;
    }

    GameObject::GameObject(Scene* scene, vec3 position)
    {
        m_Transform.s_Position = position;
        m_pScene = scene; // TODO: Remove scene reference in GameObject
    }

    GameObject::~GameObject()
    {
        for (auto& pair : m_Components)
        {
            if (pair.second)
            {
                delete pair.second;
                pair.second = nullptr;
            }
        }
        m_Components.clear();

        for (unsigned int i = 0; i < m_UpdateList.size(); i++)
        {
            if (m_UpdateList.at(i))
            {
                m_UpdateList.at(i)->CleanUp();
                delete m_UpdateList.at(i);
                m_UpdateList.at(i) = nullptr;
            }
        }
        m_UpdateList.clear();

        for (unsigned int i = 0; i < m_DrawList.size(); i++)
        {
            if (m_DrawList.at(i))
            {
                m_DrawList.at(i)->CleanUp();
                delete m_DrawList.at(i);
                m_DrawList.at(i) = nullptr;
            }
        }
        m_DrawList.clear();
    }

    void GameObject::OnSceneLoaded(Scene* scene)
    {
        if (!scene)
            return;

        m_pScene = scene;
        for (auto& pair : m_Components)
        {
            pair.second->SetParent(this);
            pair.second->Activate();
        }

        for (auto& routine : m_DrawList)
        {
            routine->SetParent(this);
            routine->Initialize(); // #TODO May want to remove
        }

        for (auto& routine : m_UpdateList)
        {
            routine->SetParent(this);
            routine->Initialize(); // #TODO May want to remove
        }
    }

    void GameObject::Update(double deltatime)
    {
        for (size_t i = 0; i < m_UpdateList.size(); i++)
        {
            if (m_UpdateList.at(i) != nullptr)
            {
                m_UpdateList.at(i)->Update(deltatime);
            }
        }
    }

    void GameObject::Draw(GameObject* camera)
    {
        for (size_t i = 0; i < m_DrawList.size(); i++)
        {
            if (m_DrawList.at(i) != nullptr)
            {
                m_DrawList.at(i)->Draw(camera);
            }
        }
    }

    bool GameObject::AddComponent(Component* component)
    {
        if (m_Components.find(component->GetTag()) == m_Components.end())
        {
            m_Components[component->GetTag()] = component;
            component->SetParent(this);
            return true;
        }
        return false;
    };

    bool GameObject::AddRoutine(Routine* routine)
    {
        // TODO: Deprecate to force using AddUpdateRoutine(), or remove other add methods
        routine->SetParent(this);
        routine->Initialize();
        return true;
    }

    bool GameObject::AddUpdateRoutine(Routine* routine)
    {
        if (!routine) { return false; }

        routine->SetParent(this);

        for (size_t i = 0; i < m_UpdateList.size(); i++)
        {
            if (m_UpdateList.at(i) == routine)
            {
                return false;
            }
        }
        m_UpdateList.push_back(routine);
        return true;
    }

    void GameObject::AddDrawRoutine(Routine* routine)
    {
        if (!routine) { return; }

        routine->SetParent(this);

        for (size_t i = 0; i < m_DrawList.size(); i++)
        {
            if (m_DrawList.at(i) == routine)
            {
                return;
            }
        }
        m_DrawList.push_back(routine);
    }

    void GameObject::RemoveRoutine(Routine* routine) // TODO:: handle removing routines
    {
        for (size_t i = 0; i < m_UpdateList.size(); i++)
        {
            if (m_UpdateList.at(i) == routine) // pointer comparison
            {
                m_UpdateList.at(i) = nullptr; // routine needs to be delete by creator
            }
        }

        for (size_t i = 0; i < m_DrawList.size(); i++)
        {
            if (m_DrawList.at(i) == routine) // pointer comparison
            {
                m_DrawList.at(i) = nullptr; // routine needs to be delete by creator
            }
        }
    }

    const Component* GameObject::GetComponent(eComponentTags tag)
    {
        if (m_Components.find(tag) != m_Components.end())
        {
            return m_Components[tag];
        }
        return nullptr;
    }

    const Component* GameObject::SeeComponent(eComponentTags tag)
    {
        if (m_Components.find(tag) != m_Components.end())
        {
            return m_Components[tag];
        }
        return nullptr;
    }

    void GameObject::Activate()
    {
        m_Enabled = true;

        for (auto& pair : m_Components)
        {
            pair.second->Activate();
        }

        for (auto& routine : m_UpdateList)
        {
            routine->Activate();
        }

        for (auto& routine : m_DrawList)
        {
            routine->Activate();
        }
    }

    void GameObject::Deactivate()
    {
        m_Enabled = false;

        for (auto pair : m_Components)
        {
            pair.second->Deactivate();
        }

        for (auto routine : m_UpdateList)
        {
            routine->Deactivate();
        }

        for (auto routine : m_DrawList)
        {
            routine->Deactivate();
        }
    }

    Routine* GameObject::GetFirstUpdateRoutineOfType(eRoutineTypes type)
    {
        Routine* t_ReturnRoutine = nullptr;
        for (unsigned int i = 0; i < m_UpdateList.size(); i++)
        {
            if (m_UpdateList.at(i)->GetRoutineType() == type)
            {
                return m_UpdateList.at(i);
            }
        }
        return t_ReturnRoutine;
    }

    Routine* GameObject::GetFirstDrawRoutineOfType(eRoutineTypes type)
    {
        Routine* rRoutine = nullptr;
        for (unsigned int i = 0; i < m_DrawList.size(); i++)
        {
            if (m_DrawList.at(i)->GetRoutineType() == type)
            {
                rRoutine = m_DrawList.at(i);
                break;
            }
        }
        return rRoutine;
    }

    void GameObject::SetPosition(vec3 position)
    {
        m_Transform.s_Position = position;
        // TODO: Update direction vectors
        if (m_Components.find(Component_Physics) != m_Components.end())
        {
            // TODO: Update children or components. Could use listeners or callbacks.
            //((PhysicsComponent*)m_Components[Component_Physics])->SetTransform(vec2(m_Position.x, m_Position.z), m_Rotation.y);
        }
    }

    void GameObject::SetRotation(vec3 rotation)
    {
        m_Transform.s_Rotation = rotation; // TODO: Cap rotation degrees to 360
        // TODO: Update direction vectors
    }

}
