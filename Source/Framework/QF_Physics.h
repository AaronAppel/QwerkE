#pragma once

namespace QwerkE {

    namespace Physics {

		class PhysicsWorld;

        void Initialize();

		PhysicsWorld* CreateWorld();

        void Shutdown();

    }

}
