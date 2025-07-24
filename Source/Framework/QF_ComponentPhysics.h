#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

namespace JPH {
    class Body;
}

namespace QwerkE {

    class Scene;

    namespace Physics {
        enum BodyShapes : u8;
        class PhysicsWorld;
    }

    class ComponentPhysics
    {
    public:
        ComponentPhysics() = default;
        ComponentPhysics(Scene* a_Scene);
        ~ComponentPhysics();

        void Initialize(Scene* a_Scene);
        void Initialize(Physics::PhysicsWorld* a_PhysicsWorld);

        // #TODO Remove Scene* dependency in every function

        bool IsActive();
        void SetActive(bool a_Activate);

        void SetLinearVelocity(vec3f a_Velocity);

        vec3f BodyPosition();
        vec3f BodyScale();

        Physics::BodyShapes Shape() { return m_Shape; }
        void SetShape(Physics::BodyShapes a_NewShape);

    private:
        void Create(Physics::PhysicsWorld* a_PhysicsWorld);
        bool Created() { return m_Body; }

        // friend class PhysicsSystem; // #TODO Friend class?
        void Destroy();

        MIRROR_PRIVATE_MEMBERS

        Physics::PhysicsWorld* m_PhysicsWorld = nullptr;
        JPH::Body* m_Body = nullptr;
        Physics::BodyShapes m_Shape;
    };

}
