#pragma once

#pragma warning( disable : 26495 )
#pragma warning( disable : 4099 )
#include "Libraries/Bullet3/LinearMath/btVector3.h"
#pragma warning( default : 26495 )
#pragma warning( default : 4099 )

class btRigidBody;
class btCollisionShape;

namespace QwerkE {

    namespace Physics  {

        btRigidBody* CreateRigidSphere(btVector3 origin, float radius, float mass);
        btRigidBody* CreateRigidPlane(btVector3 origin, float width, float length, float mass);
        btRigidBody* CreateRigidCube(btVector3 origin, float width, float height, float depth, float mass);
        btRigidBody* CreateRigidCylinder(btVector3 origin, float radius, float height, float mass);
        btRigidBody* CreateRigidCone(btVector3 origin, float radius, float height, float mass);

    }

}
