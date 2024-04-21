#pragma once

#include <vector> // For list of transforms

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_ComponentTransform.h"
#include "QF_ComponentScript.h"

#include "QF_EntityHandle.h"
#include "QF_Math.h"
#include "QF_Scriptable.h"
#include "QF_Scripting.h"

namespace QwerkE {

	class ScriptablePathFinder : public Scriptable
	{
	public:
		void OnCreate() override
		{
			HasRequiredComponents<ComponentTransform, ComponentScript>();

			// Editor only
			m_Button.m_ButtonName = "Update Targets";
			m_Button.m_CallbackFunction = []() {
				ImGui::Text("Hello World!");
			};
		}

		void OnUpdate(float deltaTime) override
		{
			if (!HasRequiredComponents<ComponentTransform, ComponentScript>())
				return;

			if (m_TransformHandles.empty())
			{
				ComponentTransform& myTransform = m_Entity.GetComponent<ComponentTransform>();

				Scene* scene = m_Entity.GetScene();

				auto viewTransforms = scene->ViewComponents<ComponentTransform>();
				for (auto& entity : viewTransforms)
				{
					EntityHandle handle(scene, entity);
					if (handle != m_Entity)
					{
						m_TransformHandles.push_back(handle);
					}
				}
			}

			if (m_TransformHandles.size() > m_CurrentTransformTargetIndex)
			{
				// #NOTE May require registering a SceneObjectDeleted callback to remove references in vector
				EntityHandle& oldTargetHandle = m_TransformHandles[m_CurrentTransformTargetIndex];
				const ComponentTransform& otherTransform = oldTargetHandle.GetComponent<ComponentTransform>();
				ComponentTransform& myTransform = m_Entity.GetComponent<ComponentTransform>();

				const vec3f distanceBeforeMove = otherTransform.GetPosition() - myTransform.GetPosition();
				if (glm::length(distanceBeforeMove) <= m_DistanceToChangeTargets)
				{
					m_CurrentTransformTargetIndex = Math::ClampRollover((size_t)0, (size_t)m_CurrentTransformTargetIndex + 1, m_TransformHandles.size() - 1);
				}

				EntityHandle& newTargetHandle = m_TransformHandles[m_CurrentTransformTargetIndex];
				const ComponentTransform& currentTargetTransform = newTargetHandle.GetComponent<ComponentTransform>();
				vec3f directionToTarget = currentTargetTransform.GetPosition() - myTransform.GetPosition();
				directionToTarget = glm::normalize(directionToTarget);

				vec3f currentPosition = myTransform.GetPosition();
				currentPosition = currentPosition + (directionToTarget * m_MovementSpeed * deltaTime);
				myTransform.SetPosition(currentPosition);
			}
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::PathFinder;
		}

	private:
		MIRROR_PRIVATE_MEMBERS

		float m_MovementSpeed = 1.f;
		float m_DistanceToChangeTargets = 1.f;
		u8 m_CurrentTransformTargetIndex = 0;
		std::vector<EntityHandle> m_TransformHandles;

		// Editor only
		ScriptGuiButton m_Button;
	};

}