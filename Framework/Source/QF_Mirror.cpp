
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#include "Libraries/entt/entt.hpp"
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

#include "QF_eKeys.h"
#include "QF_Enums.h"
#include "QF_GameObject.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

namespace QwerkE {

	// External / 3rd party types
	typedef entt::registry m_entt_registry;
	MIRROR_TYPE(m_entt_registry)

	typedef entt::entity m_enTT_Entity;
	MIRROR_TYPE(m_enTT_Entity)

	// Enums
	MIRROR_TYPE(eGameObjectTags)
	MIRROR_TYPE(eComponentTags)

	typedef QwerkE::eKeys eKeys; // #TODO Review typedef
	MIRROR_TYPE(eKeys)

	// Pointers

	// Structs

	// Vectors
	typedef std::vector<eComponentTags> m_vector_eComponentTags;
	MIRROR_TYPE(m_vector_eComponentTags)

	typedef std::vector<entt::entity> m_vector_entt_entities;
	MIRROR_TYPE(m_vector_entt_entities)

	typedef std::vector<std::string> m_vector_string;
	MIRROR_TYPE(m_vector_string)

	typedef std::vector<GameObject*> m_vector_gameobjectPtr;
	MIRROR_TYPE(m_vector_gameobjectPtr)

	// Maps

	// Structs
	MIRROR_CLASS_START(vec2)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_END(vec2)

	MIRROR_CLASS_START(vec3)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_MEMBER(z)
	MIRROR_CLASS_END(vec3)

	MIRROR_CLASS_START(EngineSettings)
	MIRROR_CLASS_MEMBER(windowWidthPixels)
	MIRROR_CLASS_MEMBER(windowHeightPixels)
	MIRROR_CLASS_MEMBER(limitFramerate)
	MIRROR_CLASS_MEMBER(maxFramesPerSecond)
	MIRROR_CLASS_MEMBER(maxEnabledScenes)
	MIRROR_CLASS_MEMBER(maxJobsAdditionalThreadCount)
	MIRROR_CLASS_MEMBER(consoleOutputWindowEnabled)
	MIRROR_CLASS_MEMBER(showingSchematicsEditor)
	MIRROR_CLASS_MEMBER(showingExampleWindow)
	MIRROR_CLASS_MEMBER(showingSettingsEditor)
	MIRROR_CLASS_MEMBER(showingFPS)
	MIRROR_CLASS_END(EngineSettings)

	MIRROR_CLASS_START(UserSettings)
	MIRROR_CLASS_MEMBER(key_camera_MoveForward)
	MIRROR_CLASS_MEMBER(key_camera_MoveBackward)
	MIRROR_CLASS_MEMBER(key_camera_MoveLeft)
	MIRROR_CLASS_MEMBER(key_camera_MoveRight)
	MIRROR_CLASS_MEMBER(key_camera_MoveUp)
	MIRROR_CLASS_MEMBER(key_camera_MoveDown)
	MIRROR_CLASS_MEMBER(key_camera_RotateLeft)
	MIRROR_CLASS_MEMBER(key_camera_RotateRight)
	MIRROR_CLASS_END(UserSettings)

	MIRROR_CLASS_START(ProjectSettings)
	MIRROR_CLASS_MEMBER(projectName)
	MIRROR_CLASS_MEMBER(assetsDirPath)
	MIRROR_CLASS_MEMBER(sceneFileNames)
	MIRROR_CLASS_END(ProjectSettings)

	MIRROR_CLASS_START(RendererSettings)
	MIRROR_CLASS_MEMBER(drawingPrimitiveType)
	MIRROR_CLASS_END(RendererSettings)

	MIRROR_CLASS_START(Scene)
	MIRROR_CLASS_MEMBER(m_Registry)
	MIRROR_CLASS_MEMBER(m_SceneFileName)
	MIRROR_CLASS_MEMBER(m_IsPaused)
	MIRROR_CLASS_MEMBER(m_CurrentCameraIndex)
	MIRROR_CLASS_MEMBER(m_CameraList)
	MIRROR_CLASS_MEMBER(m_LightList)
	MIRROR_CLASS_MEMBER(m_SceneDrawList)
	MIRROR_CLASS_END(Scene)

	MIRROR_CLASS_START(GameObject)
	// m_pScene not serializable
	MIRROR_CLASS_MEMBER(m_Enabled)
	MIRROR_CLASS_MEMBER(m_Name)
	MIRROR_CLASS_MEMBER(m_Tag)
	// MIRROR_CLASS_MEMBER(m_EntityComponents)
	MIRROR_CLASS_END(GameObject)

}
