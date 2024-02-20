
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

#include "QC_Vector.h"

#include "QF_eKeys.h"
#include "QF_Enums.h"
#include "QF_GameObject.h"
#include "QF_Material.h"
#include "QF_Scene.h"
#include "QF_Settings.h"
#include "QF_ShaderProgram.h"
#include "QF_Texture.h"

#include "QF_Component.h"
#include "QF_CameraComponent.h"
#include "QF_LightComponent.h"
#include "QF_RenderComponent.h"

#include "QF_Routine.h"
#include "QF_TransformRoutine.h"
#include "QF_RenderRoutine.h"

namespace QwerkE {

	// Enums
	typedef eGameObjectTags m_eGameObjectTags;
	MIRROR_TYPE(m_eGameObjectTags)

	typedef eComponentTags m_eComponentTags;
	MIRROR_TYPE(m_eComponentTags)

	typedef QwerkE::eKeys eKeys;
	MIRROR_TYPE(eKeys)

	// Pointers
	typedef Routine* m_routinePtr;
	MIRROR_TYPE(m_routinePtr)

	typedef RenderRoutine* m_renderRoutinePtr;
	MIRROR_TYPE(m_renderRoutinePtr)

	typedef TransformRoutine* m_transformRoutinePtr; // #TODO Needed?
	MIRROR_TYPE(m_transformRoutinePtr)

	// Vectors
	typedef std::vector<std::string> m_vector_string;
	MIRROR_TYPE(m_vector_string)

	typedef std::vector<GameObject*> m_vector_gameobjectPtr;
	MIRROR_TYPE(m_vector_gameobjectPtr)

	typedef std::vector<Routine*> m_vector_routinePtr;
	MIRROR_TYPE(m_vector_routinePtr)

	typedef std::vector<Renderable> m_vector_renderable;
	MIRROR_TYPE(m_vector_renderable)

	// Maps
	typedef std::map<eComponentTags, Component*> m_map_eComponentTags_componentPtr;
	MIRROR_TYPE(m_map_eComponentTags_componentPtr)

	typedef std::map<eMaterialMaps, Texture*> m_map_eMaterialMaps_texturePtr;
	MIRROR_TYPE(m_map_eMaterialMaps_texturePtr)

	// Structs
	MIRROR_CLASS_START(Vector3)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_MEMBER(z)
	MIRROR_CLASS_END(Vector3)

	MIRROR_CLASS_START(Vector2)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_END(Vector2)

	MIRROR_CLASS_START(Transform)
	MIRROR_CLASS_MEMBER(s_Position)
	MIRROR_CLASS_MEMBER(s_Rotation)
	MIRROR_CLASS_MEMBER(s_Scale)
	MIRROR_CLASS_END(Transform)

	MIRROR_CLASS_START(ShaderProgram)
	MIRROR_CLASS_MEMBER(m_Name)
	MIRROR_CLASS_MEMBER(m_VertName)
	MIRROR_CLASS_MEMBER(m_FragName)
	MIRROR_CLASS_MEMBER(m_GeoName)
	MIRROR_CLASS_END(ShaderProgram)

	MIRROR_CLASS_START(Material)
	MIRROR_CLASS_MEMBER(m_Name)
	MIRROR_CLASS_MEMBER(m_Components)
	MIRROR_CLASS_END(Material)

	MIRROR_CLASS_START(Texture)
	MIRROR_CLASS_MEMBER(s_FileName)
	MIRROR_CLASS_END(Texture)

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

	// GameObject components
	MIRROR_CLASS_START(Component)
	MIRROR_CLASS_END(Component)

	MIRROR_SUB_CLASS_START(ComponentCamera, Component)
	MIRROR_CLASS_MEMBER(m_ViewportSize)
	MIRROR_CLASS_END(ComponentCamera)

	MIRROR_CLASS_START(LightComponent)
	MIRROR_CLASS_END(LightComponent)

	MIRROR_CLASS_START(RenderComponent)
	MIRROR_CLASS_MEMBER(m_SchematicName)
	MIRROR_CLASS_MEMBER(m_Renderables)
	MIRROR_CLASS_END(RenderComponent)

	// GameObject routines
	MIRROR_CLASS_START(Routine)
	MIRROR_CLASS_MEMBER(m_Enabled)
	MIRROR_CLASS_END(Routine)

	MIRROR_CLASS_START(RenderRoutine)
	MIRROR_CLASS_END(RenderRoutine)

	MIRROR_CLASS_START(TransformRoutine)
	MIRROR_CLASS_MEMBER(m_Speed)
	MIRROR_CLASS_MEMBER(m_PositionOffset)
	MIRROR_CLASS_MEMBER(m_RotationOffset)
	MIRROR_CLASS_MEMBER(m_ScaleOffset)
	MIRROR_CLASS_END(TransformRoutine)

	// More classes and complex structs
	MIRROR_CLASS_START(Renderable)
	MIRROR_CLASS_MEMBER(m_RenderableName)
	MIRROR_CLASS_MEMBER(m_ShaderName)
	MIRROR_CLASS_MEMBER(m_MaterialName)
	MIRROR_CLASS_MEMBER(m_MeshFileName)
	MIRROR_CLASS_END(Renderable)

	MIRROR_CLASS_START(Scene)
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
	MIRROR_CLASS_MEMBER(m_Transform)
	MIRROR_CLASS_MEMBER(m_UpdateList)
	MIRROR_CLASS_MEMBER(m_DrawList)
	MIRROR_CLASS_MEMBER(m_Components)
	MIRROR_CLASS_MEMBER(m_RenderOrder)
	MIRROR_CLASS_END(GameObject)

}
