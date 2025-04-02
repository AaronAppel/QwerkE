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

			// Debug drawing
			constexpr bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
			bgfx::setState(BGFX_STATE_DEFAULT);
			DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
			debugDrawer.begin(viewIdFbo1, true);

			ComponentTransform& myTransform = m_Entity.GetComponent<ComponentTransform>();
			vec3f position = myTransform.GetPosition();
			bx::Vec3 adjustedTransform = bx::Vec3(position.x, position.y, position.z);
			adjustedTransform.y += 1.f;

			const bx::Vec3 bxPosition = bx::Vec3(position.x, position.y, position.z);
			debugDrawer.drawLine(bxPosition, adjustedTransform);
			debugDrawer.drawCylinder(bxPosition, adjustedTransform, 0.1f);

			debugDrawer.end();
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::Testing;
		}

	};

}