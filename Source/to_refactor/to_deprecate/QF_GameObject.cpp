#include "QF_GameObject.h"

#include "QF_Enums.h"
#include "QF_Scene.h"

namespace QwerkE {

    GameObject::GameObject(Scene* scene)
    {
        m_pScene = scene;
    }

    GameObject::GameObject(Scene* scene, vec3f position)
    {
        m_pScene = scene; // TODO: Remove scene reference in GameObject
    }

    GameObject::~GameObject()
    {
    }

    void GameObject::OnSceneLoaded(Scene* scene)
    {
        if (!scene)
            return;

        m_pScene = scene;
    }

    void GameObject::Update(double deltatime)
    {
    }

    void GameObject::Draw(GameObject* camera)
    {
    }

    void GameObject::Activate()
    {
        m_Enabled = true;
    }

    void GameObject::Deactivate()
    {
        m_Enabled = false;
    }

}
