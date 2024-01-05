#include "CameraComponent.h"
#include "StaticCameraComponent.h"
#include "QF_QwerkE_Enums.h"

namespace QwerkE {

    StaticCameraComponent::StaticCameraComponent(vec3 position, vec3 up, float yaw, float pitch) :
        CameraComponent(position, up, yaw, pitch)
    {
    }

}
