#include "QF_Physics.h"

#include <vector>

#include "QF_PhysicsWorld.h" // For debug testing

namespace QwerkE {

    namespace Physics {

		std::vector<PhysicsWorld*> s_PhysicsWorlds;

		void Initialize()
		{
		}

		PhysicsWorld* CreateWorld() // #TODO Manage worlds for Shutdown()
		{
			PhysicsWorld* newWorld = new PhysicsWorld();
			s_PhysicsWorlds.emplace_back(newWorld);
			return newWorld;
		}

		void Shutdown()
		{
			// #TODO Delete s_PhysicsWorlds;
		}

    }

}
