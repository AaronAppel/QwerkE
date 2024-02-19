#pragma once

#pragma warning( disable : 26495 )
#pragma warning( disable : 4099 )
#include "Libraries/Bullet3/LinearMath/btAlignedObjectArray.h"
#pragma warning( default : 26495 )
#pragma warning( default : 4099 )

class btDiscreteDynamicsWorld;
class btCollisionShape;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;

namespace QwerkE {

    // TODO: Abstract from Bullet3 library

    class PhysicsWorld
    {
    public:
        PhysicsWorld();
        ~PhysicsWorld();

        void CreateWorld();
        void Update(float deltatime);
        void Cleanup();

        btDiscreteDynamicsWorld* GetDynamicsWorld() { return m_pDynamicsWorld; }
        void AddShape(btCollisionShape* pShape) { m_CollisionShapes.push_back(pShape); }
        btCollisionShape* GetShape(int index) { if (index <= m_CollisionShapes.size()) return 0; return m_CollisionShapes[index]; }

    protected:
        btBroadphaseInterface* m_pBroadphase = nullptr;
        btDefaultCollisionConfiguration* m_pCollisionConfiguration = nullptr;
        btCollisionDispatcher* m_pDispatcher = nullptr;
        btSequentialImpulseConstraintSolver* m_pSolver = nullptr;
        btDiscreteDynamicsWorld* m_pDynamicsWorld = nullptr;

        // Keep track of the shapes, we release memory at exit.
        // Make sure to re-use collision shapes among rigid bodies whenever possible!
        btAlignedObjectArray<btCollisionShape*> m_CollisionShapes;
    };

}
