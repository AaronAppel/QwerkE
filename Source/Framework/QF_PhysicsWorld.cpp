#include "QF_PhysicsWorld.h"

#ifdef _QJOLT
#include "Libraries/Jolt/Jolt.h"

#include "Libraries/Jolt/RegisterTypes.h"
#include "Libraries/Jolt/Core/Factory.h"
#include "Libraries/Jolt/Core/TempAllocator.h"
#include "Libraries/Jolt/Core/JobSystemThreadPool.h"
#include "Libraries/Jolt/Physics/PhysicsSettings.h"
#include "Libraries/Jolt/Physics/PhysicsSystem.h"
#include "Libraries/Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Libraries/Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Libraries/Jolt/Physics/Body/BodyCreationSettings.h"
#include "Libraries/Jolt/Physics/Body/BodyActivationListener.h"

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

#include "QF_Input.h" // For debug testing
#include "QF_Renderer.h" // For debug testing
#include "QF_Scene.h" // For debug testing
#include "QF_Scenes.h" // For debug testing
#include "QF_EntityHandle.h" // For debug testing
#endif // _QJOLT

namespace QwerkE {

    namespace Physics {

#ifdef _QJOLT

		// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
		// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
		// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
		// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
		// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
		namespace BroadPhaseLayers
		{
			static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
			static constexpr JPH::BroadPhaseLayer MOVING(1);
			static constexpr JPH::uint NUM_LAYERS(2);
		};

		/// Class that determines if two object layers can collide
		class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
		{
		public:
			virtual bool					ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
			{
				switch (inObject1)
				{
				case Layers::NON_MOVING:
					return inObject2 == Layers::MOVING; // Non moving only collides with moving
				case Layers::MOVING:
					return true; // Moving collides with everything
				default:
					JPH_ASSERT(false);
					return false;
				}
			}
		};

		// BroadPhaseLayerInterface implementation
		// This defines a mapping between object and broadphase layers.
		class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
		{
		public:
			BPLayerInterfaceImpl()
			{
				// Create a mapping table from object to broad phase layer
				mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
				mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
			}

			virtual JPH::uint					GetNumBroadPhaseLayers() const override
			{
				return BroadPhaseLayers::NUM_LAYERS;
			}

			virtual JPH::BroadPhaseLayer			GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
			{
				JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
				return mObjectToBroadPhase[inLayer];
			}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
			virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
			{
				switch ((BroadPhaseLayer::Type)inLayer)
				{
				case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
				case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
				default:													JPH_ASSERT(false); return "INVALID";
				}
			}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

		private:
			JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
		};

		/// Class that determines if an object layer can collide with a broadphase layer
		class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
		{
		public:
			virtual bool				ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
			{
				switch (inLayer1)
				{
				case Layers::NON_MOVING:
					return inLayer2 == BroadPhaseLayers::MOVING;
				case Layers::MOVING:
					return true;
				default:
					JPH_ASSERT(false);
					return false;
				}
			}
		};

		// An example contact listener
		class MyContactListener : public JPH::ContactListener
		{
		public:
			// See: ContactListener
			virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
			{
				// std::cout << "Contact validate callback" << std::endl;

				// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
				return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
			}

			virtual void			OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
			{
				// std::cout << "A contact was added" << std::endl;
			}

			virtual void			OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
			{
				// std::cout << "A contact was persisted" << std::endl;
			}

			virtual void			OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
			{
				// std::cout << "A contact was removed" << std::endl;
			}
		};

		// An example activation listener
		class MyBodyActivationListener : public JPH::BodyActivationListener
		{
		public:
			virtual void		OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
			{
				// std::cout << "A body got activated" << std::endl;
			}

			virtual void		OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
			{
				// std::cout << "A body went to sleep" << std::endl;
			}
		};

		// Callback for traces, connect this to your own trace function if you have one
		void TraceImpl(const char* inFMT, ...)
		{
			// Format the message
			va_list list;
			va_start(list, inFMT);
			char buffer[1024];
			vsnprintf(buffer, sizeof(buffer), inFMT, list);
			va_end(list);

			// Print to the TTY
			// std::cout << buffer << std::endl;
		}

#ifdef JPH_ENABLE_ASSERTS
		bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
		{
			// Print to the TTY
			// std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

			// Breakpoint
			return true;
		}
#endif // JPH_ENABLE_ASSERTS

		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		constexpr JPH::uint cMaxBodies = 1024;

		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		constexpr JPH::uint cNumBodyMutexes = 0;

		// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
		// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
		// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		constexpr JPH::uint cMaxBodyPairs = 1024;

		// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
		// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
		constexpr JPH::uint cMaxContactConstraints = 1024;

		PhysicsWorld::~PhysicsWorld()
		{
			// #TODO Free RAM
			// m_PhysicsSystem = nullptr;
			// body_interface_ptr = nullptr;
			// m_TempAllocator = nullptr;
			// job_system = nullptr;
			// broad_phase_layer_interface = nullptr;
			// object_vs_broadphase_layer_filter = nullptr;
			// object_vs_object_layer_filter = nullptr;
			// body_activation_listener = nullptr;
			// contact_listener = nullptr;

			JPH::BodyInterface& body_interface = m_PhysicsSystem->GetBodyInterface();

			// Unregisters all types with the factory and cleans up the default material
			JPH::UnregisterTypes(); // #TODO Physics system wide shutdown?

			// Destroy the factory
			delete JPH::Factory::sInstance; // #TODO Physics system wide shutdown?
			JPH::Factory::sInstance = nullptr;
		}

		void PhysicsWorld::StepSimulation()
		{
			if (!Scenes::GetCurrentScene()->GetIsPaused())
			{
				// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
				const int cCollisionSteps = 1;

				// Step the world
				// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
				constexpr float cDeltaTime = 1.0f / 60.0f;
				m_PhysicsSystem->Update(cDeltaTime, cCollisionSteps, m_TempAllocator, job_system);
				++step;
			}
		}

		JPH::BodyInterface& PhysicsWorld::GetbodyInterface()
		{
			// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
			// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
			return m_PhysicsSystem->GetBodyInterface();
		}

		JPH::Body* PhysicsWorld::CreateBody(const JPH::BodyCreationSettings& a_BodySettings)
		{
			JPH::BodyInterface& body_interface = m_PhysicsSystem->GetBodyInterface();
			return body_interface.CreateBody(a_BodySettings); // Note that if we run out of bodies this can return nullptr;
		}

		void PhysicsWorld::AddBody(JPH::Body& a_Body, JPH::EActivation a_Activation)
		{
			JPH::BodyInterface& body_interface = m_PhysicsSystem->GetBodyInterface();
			body_interface.AddBody(a_Body.GetID(), a_Activation);
		}

		PhysicsWorld::PhysicsWorld()
		{
			// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
			// This needs to be done before any other Jolt function is called.
			JPH::RegisterDefaultAllocator();

			// Install trace and assert callbacks
			JPH::Trace = TraceImpl;
			JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

			// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
			// It is not directly used in this example but still required.
			JPH::Factory::sInstance = new JPH::Factory();

			// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
			// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
			// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
			JPH::RegisterTypes();

			// We need a temp allocator for temporary allocations during the physics update. We're
			// pre-allocating 10 MB to avoid having to do allocations during the physics update.
			// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
			// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
			// malloc / free.
			m_TempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);

			// We need a job system that will execute physics jobs on multiple threads. Typically
			// you would implement the JobSystem interface yourself and let Jolt Physics run on top
			// of your own job scheduler. JobSystemThreadPool is an example implementation.
			job_system = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);

			// Create mapping table from object layer to broadphase layer
			// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
			// Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
			broad_phase_layer_interface = new BPLayerInterfaceImpl();

			// Create class that filters object vs broadphase layers
			// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
			// Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
			object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilterImpl();

			// Create class that filters object vs object layers
			// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
			// Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
			object_vs_object_layer_filter = new ObjectLayerPairFilterImpl();

			// Now we can create the actual physics system.
			m_PhysicsSystem = new JPH::PhysicsSystem();
			m_PhysicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);

			// A body activation listener gets notified when bodies activate and go to sleep
			// Note that this is called from a job so whatever you do here needs to be thread safe.
			// Registering one is entirely optional.
			body_activation_listener = new MyBodyActivationListener();
			m_PhysicsSystem->SetBodyActivationListener(body_activation_listener);

			// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
			// Note that this is called from a job so whatever you do here needs to be thread safe.
			// Registering one is entirely optional.
			contact_listener = new MyContactListener();
			m_PhysicsSystem->SetContactListener(contact_listener);

			// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
			// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
			// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
			m_PhysicsSystem->OptimizeBroadPhase();

			// Now we're ready to simulate the body, keep simulating until it goes to sleep
		}

#endif // _QJOLT

    }

}
