#include "QF_CameraComponent.h"

#include "QF_StaticCameraComponent.h"
#include "QF_QwerkE_Enums.h"

#include "../QwerkE_Framework/Source/Core/Math/Vector.h"

namespace QwerkE {

    StaticCameraComponent::StaticCameraComponent(vec3 position, vec3 up, float yaw, float pitch) :
        CameraComponent(position, up, yaw, pitch)
    {
    }

}
