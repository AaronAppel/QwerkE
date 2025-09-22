#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#include "QC_Math.h"
#include "QC_Time.h"

#include "QF_ComponentHelpers.h"

#include "QF_Camera.h"
#include "QF_EntityHandle.h"
#include "QF_GameActions.h"
#include "QF_Input.h"
#include "QF_InputMouseDragTracker.h"
#include "QF_Scriptable.h"

#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/gtc/constants.hpp"
#endif // _QGLM

namespace QwerkE {

	// #TODO See how hard/complex it would be to support 1 camera class that handles all types of behaviour.
	// First person, third person, orthographic, perspective, and whatever else
	class ScriptableCamera : public Scriptable
	{
	public:
		ScriptableCamera() = default;

		void OnCreate() override
		{
			HasRequiredComponents<ComponentTransform, ComponentCamera, ComponentScript>();
		}

		void OnUpdate(float a_DeltaTime) override
		{
			if (!HasRequiredComponents<ComponentTransform, ComponentCamera, ComponentScript>())
				return;

			ComponentTransform& cameraTransform = m_Entity.GetComponent<ComponentTransform>();
			const Input::GameActions& gameActions = Input::GetGameActions();
			m_Camera.Update(gameActions, cameraTransform.m_Matrix, a_DeltaTime);
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::e_Camera;
		}

        Camera m_Camera;
	};

}