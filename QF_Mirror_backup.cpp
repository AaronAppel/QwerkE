
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

#include "QF_ComponentHelpers.h"

#include "QF_Scriptable.h"
#include "QF_ScriptPathFinder.h"
#include "QF_ScriptPatrol.h"

// Editor types
// #TODO Move serialization code out of framework domain
#include "../Editor/Source/QE_EditorWindowHelpers.h"

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
	MIRROR_CLASS_MEMBER(lastOpenedProjectFileName)
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
	MIRROR_CLASS_MEMBER_FLAGS(m_ShowSphere, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_MEMBER(m_MoveSpeed)
	MIRROR_CLASS_MEMBER(m_LookAtPosition)
	MIRROR_CLASS_MEMBER(m_Fov)
	MIRROR_CLASS_MEMBER(m_Near)
	MIRROR_CLASS_MEMBER(m_Far)
	MIRROR_CLASS_END(ComponentCamera)

	MIRROR_CLASS_START(ComponentInfo)
	MIRROR_CLASS_MEMBER_FLAGS(m_EntityName, FieldSerializationFlags::_HideInInspector)
	// MIRROR_CLASS_MEMBER(m_Guid)	// #NOTE Unserialized as needed earlier during deserialization (parent array name instead)
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
	using CallBackFunction = void(*)(void);
	MIRROR_TYPE(CallBackFunction)

	MIRROR_CLASS_START(ScriptGuiButton)
	MIRROR_CLASS_MEMBER_FLAGS(m_CallbackFunction, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_END(ScriptGuiButton)

	MIRROR_CLASS_START(ScriptablePatrol)
	MIRROR_CLASS_MEMBER(m_Stride)
	MIRROR_CLASS_MEMBER(m_Speed)
	MIRROR_CLASS_END(ScriptablePatrol)

	MIRROR_CLASS_START(ScriptablePathFinder)
	MIRROR_CLASS_MEMBER(m_MovementSpeed)
	MIRROR_CLASS_MEMBER(m_DistanceToChangeTargets)
	MIRROR_CLASS_MEMBER_FLAGS(m_CurrentTransformTargetIndex, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_MEMBER_FLAGS(m_Button, FieldSerializationFlags::_InspectorOnly)
	// #TODO Serialize guids or entity handle references to show/edit in GUI m_Transforms
	MIRROR_CLASS_END(ScriptablePathFinder)

	// Misc
	MIRROR_CLASS_START(EntityHandle)
	MIRROR_CLASS_END(EntityHandle)

	MIRROR_CLASS_START(Mesh)
	MIRROR_CLASS_END(Mesh)

	MIRROR_CLASS_START(Shader)
	MIRROR_CLASS_END(Shader)

	// Editor types
	typedef Editor::EditorWindowFlags EditorWindowFlags;
	MIRROR_TYPE(EditorWindowFlags)

	typedef Editor::EditorWindowTypes EditorWindowTypes;
	MIRROR_TYPE(EditorWindowTypes)

	typedef Editor::EditorWindowAssets EditorWindowAssets;
	MIRROR_CLASS_START(EditorWindowAssets)
	MIRROR_CLASS_END(EditorWindowAssets)

	typedef Editor::EditorWindowDefaultDebug EditorWindowDefaultDebug;
	MIRROR_CLASS_START(EditorWindowDefaultDebug)
	MIRROR_CLASS_END(EditorWindowDefaultDebug)

	typedef Editor::EditorWindowDockingContext EditorWindowDockingContext;
	MIRROR_CLASS_START(EditorWindowDockingContext)
	MIRROR_CLASS_END(EditorWindowDockingContext)

	typedef Editor::EditorWindowEntityInspector EditorWindowEntityInspector;
	MIRROR_CLASS_START(EditorWindowEntityInspector)
	MIRROR_CLASS_END(EditorWindowEntityInspector)

	typedef Editor::EditorWindowImGuiDemo EditorWindowImGuiDemo;
	MIRROR_CLASS_START(EditorWindowImGuiDemo)
	MIRROR_CLASS_END(EditorWindowImGuiDemo)

	typedef Editor::EditorWindowMenuBar EditorWindowMenuBar;
	MIRROR_CLASS_START(EditorWindowMenuBar)
	MIRROR_CLASS_END(EditorWindowMenuBar)

	typedef Editor::EditorWindowSceneControls EditorWindowSceneControls;
	MIRROR_CLASS_START(EditorWindowSceneControls)
	MIRROR_CLASS_END(EditorWindowSceneControls)

	typedef Editor::EditorWindowSceneGraph EditorWindowSceneGraph;
	MIRROR_CLASS_START(EditorWindowSceneGraph)
	MIRROR_CLASS_END(EditorWindowSceneGraph)

	typedef Editor::EditorWindowSceneView EditorWindowSceneView;
	MIRROR_CLASS_START(EditorWindowSceneView)
	MIRROR_CLASS_MEMBER(m_TextureId)
	MIRROR_CLASS_MEMBER(m_ViewId)
	MIRROR_CLASS_END(EditorWindowSceneView)

	typedef Editor::EditorWindowSettings EditorWindowSettings;
	MIRROR_CLASS_START(EditorWindowSettings)
	MIRROR_CLASS_END(EditorWindowSettings)

	typedef Editor::EditorWindowStylePicker EditorWindowStylePicker;
	MIRROR_CLASS_START(EditorWindowStylePicker)
	MIRROR_CLASS_END(EditorWindowStylePicker)

	typedef Editor::EditorWindowMaterialEditor EditorWindowMaterialEditor;
	MIRROR_CLASS_START(EditorWindowMaterialEditor)
	MIRROR_CLASS_END(EditorWindowMaterialEditor)

	typedef Editor::EditorWindowFolderViewer EditorWindowFolderViewer;
	MIRROR_CLASS_START(EditorWindowFolderViewer)
	MIRROR_CLASS_END(EditorWindowFolderViewer)

	typedef Editor::EditorWindowNodeEditor EditorWindowNodeEditor;
	MIRROR_CLASS_START(EditorWindowNodeEditor)
	MIRROR_CLASS_END(EditorWindowNodeEditor)

	typedef Editor::EditorWindowShaderEditor EditorWindowShaderEditor;
	MIRROR_CLASS_START(EditorWindowShaderEditor)
	MIRROR_CLASS_END(EditorWindowShaderEditor)

	typedef Editor::EditorWindow EditorWindow;
	MIRROR_CLASS_START(EditorWindow)
	MIRROR_CLASS_MEMBER(m_Guid) // Order dependency! 1st
	MIRROR_CLASS_MEMBER(m_WindowName)
	MIRROR_CLASS_MEMBER(m_ImGuiFlags)
	MIRROR_CLASS_MEMBER(m_WindowFlags)
	MIRROR_CLASS_MEMBER(m_MinimumWidth)
	MIRROR_CLASS_MEMBER(m_MinimumHeight)
	MIRROR_CLASS_MEMBER(m_EditorWindowType)

	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowAssets, Editor::EditorWindowTypes::Assets)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowDefaultDebug, Editor::EditorWindowTypes::DefaultDebug)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowDockingContext, Editor::EditorWindowTypes::DockingContext)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowEntityInspector, Editor::EditorWindowTypes::EntityInspector)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowImGuiDemo, Editor::EditorWindowTypes::ImGuiDemo)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowMenuBar, Editor::EditorWindowTypes::MenuBar)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowSceneControls, Editor::EditorWindowTypes::SceneControls)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowSceneGraph, Editor::EditorWindowTypes::SceneGraph)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowSceneView, Editor::EditorWindowTypes::SceneView)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowSettings, Editor::EditorWindowTypes::Settings)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowStylePicker, Editor::EditorWindowTypes::StylePicker)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowMaterialEditor, Editor::EditorWindowTypes::MaterialEditor)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowFolderViewer, Editor::EditorWindowTypes::FolderViewer)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowNodeEditor, Editor::EditorWindowTypes::NodeEditor)
	MIRROR_CLASS_SUBCLASS_USER_TYPE(EditorWindowShaderEditor, Editor::EditorWindowTypes::ShaderEditor)
	MIRROR_CLASS_END(EditorWindow)

	// Pointers
	typedef Editor::EditorWindow* m_editorWindowPtr;
	MIRROR_POINTER(m_editorWindowPtr)

	typedef Scriptable* m_scriptablePtr;
	MIRROR_POINTER(m_scriptablePtr)

	// Pairs
	typedef std::pair<GUID, entt::entity> m_pair_guid_enttEntity;
	MIRROR_PAIR(m_pair_guid_enttEntity, GUID, entt::entity)

	typedef std::pair<GUID, m_editorWindowPtr> m_pair_guid_editorWindowPtr;
	MIRROR_PAIR(m_pair_guid_editorWindowPtr, GUID, m_editorWindowPtr)

	typedef std::pair<eScriptTypes, Scriptable*> m_pair_eScriptTypes_ScriptablePtr;
	MIRROR_PAIR(m_pair_eScriptTypes_ScriptablePtr, eScriptTypes, Scriptable*)

	typedef std::pair<GUID, std::string> m_pair_guid_string;
	MIRROR_PAIR(m_pair_guid_string, GUID, std::string);

	// Vectors
	typedef std::vector<m_pair_guid_string> m_vec_pair_guid_string;
	MIRROR_VECTOR(m_vec_pair_guid_string, m_pair_guid_string);

	typedef std::vector<entt::entity> m_vector_entt_entities;
	MIRROR_VECTOR(m_vector_entt_entities, entt::entity)

	typedef std::vector<std::string> m_vec_string;
	MIRROR_VECTOR(m_vec_string, std::string)

	// Arrays
	using m_arr_float16 = float[16]; // #TODO Review hard coded size, and name
	MIRROR_ARRAY(m_arr_float16, float)

	// Maps
	typedef std::unordered_map<eScriptTypes, Scriptable*> m_map_eScriptTypes_ScriptablePtr;
	MIRROR_MAP(m_map_eScriptTypes_ScriptablePtr, m_map_eScriptTypes_ScriptablePtr)

	typedef std::unordered_map<GUID, entt::entity> m_map_guid_entt;
	MIRROR_MAP(m_map_guid_entt, m_pair_guid_enttEntity)

	typedef std::unordered_map<GUID, m_editorWindowPtr> m_umap_guid_editorWindowPtr;
	MIRROR_MAP(m_umap_guid_editorWindowPtr, m_pair_guid_editorWindowPtr)
}