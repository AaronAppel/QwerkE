#pragma once

#include "glew/GL/glew.h"

#include "QF_QwerkE_Global_Constants.h"

#include "../Component.h"
#include "CameraComponent.h"

namespace QwerkE {

    class FreeCameraComponent : public CameraComponent
    {
    public:
        FreeCameraComponent(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = g_WORLDUP, float yaw = gc_YAW, float pitch = gc_PITCH);

        void ProcessKeyboard(eCamera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
        void ProcessMouseScroll(float yoffset);
    };

}
