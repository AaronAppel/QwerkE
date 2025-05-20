#pragma once

#include "Libraries/Mirror/Source/MIR_Mirror.h"

#include "QF_Input.h"
#include "QF_Scriptable.h"

namespace QwerkE {

	class ScriptableTesting : public Scriptable
	{
	public:
		ScriptableTesting()
		{
			m_GuiButton.m_ButtonName = "Testing";
			m_GuiButton.m_CallbackFunction = []() {
				TestScriptButtonCallback();
				// #TODO Find a good way to capture [this]
				// Reference : https://stackoverflow.com/questions/7895879/using-data-member-in-lambda-capture-list-inside-a-member-function
				// IncrementTarget();
			};
			// m_GuiButton.m_CallbackFunction();
		}

		void OnUpdate(float deltaTime) override
		{
			if (Input::KeyDown(QKey::e_H))
			{
				// LOG_INFO("{0} Key \"H\" is pressed", __FUNCTION__);
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

	private:
		static void TestScriptButtonCallback()
		{
			LOG_ERROR("Script call back");
		}

		MIRROR_PRIVATE_MEMBERS

		ScriptGuiButton m_GuiButton;
	};

}