#pragma once

#pragma warning( disable : 26495 )
#pragma warning( disable : 4099 )
#include "Libraries/Bullet3/LinearMath/btVector3.h"
#pragma warning( default : 26495 )
#pragma warning( default : 4099 )

class btRigidBody;
class btCollisionShape;

namespace QwerkE {

    class PhysicsFactory
    {
    public:
        static btRigidBody* CreateRigidSphere(btVector3 origin, float radius, float mass);
        static btRigidBody* CreateRigidPlane(btVector3 origin, float width, float length, float mass);
        static btRigidBody* CreateRigidCube(btVector3 origin, float width, float height, float depth, float mass);
        static btRigidBody* CreateRigidCylinder(btVector3 origin, float radius, float height, float mass);
        static btRigidBody* CreateRigidCone(btVector3 origin, float radius, float height, float mass);

    private:
        static btRigidBody* CreateRigidBody(btVector3 origin, btCollisionShape* shape, float mass);
    };

}
