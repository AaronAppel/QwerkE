#pragma once

class Camera
{
public:
	struct MouseCoords
	{
		int32_t m_mx;
		int32_t m_my;
		int32_t m_mz;
	};

	Camera()
	{
		Reset();
		entry::MouseState mouseState;
		Update(0.0f, mouseState, true);

		// cmdAdd("move", cmdMove);
		// inputAddBindings("camBindings", s_camBindings);
	}

	void Reset()
	{
		m_mouseNow.m_mx = 0;
		m_mouseNow.m_my = 0;
		m_mouseNow.m_mz = 0;
		m_mouseLast.m_mx = 0;
		m_mouseLast.m_my = 0;
		m_mouseLast.m_mz = 0;
		m_eye.x = 0.0f;
		m_eye.y = 0.0f;
		m_eye.z = -35.0f;
		m_at.x = 0.0f;
		m_at.y = 0.0f;
		m_at.z = -1.0f;
		m_up.x = 0.0f;
		m_up.y = 1.0f;
		m_up.z = 0.0f;
		m_horizontalAngle = 0.01f;
		m_verticalAngle = 0.0f;
		m_mouseSpeed = 0.0020f;
		m_gamepadSpeed = 0.04f;
		m_moveSpeed = 30.0f;
		m_keys = 0;
		m_mouseDown = false;
	}

	void SetKeyState(uint8_t _key, bool _down)
	{
		m_keys &= ~_key;
		m_keys |= _down ? _key : 0;
	}

	void Update(float _deltaTime, const entry::MouseState& _mouseState, bool _reset = false)
	{
		if (_reset)
		{
			m_mouseLast.m_mx = _mouseState.m_mx;
			m_mouseLast.m_my = _mouseState.m_my;
			m_mouseLast.m_mz = _mouseState.m_mz;
			m_mouseNow = m_mouseLast;
			m_mouseDown = false;

			return;
		}

		if (!m_mouseDown)
		{
			m_mouseLast.m_mx = _mouseState.m_mx;
			m_mouseLast.m_my = _mouseState.m_my;
		}

		m_mouseDown = !!_mouseState.m_buttons[entry::MouseButton::Right];

		if (m_mouseDown)
		{
			m_mouseNow.m_mx = _mouseState.m_mx;
			m_mouseNow.m_my = _mouseState.m_my;
		}

		m_mouseLast.m_mz = m_mouseNow.m_mz;
		m_mouseNow.m_mz = _mouseState.m_mz;

		const float deltaZ = float(m_mouseNow.m_mz - m_mouseLast.m_mz);

		if (m_mouseDown)
		{
			const int32_t deltaX = m_mouseNow.m_mx - m_mouseLast.m_mx;
			const int32_t deltaY = m_mouseNow.m_my - m_mouseLast.m_my;

			m_horizontalAngle += m_mouseSpeed * float(deltaX);
			m_verticalAngle -= m_mouseSpeed * float(deltaY);

			m_mouseLast.m_mx = m_mouseNow.m_mx;
			m_mouseLast.m_my = m_mouseNow.m_my;
		}

		entry::GamepadHandle handle = { 0 };
		// m_horizontalAngle += m_gamepadSpeed * inputGetGamepadAxis(handle, entry::GamepadAxis::RightX)/32768.0f;
		// m_verticalAngle   -= m_gamepadSpeed * inputGetGamepadAxis(handle, entry::GamepadAxis::RightY)/32768.0f;
		// const int32_t gpx = inputGetGamepadAxis(handle, entry::GamepadAxis::LeftX);
		// const int32_t gpy = inputGetGamepadAxis(handle, entry::GamepadAxis::LeftY);
		// m_keys |= gpx < -16834 ? CAMERA_KEY_LEFT     : 0;
		// m_keys |= gpx >  16834 ? CAMERA_KEY_RIGHT    : 0;
		// m_keys |= gpy < -16834 ? CAMERA_KEY_FORWARD  : 0;
		// m_keys |= gpy >  16834 ? CAMERA_KEY_BACKWARD : 0;

		const bx::Vec3 direction =
		{
			bx::cos(m_verticalAngle) * bx::sin(m_horizontalAngle),
			bx::sin(m_verticalAngle),
			bx::cos(m_verticalAngle) * bx::cos(m_horizontalAngle),
		};

		const bx::Vec3 right =
		{
			bx::sin(m_horizontalAngle - bx::kPiHalf),
			0.0f,
			bx::cos(m_horizontalAngle - bx::kPiHalf),
		};

		const bx::Vec3 up = bx::cross(right, direction);

		m_eye = bx::mad(direction, deltaZ * _deltaTime * m_moveSpeed, m_eye);

		if (m_keys & CAMERA_KEY_FORWARD)
		{
			m_eye = bx::mad(direction, _deltaTime * m_moveSpeed, m_eye);
			SetKeyState(CAMERA_KEY_FORWARD, false);
		}

		if (m_keys & CAMERA_KEY_BACKWARD)
		{
			m_eye = bx::mad(direction, -_deltaTime * m_moveSpeed, m_eye);
			SetKeyState(CAMERA_KEY_BACKWARD, false);
		}

		if (m_keys & CAMERA_KEY_LEFT)
		{
			m_eye = bx::mad(right, _deltaTime * m_moveSpeed, m_eye);
			SetKeyState(CAMERA_KEY_LEFT, false);
		}

		if (m_keys & CAMERA_KEY_RIGHT)
		{
			m_eye = bx::mad(right, -_deltaTime * m_moveSpeed, m_eye);
			SetKeyState(CAMERA_KEY_RIGHT, false);
		}

		if (m_keys & CAMERA_KEY_UP)
		{
			m_eye = bx::mad(up, _deltaTime * m_moveSpeed, m_eye);
			SetKeyState(CAMERA_KEY_UP, false);
		}

		if (m_keys & CAMERA_KEY_DOWN)
		{
			m_eye = bx::mad(up, -_deltaTime * m_moveSpeed, m_eye);
			SetKeyState(CAMERA_KEY_DOWN, false);
		}

		m_at = bx::add(m_eye, direction);
		m_up = bx::cross(right, direction);
	}

	void SetMouseCoordsNow(MouseCoords mouseCoordsNow) { m_mouseNow = mouseCoordsNow; }
	MouseCoords GetMouseCoordsNow() { return m_mouseNow; }

	void GetViewMtx(float* _viewMtx)
	{
		bx::mtxLookAt(_viewMtx, bx::load<bx::Vec3>(&m_eye.x), bx::load<bx::Vec3>(&m_at.x), bx::load<bx::Vec3>(&m_up.x));
	}

	bx::Vec3 GetEye()
	{
		return m_eye;
	}

	 void SetEye(bx::Vec3 eye)
	{
		m_eye = eye;
	}

private:
	MouseCoords m_mouseNow;
	MouseCoords m_mouseLast;

	bx::Vec3 m_eye = bx::InitZero;
	bx::Vec3 m_at = bx::InitZero;
	bx::Vec3 m_up = bx::InitZero;
	float m_horizontalAngle;
	float m_verticalAngle;

	float m_mouseSpeed;
	float m_gamepadSpeed;
	float m_moveSpeed;

	uint8_t m_keys;
	bool m_mouseDown;
};
