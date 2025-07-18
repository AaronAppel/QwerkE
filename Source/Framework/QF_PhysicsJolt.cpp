#include "QF_Physics.h"

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

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
using namespace JPH::literals;

#include "QF_Input.h" // For debug testing
#include "QF_Renderer.h" // For debug testing
#include "QF_Scene.h" // For debug testing
#include "QF_Scenes.h" // For debug testing
#include "QF_EntityHandle.h" // For debug testing
#endif // _QJOLT

namespace QwerkE {

    namespace Physics {

#ifdef _QJOLT
		// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
		JPH_SUPPRESS_WARNINGS

		// All Jolt symbols are in the JPH namespace
		using namespace JPH;

		// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
		using namespace JPH::literals;

		// We're also using STL classes in this example
		using namespace std;

		// Callback for traces, connect this to your own trace function if you have one
		static void TraceImpl(const char* inFMT, ...)
		{
			// Format the message
			va_list list;
			va_start(list, inFMT);
			char buffer[1024];
			vsnprintf(buffer, sizeof(buffer), inFMT, list);
			va_end(list);

			// Print to the TTY
			cout << buffer << endl;
		}

#ifdef JPH_ENABLE_ASSERTS

		// Callback for asserts, connect this to your own assert handler if you have one
		static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine)
		{
			// Print to the TTY
			cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << endl;

			// Breakpoint
			return true;
		};

#endif // JPH_ENABLE_ASSERTS

		// Layer that objects can be in, determines which other objects it can collide with
		// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
		// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
		// but only if you do collision testing).
		namespace Layers
		{
			static constexpr ObjectLayer NON_MOVING = 0;
			static constexpr ObjectLayer MOVING = 1;
			static constexpr ObjectLayer NUM_LAYERS = 2;
		};

		/// Class that determines if two object layers can collide
		class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
		{
		public:
			virtual bool					ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
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

		// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
		// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
		// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
		// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
		// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
		namespace BroadPhaseLayers
		{
			static constexpr BroadPhaseLayer NON_MOVING(0);
			static constexpr BroadPhaseLayer MOVING(1);
			static constexpr uint NUM_LAYERS(2);
		};

		// BroadPhaseLayerInterface implementation
		// This defines a mapping between object and broadphase layers.
		class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
		{
		public:
			BPLayerInterfaceImpl()
			{
				// Create a mapping table from object to broad phase layer
				mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
				mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
			}

			virtual uint					GetNumBroadPhaseLayers() const override
			{
				return BroadPhaseLayers::NUM_LAYERS;
			}

			virtual BroadPhaseLayer			GetBroadPhaseLayer(ObjectLayer inLayer) const override
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
			BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
		};

		/// Class that determines if an object layer can collide with a broadphase layer
		class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
		{
		public:
			virtual bool				ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
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
		class MyContactListener : public ContactListener
		{
		public:
			// See: ContactListener
			virtual ValidateResult	OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const CollideShapeResult& inCollisionResult) override
			{
				cout << "Contact validate callback" << endl;

				// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
				return ValidateResult::AcceptAllContactsForThisBodyPair;
			}

			virtual void			OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
			{
				cout << "A contact was added" << endl;
			}

			virtual void			OnContactPersisted(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
			{
				cout << "A contact was persisted" << endl;
			}

			virtual void			OnContactRemoved(const SubShapeIDPair& inSubShapePair) override
			{
				cout << "A contact was removed" << endl;
			}
		};

		// An example activation listener
		class MyBodyActivationListener : public BodyActivationListener
		{
		public:
			virtual void		OnBodyActivated(const BodyID& inBodyID, uint64 inBodyUserData) override
			{
				cout << "A body got activated" << endl;
			}

			virtual void		OnBodyDeactivated(const BodyID& inBodyID, uint64 inBodyUserData) override
			{
				cout << "A body went to sleep" << endl;
			}
		};

		// Create the actual rigid body
		Body* floor = nullptr;
		BodyID sphere_id;
		PhysicsSystem* physics_system = nullptr;
		uint step = 0;
		BodyInterface* body_interface_ptr = nullptr;
		TempAllocatorImpl* temp_allocator = nullptr;
		JobSystemThreadPool* job_system = nullptr;
		BPLayerInterfaceImpl* broad_phase_layer_interface = nullptr;
		ObjectVsBroadPhaseLayerFilterImpl* object_vs_broadphase_layer_filter = nullptr;
		ObjectLayerPairFilterImpl* object_vs_object_layer_filter = nullptr;
		MyBodyActivationListener* body_activation_listener = nullptr;
		MyContactListener* contact_listener = nullptr;

		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		constexpr uint cMaxBodies = 1024;

		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		constexpr uint cNumBodyMutexes = 0;

		// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
		// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
		// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		constexpr uint cMaxBodyPairs = 1024;

		// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
		// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
		constexpr uint cMaxContactConstraints = 1024;

		void Initialize()
		{
			// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
			// This needs to be done before any other Jolt function is called.
			RegisterDefaultAllocator();

			// Install trace and assert callbacks
			Trace = TraceImpl;
			JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

			// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
			// It is not directly used in this example but still required.
			Factory::sInstance = new Factory();

			// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
			// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
			// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
			RegisterTypes();

			// We need a temp allocator for temporary allocations during the physics update. We're
			// pre-allocating 10 MB to avoid having to do allocations during the physics update.
			// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
			// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
			// malloc / free.
			temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);

			// We need a job system that will execute physics jobs on multiple threads. Typically
			// you would implement the JobSystem interface yourself and let Jolt Physics run on top
			// of your own job scheduler. JobSystemThreadPool is an example implementation.
			job_system = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

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
			physics_system = new PhysicsSystem();
			physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);

			// A body activation listener gets notified when bodies activate and go to sleep
			// Note that this is called from a job so whatever you do here needs to be thread safe.
			// Registering one is entirely optional.
			body_activation_listener = new MyBodyActivationListener();
			physics_system->SetBodyActivationListener(body_activation_listener);

			// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
			// Note that this is called from a job so whatever you do here needs to be thread safe.
			// Registering one is entirely optional.
			contact_listener = new MyContactListener();
			physics_system->SetContactListener(contact_listener);

			// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
			// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
			BodyInterface& body_interface = physics_system->GetBodyInterface();
			body_interface_ptr = &body_interface;

			// Next we can create a rigid body to serve as the floor, we make a large box
			// Create the settings for the collision volume (the shape).
			// Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
			BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
			floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

			// Create the shape
			ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
			ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

			// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
			BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

			// Create the actual rigid body
			floor = body_interface.CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

			// Add it to the world
			body_interface.AddBody(floor->GetID(), EActivation::DontActivate);

			// Now create a dynamic body to bounce on the floor
			// Note that this uses the shorthand version of creating and adding a body to the world
			BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 3.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
			sphere_id = body_interface.CreateAndAddBody(sphere_settings, EActivation::Activate);

			// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
			// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
			body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));

			// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
			// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
			// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
			physics_system->OptimizeBroadPhase();

			// Now we're ready to simulate the body, keep simulating until it goes to sleep
		}

		EntityHandle entity;
		void StepSimulation()
		{
			// #TODO Refactor test scenario

			static bool runningSimulation = true;

			if (Input::KeyDown(QKey::e_M))
			{
				runningSimulation = !runningSimulation;
			}

			if (Input::KeyPressed(QKey::e_O))
			{
				runningSimulation = false;
			}

			if ((!Scenes::GetCurrentScene()->GetIsPaused() && runningSimulation) || Input::KeyPressed(QKey::e_O) || Input::KeyPressed(QKey::e_L))
			{
				if (Input::KeyPressed(QKey::e_L))
				{
					if (!body_interface_ptr->IsActive(sphere_id))
					{
						body_interface_ptr->ActivateBody(sphere_id);
					}
					body_interface_ptr->SetLinearVelocity(sphere_id, Vec3(0.0f, 5.0f, 0.0f));
				}

				// Output current position and velocity of the sphere
				RVec3 position = body_interface_ptr->GetCenterOfMassPosition(sphere_id);
				Vec3 velocity = body_interface_ptr->GetLinearVelocity(sphere_id);
				// cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << endl;

				// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
				const int cCollisionSteps = 1;

				// Step the world
				// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
				constexpr float cDeltaTime = 1.0f / 60.0f;
				physics_system->Update(cDeltaTime, cCollisionSteps, temp_allocator, job_system);
				++step;

				// TESTING
				if (!entity.IsValid())
				{
					entity = Scenes::GetCurrentScene()->GetEntityByGuid(4131277061482423251); // #TODO Hard coded guid
				}

				vec3f pos = { position.GetX(), position.GetY(), position.GetZ() };

				DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TODO Move physics debug drawing
				debugDrawer.begin(2); // #TODO Hard coded view ID
				debugDrawer.drawSphere(pos.x, pos.y, pos.z, 0.5f);
				debugDrawer.end();

				if (entity.IsValid())
				{
					entity.GetComponent<ComponentTransform>().SetPosition(pos);
				}
			}
		}

		void Shutdown()
		{
			BodyInterface& body_interface = physics_system->GetBodyInterface();

			// Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
			body_interface.RemoveBody(sphere_id);

			// Destroy the sphere. After this the sphere ID is no longer valid.
			body_interface.DestroyBody(sphere_id);

			// Remove and destroy the floor
			body_interface.RemoveBody(floor->GetID());
			body_interface.DestroyBody(floor->GetID());

			// Unregisters all types with the factory and cleans up the default material
			UnregisterTypes();

			// Destroy the factory
			delete Factory::sInstance;
			Factory::sInstance = nullptr;

		}

#endif // _QJOLT

    }

}
