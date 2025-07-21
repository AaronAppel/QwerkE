#pragma once

#ifdef _QJOLT
#include "Libraries/Jolt/Physics/Body/BodyCreationSettings.h"
#include "Libraries/Jolt/Physics/Body/BodyInterface.h"
#include "Libraries/Jolt/Physics/Collision/ObjectLayer.h"
#endif

#include "QF_ComponentPhysics.h"

namespace JPH {
	class Body;
}

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
	// #TODO Define in data
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

namespace QwerkE {

    namespace Physics {

		enum BodyShapes : u8
		{
			Sphere = 0,
			Box,
			Count,
		};

		// #TODO Consider creating a class to handle physics for a Scene
		class PhysicsWorld
		{
		public:
			PhysicsWorld() = default;

			// #TODO Public methods

			void StepSimulation() { }

		private:
			friend class ComponentPhysics;
			void CreateCollider() {}
			void CreateTrigger() {}

		};

        void Initialize();

		JPH::Body* CreateBody(const JPH::BodyCreationSettings& a_BodySettings);
		void AddBody(JPH::Body& a_Body, JPH::EActivation a_Activation = JPH::EActivation::DontActivate);

		JPH::BodyInterface& _GetbodyInterface();

        void StepSimulation();

        void Shutdown();

    }

}
