#pragma once

#pragma warning( disable : 26495 )
#pragma warning( disable : 4099 )
#include "Libraries/Bullet3/LinearMath/btMotionState.h"
#include "Libraries/Bullet3/LinearMath/btTransform.h"
#pragma warning( default : 26495 )
#pragma warning( default : 4099 )

#include "QF_Component.h"
#include "QF_PhysicsComponent.h"
#include "QF_GameObject.h"

class btRigidBody;

namespace QwerkE {

    class Bullet3Component : public PhysicsComponent, public btMotionState
    {
    public:
        Bullet3Component(btRigidBody* rigidBody);
        ~Bullet3Component();

        void ApplyForce(vec3 force);
        void ApplyTorque(vec3 force);

        void getWorldTransform(btTransform& worldTrans) const;
        vec3 GetBodyPosition();
        vec3 GetBodyRotation();

        void setWorldTransform(const btTransform& worldTrans);

    private:
        btRigidBody* m_Body;
    };

}
