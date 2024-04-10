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
		}

		eScriptTypes ScriptType()
		{
			return eScriptTypes::Testing;
		}

	};

}