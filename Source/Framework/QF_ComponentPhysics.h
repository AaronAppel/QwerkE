#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

namespace JPH {
    class Body;
}

namespace QwerkE {

    namespace Physics {
        enum BodyShapes : u8;
    }

    class ComponentPhysics
    {
    public:
        void Create();
        bool Created() { return m_Body; }

        bool IsActive();
        void SetActive(bool a_Activate);

        void SetLinearVelocity(vec3f a_Velocity);

        vec3f BodyPosition();

        Physics::BodyShapes Shape() { return m_Shape; }
        void SetShape(Physics::BodyShapes a_NewShape);

    private:
        // friend class PhysicsSystem; // #TODO Friend class?
        void Destroy();

        MIRROR_PRIVATE_MEMBERS

        JPH::Body* m_Body = nullptr;
        Physics::BodyShapes m_Shape;
    };

}
