#include "QF_StaticCameraComponent.h"

#include "QC_Vector.h"

namespace QwerkE {

    StaticCameraComponent::StaticCameraComponent(vec3 position, vec3 up, float yaw, float pitch) :
        ComponentCamera(position, up, yaw, pitch)
    {
    }

}
