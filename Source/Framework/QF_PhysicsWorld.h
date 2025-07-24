#pragma once

#ifdef _QJOLT
#include "Libraries/Jolt/Physics/Body/BodyCreationSettings.h"
#include "Libraries/Jolt/Physics/Body/BodyInterface.h"
#include "Libraries/Jolt/Physics/Collision/ObjectLayer.h"
#include "Libraries/Jolt/Physics/PhysicsSystem.h"

#include "Libraries/Jolt/RegisterTypes.h"
#include "Libraries/Jolt/Core/Factory.h"
#include "Libraries/Jolt/Core/TempAllocator.h"
#include "Libraries/Jolt/Core/JobSystemThreadPool.h"
#include "Libraries/Jolt/Physics/PhysicsSettings.h"
#include "Libraries/Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Libraries/Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Libraries/Jolt/Physics/Body/BodyActivationListener.h"
#endif

namespace JPH {
	class Body;
}

namespace QwerkE {

    namespace Physics {

		void TraceImpl(const char* inFMT, ...);

#ifdef JPH_ENABLE_ASSERTS
		bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine);
#endif // JPH_ENABLE_ASSERTS

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

		enum BodyShapes : u8 // #TODO Consider using QC_ENUM to expose new entries easily in editor as strings
		{
			Sphere = 0,
			Box,
			Plane,
			Count,
		};

		class ObjectLayerPairFilterImpl;
		class BPLayerInterfaceImpl;
		class ObjectVsBroadPhaseLayerFilterImpl;
		class MyContactListener;
		class MyBodyActivationListener;

		// #TODO Consider creating a class to handle physics for a Scene
		class PhysicsWorld
		{
		public:
			~PhysicsWorld();

			void StepSimulation();

			JPH::BodyInterface& GetbodyInterface();

			JPH::Body* CreateBody(const JPH::BodyCreationSettings& a_BodySettings);
			void AddBody(JPH::Body& a_Body, JPH::EActivation a_Activation = JPH::EActivation::DontActivate);

		private:
			friend class Scene;
			friend Physics::PhysicsWorld* CreateWorld();
			PhysicsWorld();

			JPH::PhysicsSystem* m_PhysicsSystem = nullptr;
			JPH::uint step = 0;
			JPH::BodyInterface* body_interface_ptr = nullptr;
			JPH::TempAllocatorImpl* m_TempAllocator = nullptr;
			JPH::JobSystemThreadPool* job_system = nullptr;

			ObjectLayerPairFilterImpl* object_vs_object_layer_filter = nullptr;
			BPLayerInterfaceImpl* broad_phase_layer_interface = nullptr;
			ObjectVsBroadPhaseLayerFilterImpl* object_vs_broadphase_layer_filter = nullptr;
			MyContactListener* contact_listener = nullptr;
			MyBodyActivationListener* body_activation_listener = nullptr;
		};

    }

}
