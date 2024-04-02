#pragma once

#include "QF_ComponentTransform.h"

namespace QwerkE {

	class ScriptableCamera : public Scriptable
	{
	public:
		ScriptableCamera() = default;

		void OnCreate()
		{
			LOG_TRACE("{0}: ScriptableCamera", __FUNCTION__);

			// #TODO Taking a reference to a component is bad!
			m_CameraTransform = &m_Entity->GetComponent<ComponentTransform>();
		}

		void OnDestroy()
		{
			LOG_TRACE("{0}: ScriptableCamera", __FUNCTION__);
		}

		void OnUpdate(float deltaTime)
		{
			if (Input::GetIsKeyDown(eKeys_W))
			{
				const float moveSpeed = 3.f;
				vec3f tempTransform = m_CameraTransform->GetPosition();
				tempTransform.x = tempTransform.x + (moveSpeed * deltaTime);
				m_CameraTransform->SetPosition(tempTransform);
			}
			// LOG_TRACE("{0}: ScriptableCamera", __FUNCTION__);
		}

		// #TODO Potential methods to add
		// void OnAwake() {}
		// void OnActivate(true/false) {}
		// void OnFixedUpdate() {}
		// void OnLateUpdate() {}

	private:
		// int m_Member = 0;
		ComponentTransform* m_CameraTransform = nullptr;
	};

}