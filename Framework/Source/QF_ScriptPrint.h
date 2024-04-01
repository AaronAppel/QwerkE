#pragma once

#include "QF_NativeScript.h"

namespace QwerkE {

	class ScriptablePrint : public Scriptable
	{
	public:
		ScriptablePrint(Entity* entity) : Scriptable(entity) { }

		void OnCreate()
		{
			LOG_TRACE("{0} OnCreate", __FUNCTION__);
			// m_Member = 1;
			// GetComponent<Transform>();
		}

		void OnDestroy()
		{
			LOG_TRACE("{0} OnDestroy", __FUNCTION__);
		}

		void OnUpdate(float deltaTime)
		{
			LOG_TRACE("{0} OnUpdate", __FUNCTION__);
		}

		// #TODO Potential methods to add
		// void OnAwake() {}
		// void OnActivate(true/false) {}
		// void OnFixedUpdate() {}
		// void OnLateUpdate() {}

	private:
		// int m_Member = 0;
		// Transform* playerTransform = nullptr;
	};

}