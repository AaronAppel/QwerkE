#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QC_Time.h"

namespace QwerkE {

	class ScriptableSceneTransition : public Scriptable
	{
	public:
		ScriptableSceneTransition() = default;

		void OnDestroy() override
		{
			Scenes::SetCurrentScene(m_SceneToTransitionToGuid);
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::SceneTransition;
		}

		MIRROR_PRIVATE_MEMBERS

		GUID m_SceneToTransitionToGuid = GUID::Invalid;
	};

}