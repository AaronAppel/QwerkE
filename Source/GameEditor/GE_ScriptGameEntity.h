#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#ifdef _QBGFX
#include "Libraries/bx/include/bx/math.h"
#endif

#include "QC_Math.h"

#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

namespace QwerkE { // #TODO Should be in a Game namespace

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

			float matrix[16];
			bx::mtxRotateXYZ(matrix, 0.f, m_DegreesPerSecond * DEG_TO_RAD * deltaTime, 0.f);
			bx::mtxMul(transform.m_Matrix, matrix, transform.m_Matrix);
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::UserScript1;
		}

	private:
		MIRROR_PRIVATE_MEMBERS
		float m_DegreesPerSecond = 90.f; // #TODO Add Editor inspection support. May need to move to Editor domain
	};

}