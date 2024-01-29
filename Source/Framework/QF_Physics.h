#pragma once

#define STATIC_MASS 0.0f // TODO: Find a better spot or deprecate

class btRigidBody;

namespace QwerkE {

    // TODO: Abstract from Bullet3 library
    class Physics
    {
    public:
        static void Initialize();
        static void TearDown();

        static void Tick();

        static void RegisterObject(btRigidBody* rigidBody);
        static void UnregisterObject(btRigidBody* rigidBody);

        static float GetDefaultRestitution();
        static float GetSphereRestitution();

    protected:
        Physics() = default;

        static void LibraryInitialize();
    };

}
