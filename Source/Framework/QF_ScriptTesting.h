#pragma once

#include "QF_Input.h"
#include "QF_Scriptable.h"

namespace QwerkE {

	class ScriptableTesting : public Scriptable
	{
	public:
		void OnUpdate(float deltaTime) override
		{
			if (Input::IsKeyDown(eKeys::eKeys_H))
			{
				LOG_INFO("{0} Key \"H\" is pressed", __FUNCTION__);
			}

#ifdef _QDEBUG
			ComponentTransform& myTransform = m_Entity.GetComponent<ComponentTransform>();
			vec3f endPosition = myTransform.GetPosition();
			endPosition += 1.f;
			Debug::DrawLine(myTransform.GetPosition(), endPosition);
			// Debug::DrawCylinder(myTransform.GetPosition(), adjustedTransform, 0.1f);
#endif
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::Testing;
		}

	};

}