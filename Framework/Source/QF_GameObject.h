#pragma once

#include <map>
#include <string>
#include <vector>

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QGLM
#include "Libraries/glm/common.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Enums.h"
#include "QF_Constants.h"

namespace QwerkE {

    class Scene;

    class GameObject final
    {
    public:
        GameObject() = default;
        GameObject(Scene* scene);
        GameObject(Scene* scene, vec3 position);
        ~GameObject();

        void Update(double deltatime);
        void Draw(GameObject* camera);

        void Activate();
        void Deactivate();
        bool Enabled() { return m_Enabled; }

        std::string GetName() { return m_Name; };
        const Scene* GetScene() { return m_pScene; };
        eGameObjectTags GetTag() { return m_Tag; }

        void SetName(std::string name) { m_Name = name; };
        void SetTag(eGameObjectTags tag) { m_Tag = tag; }

        void OnSceneLoaded(Scene* scene);

        entt::entity GetEntity() { return m_Entity; }

    private:
        MIRROR_PRIVATE_MEMBERS

        entt::entity m_Entity = entt::null;

        bool m_Enabled = true;
        Scene* m_pScene = nullptr; // TODO: Remove scene reference in GameObject
        std::string m_Name = Constants::gc_DefaultStringValue;
        eGameObjectTags m_Tag = GO_Tag_Null;

        vec3 m_Position = vec3(0.f);

        glm::mat4 m_Transform2 = glm::mat4(1.0f);
    };

}
