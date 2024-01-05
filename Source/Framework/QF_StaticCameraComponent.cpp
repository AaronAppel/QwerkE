#include "QF_StaticCameraComponent.h"

#include "QF_Vector.h"

namespace QwerkE {

    StaticCameraComponent::StaticCameraComponent(vec3 position, vec3 up, float yaw, float pitch) :
        CameraComponent(position, up, yaw, pitch)
    {
    }

}
