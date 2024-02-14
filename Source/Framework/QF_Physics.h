#pragma once

class btRigidBody;

namespace QwerkE {

    namespace Physics { // #TODO: Abstract from Bullet3 library

        void Initialize();
        void LibraryInitialize();
        void TearDown();

        void Tick();

        void RegisterObject(btRigidBody* rigidBody);
        void UnregisterObject(btRigidBody* rigidBody);

        float GetDefaultRestitution();
        float GetSphereRestitution();

    }

}
