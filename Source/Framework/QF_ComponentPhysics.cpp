#include "QF_ComponentPhysics.h"

#ifdef _QJOLT
#include "Libraries/Jolt/Physics/Body/Body.h"
#include "Libraries/Jolt/Physics/Body/BodyId.h"
#include "Libraries/Jolt/Physics/Body/BodyCreationSettings.h"
#include "Libraries/Jolt/Physics/Body/BodyInterface.h"

#include "Libraries/Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Libraries/Jolt/Physics/Collision/Shape/SphereShape.h"

#include "Libraries/Jolt/Math/Real.h"
#endif

#include "QF_ComponentTransform.h"
#include "QF_Physics.h"

// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
using namespace JPH::literals;

namespace QwerkE {

    void ComponentPhysics::Create()
    {
        switch (m_Shape)
        {
        case QwerkE::Physics::Sphere:
        {
            JPH::BodyInterface& bodyInterface = Physics::_GetbodyInterface();
            // Now create a dynamic body to bounce on the floor
            // Note that this uses the shorthand version of creating and adding a body to the world
            JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0_r, 3.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
            m_Body = Physics::CreateBody(sphere_settings);
            Physics::AddBody(*m_Body, JPH::EActivation::Activate);
        }
        break;
        case QwerkE::Physics::Box:
        {
            // Next we can create a rigid body to serve as the floor, we make a large box
            // Create the settings for the collision volume (the shape).
            // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
            JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));
            floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

            // Create the shape
            JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
            JPH::ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

            // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
            JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(0.0_r, -1.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);

            // Add it to the world
            m_Body = Physics::CreateBody(floor_settings);
            if (m_Body)
            {
                Physics::AddBody(*m_Body, JPH::EActivation::DontActivate);
            }
            else
            {
                // #TODO Error
            }
        }
        break;

        case QwerkE::Physics::Count:
        default:
            break;
        }
    }

    bool ComponentPhysics::IsActive()
    {
        return m_Body && m_Body->IsActive();
    }

    void ComponentPhysics::SetActive(bool a_Activate)
    {
        if (!IsActive())
        {
            JPH::BodyInterface& bodyInterface = Physics::_GetbodyInterface();
            bodyInterface.ActivateBody(m_Body->GetID());
        }
    }

    void ComponentPhysics::SetLinearVelocity(vec3f a_Velocity)
    {
        if (m_Body)
        {
            if (!IsActive())
            {
                SetActive(true);
            }
            JPH::BodyInterface& bodyInterface = Physics::_GetbodyInterface();
            bodyInterface.SetLinearVelocity(m_Body->GetID(), JPH::Vec3(a_Velocity.x, a_Velocity.y, a_Velocity.z));
        }
    }

    vec3f ComponentPhysics::BodyPosition()
    {
        if (m_Body)
        {
            // JPH::Vec3 bodyPosition = m_Body->GetPosition();
            JPH::Vec3 bodyPosition = m_Body->GetCenterOfMassPosition();
            return vec3f(bodyPosition.GetX(), bodyPosition.GetY(), bodyPosition.GetZ());
        }
        return vec3f();
    }

    void ComponentPhysics::SetShape(Physics::BodyShapes a_NewShape)
    {
        if (true)
        {
            // #TODO Destroy old shape
        }

        // #TODO Create old shape
    }

    void ComponentPhysics::Destroy()
    {
        // #TODO Copy from physics jolt example
        // body_interface.RemoveBody(floor->GetID());
        // body_interface.DestroyBody(floor->GetID());
        m_Body = nullptr;
    }

}
