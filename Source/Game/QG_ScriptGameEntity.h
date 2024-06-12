#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#ifdef _QBGFX
#include "Libraries/bx/include/bx/math.h"
#endif

#include "QC_Math.h"

#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

	class ScriptGameEntity : public Scriptable
	{
	public:
		void OnCreate() override
		{
			HasRequiredComponents<ComponentTransform, ComponentScript>();
		}

		void OnUpdate(float deltaTime) override
		{
			if (!HasRequiredComponents<ComponentTransform, ComponentScript>())
				return;

			ComponentTransform& transform = m_Entity.GetComponent<ComponentTransform>();

			constexpr float angle = 90.f * DEG_TO_RAD;

			float matrix[16];
			bx::mtxRotateXYZ(matrix, 0.f, angle * deltaTime, 0.f);
			bx::mtxMul(transform.m_Matrix, matrix, transform.m_Matrix);
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::UserScript1;
		}

	private:
		MIRROR_PRIVATE_MEMBERS
	};

}