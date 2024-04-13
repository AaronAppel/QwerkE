
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#include "QC_Guid.h"

#include "QF_eKeys.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_Projects.h"
#include "QF_Mesh.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

#include "QF_ComponentCamera.h"
#include "QF_ComponentInfo.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

#include "QF_ScriptPatrol.h"

namespace QwerkE {

#ifdef _QDEARIMGUI
	MIRROR_CLASS_START(ImVec2)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_END(ImVec2)

	MIRROR_CLASS_START(ImVec4)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_MEMBER(z)
	MIRROR_CLASS_MEMBER(w)
	MIRROR_CLASS_END(ImVec4)

	typedef ImVec4* m_imvec4_array; // #TODO try : using m_imvec4_array = ImVec4[4]; to see if it works any better
	MIRROR_TYPE(m_imvec4_array)

	MIRROR_CLASS_START(ImGuiStyle)
	MIRROR_CLASS_MEMBER(Alpha)
	MIRROR_CLASS_MEMBER(DisabledAlpha)
	MIRROR_CLASS_MEMBER(WindowPadding)
	MIRROR_CLASS_MEMBER(WindowRounding)
	MIRROR_CLASS_MEMBER(WindowBorderSize)
	MIRROR_CLASS_MEMBER(WindowMinSize)
	MIRROR_CLASS_MEMBER(WindowTitleAlign)
	MIRROR_CLASS_MEMBER(WindowMenuButtonPosition)
	MIRROR_CLASS_MEMBER(ChildRounding)
	MIRROR_CLASS_MEMBER(ChildBorderSize)
	MIRROR_CLASS_MEMBER(PopupRounding)
	MIRROR_CLASS_MEMBER(PopupBorderSize)
	MIRROR_CLASS_MEMBER(FramePadding)
	MIRROR_CLASS_MEMBER(FrameRounding)
	MIRROR_CLASS_MEMBER(FrameBorderSize)
	MIRROR_CLASS_MEMBER(ItemSpacing)
	MIRROR_CLASS_MEMBER(ItemInnerSpacing)
	MIRROR_CLASS_MEMBER(CellPadding)
	MIRROR_CLASS_MEMBER(TouchExtraPadding)
	MIRROR_CLASS_MEMBER(IndentSpacing)
	MIRROR_CLASS_MEMBER(ColumnsMinSpacing)
	MIRROR_CLASS_MEMBER(ScrollbarSize)
	MIRROR_CLASS_MEMBER(ScrollbarRounding)
	MIRROR_CLASS_MEMBER(GrabMinSize)
	MIRROR_CLASS_MEMBER(GrabRounding)
	MIRROR_CLASS_MEMBER(LogSliderDeadzone)
	MIRROR_CLASS_MEMBER(TabRounding)
	MIRROR_CLASS_MEMBER(TabBorderSize)
	MIRROR_CLASS_MEMBER(TabMinWidthForCloseButton)
	MIRROR_CLASS_MEMBER(TabBarBorderSize)
	MIRROR_CLASS_MEMBER(TableAngledHeadersAngle)
	MIRROR_CLASS_MEMBER(ColorButtonPosition)
	MIRROR_CLASS_MEMBER(ButtonTextAlign)
	MIRROR_CLASS_MEMBER(SelectableTextAlign)
	MIRROR_CLASS_MEMBER(SeparatorTextBorderSize)
	MIRROR_CLASS_MEMBER(SeparatorTextAlign)
	MIRROR_CLASS_MEMBER(SeparatorTextPadding)
	MIRROR_CLASS_MEMBER(DisplayWindowPadding)
	MIRROR_CLASS_MEMBER(DisplaySafeAreaPadding)
	MIRROR_CLASS_MEMBER(DockingSeparatorSize)
	MIRROR_CLASS_MEMBER(MouseCursorScale)
	MIRROR_CLASS_MEMBER(AntiAliasedLines)
	MIRROR_CLASS_MEMBER(AntiAliasedLinesUseTex)
	MIRROR_CLASS_MEMBER(AntiAliasedFill)
	MIRROR_CLASS_MEMBER(CurveTessellationTol)
	MIRROR_CLASS_MEMBER(CircleTessellationMaxError)
	MIRROR_CLASS_MEMBER_TYPE_OVERRIDE(Colors, m_imvec4_array)
	MIRROR_CLASS_END(ImGuiStyle)
#endif

#ifdef _QENTT
	typedef entt::entity m_enTT_Entity;
	MIRROR_TYPE(m_enTT_Entity)
#endif

	// Enums
	MIRROR_TYPE(eScriptTypes)
	MIRROR_TYPE(eComponentTags)
	MIRROR_TYPE(eKeys)

	// Vectors
	typedef std::vector<entt::entity> m_vector_entt_entities;
	MIRROR_TYPE(m_vector_entt_entities)

	typedef std::vector<std::string> m_vector_string;
	MIRROR_TYPE(m_vector_string)

	// Arrays
	using m_floatArray16 = float[16]; // #TODO Review hard coded size, and name
	MIRROR_TYPE(m_floatArray16)

	// Maps
	typedef std::unordered_map<eScriptTypes, Scriptable*> m_map_eScriptTypes_ScriptablePtr;
	MIRROR_TYPE(m_map_eScriptTypes_ScriptablePtr)

	typedef std::unordered_map<GUID, entt::entity> m_map_guid_entt;
	MIRROR_TYPE(m_map_guid_entt)

	// Structs
	MIRROR_CLASS_START(GUID)
	MIRROR_CLASS_MEMBER(m_Guid)
	MIRROR_CLASS_END(GUID)

	MIRROR_CLASS_START(vec2f)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_END(vec2f)

	MIRROR_CLASS_START(vec3f)
	MIRROR_CLASS_MEMBER(x)
	MIRROR_CLASS_MEMBER(y)
	MIRROR_CLASS_MEMBER(z)
	MIRROR_CLASS_END(vec3f)

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
	MIRROR_CLASS_MEMBER(showingStylePicker)
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

	MIRROR_CLASS_START(Project)
	MIRROR_CLASS_MEMBER(projectName)
	MIRROR_CLASS_MEMBER(assetsDirPath)
	MIRROR_CLASS_MEMBER(sceneFileNames)
	MIRROR_CLASS_END(Project)

	MIRROR_CLASS_START(RendererSettings)
	MIRROR_CLASS_MEMBER(drawingPrimitiveType)
	MIRROR_CLASS_END(RendererSettings)

	MIRROR_CLASS_START(Scene)
	MIRROR_CLASS_MEMBER(m_SceneFileName)
	MIRROR_CLASS_MEMBER(m_GuidsToEntts)
	MIRROR_CLASS_END(Scene)

	// Components
	MIRROR_CLASS_START(ComponentCamera)
	MIRROR_CLASS_MEMBER(m_ShowSphere)
	MIRROR_CLASS_MEMBER(m_MoveSpeed)
	MIRROR_CLASS_MEMBER(m_At)
	MIRROR_CLASS_MEMBER(m_Fov)
	MIRROR_CLASS_MEMBER(m_Near)
	MIRROR_CLASS_MEMBER(m_Far)
	MIRROR_CLASS_END(ComponentCamera)

	MIRROR_CLASS_START(ComponentInfo)
	MIRROR_CLASS_MEMBER(m_EditorDisplayName)
	// MIRROR_CLASS_MEMBER(m_Guid)	// #NOTE Unserialized as needed earlier (array name instead)
									// and current cJSON implementation doesn't serialize 64 bit values
	MIRROR_CLASS_MEMBER(m_Enabled)
	MIRROR_CLASS_END(ComponentInfo)

	MIRROR_CLASS_START(ComponentLight)
	MIRROR_CLASS_END(ComponentLight)

	MIRROR_CLASS_START(ComponentMesh)
	MIRROR_CLASS_MEMBER(m_MeshGuid)
	MIRROR_CLASS_MEMBER(m_ShaderGuid)
	MIRROR_CLASS_END(ComponentMesh)

	MIRROR_CLASS_START(ComponentTransform)
	MIRROR_CLASS_MEMBER(m_Matrix)
	MIRROR_CLASS_END(ComponentTransform)

	MIRROR_CLASS_START(ComponentScript)
	MIRROR_CLASS_MEMBER(m_ScriptInstances)
	MIRROR_CLASS_END(ComponentScript)

	// Scripts
	MIRROR_CLASS_START(ScriptablePatrol)
	MIRROR_CLASS_MEMBER(m_Stride)
	MIRROR_CLASS_MEMBER(m_Speed)
	MIRROR_CLASS_END(ScriptablePatrol)

	// Misc
	MIRROR_CLASS_START(EntityHandle)
	MIRROR_CLASS_END(EntityHandle)

	// Assets
	typedef std::pair<GUID, std::string> m_pair_guid_string;
	MIRROR_TYPE(m_pair_guid_string);

	typedef std::vector<std::pair<GUID, std::string>> m_vec_pair_guid_string;
	MIRROR_TYPE(m_vec_pair_guid_string);

	MIRROR_CLASS_START(Mesh)
	MIRROR_CLASS_END(Mesh)

	MIRROR_CLASS_START(Shader)
	MIRROR_CLASS_END(Shader)
}
