
#include <vector>
#include <string>

#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

#include "QC_Vector.h"

#include "QF_CameraComponent.h"
#include "QF_Component.h"
#include "QF_eKeys.h"
#include "QF_Enums.h"
#include "QF_GameObject.h"
#include "QF_FirstPersonCameraComponent.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

namespace QwerkE {

	// Primitives and enums
	typedef eSceneTypes m_eSceneTypes;
	MIRROR_TYPE(m_eSceneTypes)

	typedef eGameObjectTags m_eGameObjectTags;
	MIRROR_TYPE(m_eGameObjectTags)

	typedef QwerkE::eKeys eKeys;
	MIRROR_TYPE(eKeys)

	// Vectors
	typedef std::vector<std::string> m_vector_string;
	MIRROR_TYPE(m_vector_string)

	typedef std::vector<GameObject*> m_vector_gameobjectptr;
	MIRROR_TYPE(m_vector_gameobjectptr)

	typedef std::vector<Routine*> m_vector_routineptr;
	MIRROR_TYPE(m_vector_routineptr)

	// Maps
	typedef std::map<eComponentTags, Component*> m_map_eComponentTags_componentptr;
	MIRROR_TYPE(m_map_eComponentTags_componentptr)

	// Structs
	MIRROR_CLASS_START(Vector3)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_MEMBER(z)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(Transform)
	MIRROR_CLASS_MEMBER(s_Position)
	MIRROR_CLASS_MEMBER(s_Rotation)
	MIRROR_CLASS_MEMBER(s_Scale)
	// MIRROR_CLASS_MEMBER(s_Forward)
	// MIRROR_CLASS_MEMBER(s_Up)
	// MIRROR_CLASS_MEMBER(s_Right)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(EngineSettings)
	MIRROR_CLASS_MEMBER(windowWidthPixels)
	MIRROR_CLASS_MEMBER(windowHeightPixels)
	MIRROR_CLASS_MEMBER(showFps)
	MIRROR_CLASS_MEMBER(limitFramerate)
	MIRROR_CLASS_MEMBER(maxFramesPerSecond)
	MIRROR_CLASS_MEMBER(maxEnabledScenes)
	MIRROR_CLASS_MEMBER(maxConcurrentThreadCount)
	MIRROR_CLASS_MEMBER(audioEnabled)
	MIRROR_CLASS_MEMBER(physicsEnabled)
	MIRROR_CLASS_MEMBER(networkingEnabled)
	MIRROR_CLASS_MEMBER(consoleOutputWindowEnabled)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(UserSettings)
	MIRROR_CLASS_MEMBER(key_camera_MoveForward)
	MIRROR_CLASS_MEMBER(key_camera_MoveBackward)
	MIRROR_CLASS_MEMBER(key_camera_MoveLeft)
	MIRROR_CLASS_MEMBER(key_camera_MoveRight)
	MIRROR_CLASS_MEMBER(key_camera_MoveUp)
	MIRROR_CLASS_MEMBER(key_camera_MoveDown)
	MIRROR_CLASS_MEMBER(key_camera_RotateLeft)
	MIRROR_CLASS_MEMBER(key_camera_RotateRight)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(ProjectSettings)
	MIRROR_CLASS_MEMBER(projectName)
	MIRROR_CLASS_MEMBER(assetsDirPath)
	MIRROR_CLASS_MEMBER(sceneFileNames)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(SceneViewerData)
	MIRROR_CLASS_MEMBER(maxEnabledScenes)
	MIRROR_CLASS_MEMBER(maxDisplayedScenes)
	MIRROR_CLASS_MEMBER(viewDistance)
	MIRROR_CLASS_END

	// Classes and complex structs
	MIRROR_CLASS_START(Scene)
	MIRROR_CLASS_MEMBER(m_IsEnabled)
	MIRROR_CLASS_MEMBER(m_IsPaused)
	MIRROR_CLASS_MEMBER(m_SceneFileName)
	MIRROR_CLASS_MEMBER(m_ID)
	MIRROR_CLASS_MEMBER(m_CurrentCamera)
	MIRROR_CLASS_MEMBER(m_CameraList)
	MIRROR_CLASS_MEMBER(m_LightList)
	MIRROR_CLASS_MEMBER(m_SceneDrawList)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(Component)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(ComponentCamera)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(FirstPersonCameraComponent)
	MIRROR_CLASS_END

	MIRROR_CLASS_START(GameObject)
	// m_pScene not serializable
	MIRROR_CLASS_MEMBER(m_Name)
	MIRROR_CLASS_MEMBER(m_Tag)
	MIRROR_CLASS_MEMBER(m_Transform)
	// MIRROR_CLASS_MEMBER(m_BaseUpdateListSize)
	MIRROR_CLASS_MEMBER(m_UpdateList)
	// MIRROR_CLASS_MEMBER(m_BaseDrawListSize)
	MIRROR_CLASS_MEMBER(m_DrawList)
	MIRROR_CLASS_MEMBER(m_RenderOrder)
	MIRROR_CLASS_MEMBER(m_Components)
	MIRROR_CLASS_END

}
