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
    }

    class ComponentPhysics
    {
    public:
        ComponentPhysics();
        ComponentPhysics(Scene* a_Scene);
        ~ComponentPhysics();

        void Initialize(Scene* a_Scene);

        // #TODO Remove Scene* dependency in every function

        bool IsActive(Scene* a_Scene);
        void SetActive(bool a_Activate, Scene* a_Scene);

        void SetLinearVelocity(vec3f a_Velocity, Scene* a_Scene);

        vec3f BodyPosition(Scene* a_Scene);

        Physics::BodyShapes Shape(Scene* a_Scene) { return m_Shape; }
        void SetShape(Physics::BodyShapes a_NewShape, Scene* a_Scene);

    private:
        void Create(Scene* a_Scene);
        bool Created() { return m_Body; }

        // friend class PhysicsSystem; // #TODO Friend class?
        void Destroy();

        MIRROR_PRIVATE_MEMBERS

        JPH::Body* m_Body = nullptr;
        Physics::BodyShapes m_Shape;
    };

}
