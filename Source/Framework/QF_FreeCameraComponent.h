#pragma once

#include "Libraries/glew/GL/glew.h"

#include "QF_Constants.h"

#include "QF_Component.h"
#include "QF_CameraComponent.h"

namespace QwerkE {

    class FreeCameraComponent : public ComponentCamera
    {
    public:
        FreeCameraComponent(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = g_WORLDUP, float yaw = gc_YAW, float pitch = gc_PITCH);

        void ProcessKeyboard(eCamera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
        void ProcessMouseScroll(float yoffset);
    };

}
