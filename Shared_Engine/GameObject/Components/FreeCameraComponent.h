#ifndef __FreeCameraComponent_H_
#define __FreeCameraComponent_H_

class FreeCameraComponent : public CameraComponent
{
public:
	FreeCameraComponent(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = g_WORLDUP, float yaw = g_YAW, float pitch = g_PITCH);
	~FreeCameraComponent();

	void ProcessKeyboard(eCamera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
	void ProcessMouseScroll(float yoffset);

private:

};

#endif //!__FreeCameraComponent_H_
