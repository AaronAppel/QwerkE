#pragma once

#include <map>
#include <string>
#include <vector>

#include "Libraries/entt/entt.hpp"
#include "Libraries/glm/glm/common.hpp"
#include "Libraries/glm/glm/gtc/type_ptr.hpp"
#include "Libraries/glm/glm/gtc/matrix_transform.hpp"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Enums.h"
#include "QF_Constants.h"

#include "QC_Vector.h"

namespace QwerkE {

    class Component;
    class Routine;
    class Scene;

    // TODO: setup a Unity-esque transform for a GameObject
    // TODO: Move transform to it's own math file
    struct Transform
    {
        // MyMatrix s_Mat; Use?

        vec3 s_Position = 0; // calculate from matrix?
        vec3 s_Rotation = 0;
        vec3 s_Scale = 1.0f;

        vec3 s_Forward = vec3(0, 0, 1); // TODO: Calculate instead of saving?
        vec3 s_Up = vec3(0, 1, 0);
        vec3 s_Right = vec3(1, 0, 0);

        // TODO:
        void Scale(vec3 scale) {}
        void Rotate(vec3 rotation) {}
        void Translate(vec3 translation) {}

        // TODO: Use quaternions to avoid gimbal lock
    };

    class GameObject final
    {
    public:
        GameObject() = default;
        GameObject(Scene* scene);
        GameObject(Scene* scene, vec3 position);
        ~GameObject();

        void Update(double deltatime);
        void Draw(GameObject* camera);

        bool AddComponent(Component* component);
        void RemoveComponent(eComponentTags tag) { m_Components.erase(tag); };

        bool AddRoutine(Routine* routine);
        void RemoveRoutine(Routine* routine);

        bool AddUpdateRoutine(Routine* routine);
        void AddDrawRoutine(Routine* routine);

        void Activate();
        void Deactivate();
        bool Enabled() { return m_Enabled; }

        const std::map<eComponentTags, Component*>* SeeComponents() const { return &m_Components; }
        const std::vector<Routine*>* SeeUpdateRoutines() { return &m_UpdateList; }
        const std::vector<Routine*>* SeeDrawRoutines() { return &m_DrawList; }

        std::string GetName() { return m_Name; };
        const vec3& GetPosition() const { return m_Transform.s_Position; };
        const vec3& GetRotation() const { return m_Transform.s_Rotation; };
        const vec3& GetScale() const { return m_Transform.s_Scale; };
        const Component* GetComponent(eComponentTags tag);
        const Component* SeeComponent(eComponentTags tag);
        Routine* GetFirstDrawRoutineOfType(eRoutineTypes type);
        Routine* GetFirstUpdateRoutineOfType(eRoutineTypes type);
        const Scene* GetScene() { return m_pScene; };
        eGameObjectTags GetTag() { return m_Tag; }
        const Transform& GetTransform() const { return m_Transform; };

        // TODO: overload functions to take object like float[x]s
        void UpdatePosition(vec3 position) { m_Transform.s_Position = position; }; // box 2d
        void UpdateRotation(vec3 rotation) { m_Transform.s_Rotation = rotation; }; // box 2d
        void SetName(std::string name) { m_Name = name; };
        void SetPosition(vec3 position);
        void SetRotation(vec3 rotation);
        void SetScale(vec3 scale) { m_Transform.s_Scale = scale; };
        void SetTag(eGameObjectTags tag) { m_Tag = tag; }
        void SetTransform(Transform transform) { m_Transform = transform; };

        void OnSceneLoaded(Scene* scene);

    private:
        MIRROR_PRIVATE_MEMBERS

        entt::entity m_Entity = entt::null;

        bool m_Enabled = true;
        Scene* m_pScene = nullptr; // TODO: Remove scene reference in GameObject
        std::string m_Name = gc_DefaultStringValue;
        eGameObjectTags m_Tag = GO_Tag_Null;

        Transform m_Transform;

        std::vector<Routine*> m_UpdateList;
        std::vector<Routine*> m_DrawList;

        std::map<eComponentTags, Component*> m_Components;
    };

}
