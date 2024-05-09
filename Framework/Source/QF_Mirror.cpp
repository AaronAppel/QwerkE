
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTesting.h"
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
#include "QF_Mesh.h"
#include "QF_Scene.h"

#include "QF_ComponentHelpers.h"

#include "QF_ScriptHelpers.h"

// Editor types
// #TODO Move editor serialization code out of framework domain
#include "../Editor/Source/QE_EditorWindowHelpers.h"
#include "../Editor/Source/QE_Editor.h"
#include "../Editor/Source/QE_Projects.h"
#include "../Editor/Source/QE_Settings.h"

namespace QwerkE {

	template <typename SuperClass, typename... SubClass>
	static void MirrorSubClass(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
	{
		uint16_t enumValue = enumStartOffset;
		([&]()
		{
			const QwerkE::Mirror::TypeInfo* subclassTypeInfo = QwerkE::Mirror::InfoForType<SubClass>();
			localStaticTypeInfo.derivedTypes.push_back(subclassTypeInfo);
			const_cast<QwerkE::Mirror::TypeInfo*>(subclassTypeInfo)->superTypeInfo = &localStaticTypeInfo;
			const_cast<QwerkE::Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc =
				[](const void* pointerToInstance) -> bool {
				SubClass* subClass = (SubClass*)pointerToInstance;
				return dynamic_cast<SubClass*>(*(SuperClass**)pointerToInstance) != nullptr;
			};
			++enumValue;
		}(), ...);
	}

	template<typename SuperClass, typename... SubClass>
	static void MirrorSubClasses(TemplateArgumentList<SubClass...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
	{
		MirrorSubClass<SuperClass, SubClass...>(localStaticTypeInfo, enumStartOffset);
	}

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

	using m_imvec4_array = ImVec4[ImGuiCol_COUNT];
	MIRROR_ARRAY(m_imvec4_array, ImVec4)

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
	MIRROR_CLASS_MEMBER(Colors)
	MIRROR_CLASS_END(ImGuiStyle)
#endif

#ifdef _QENTT
	typedef entt::registry m_entt_registry;
	MIRROR_TYPE(m_entt_registry)

	typedef entt::entity m_enTT_Entity;
	MIRROR_ENUM(m_enTT_Entity)
#endif

	// Enums
	MIRROR_ENUM(eScriptTypes)
	MIRROR_ENUM(eComponentTags)
	MIRROR_ENUM(eKeys)

	// Vectors
	typedef std::vector<entt::entity> m_vector_entt_entities;
	MIRROR_VECTOR(m_vector_entt_entities, entt::entity)

	typedef std::vector<std::string> m_vec_string;
	MIRROR_VECTOR(m_vec_string, std::string)

	typedef std::vector<std::string>* m_vec_string_ptr;
	MIRROR_POINTER(m_vec_string_ptr)

	// Arrays
	using m_arr_float16 = float[16];
	MIRROR_ARRAY(m_arr_float16, float)

	// Maps
	// #TODO Move collections to bottom

	// Scripts
	using CallBackFunction = void(*)(void);
	MIRROR_TYPE(CallBackFunction)

	MIRROR_CLASS_START(ScriptGuiButton)
	MIRROR_CLASS_MEMBER_FLAGS(m_CallbackFunction, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_END(ScriptGuiButton)

	MIRROR_CLASS_START(ScriptableCamera)
	MIRROR_CLASS_END(ScriptableCamera)

	MIRROR_CLASS_START(ScriptablePatrol)
	MIRROR_CLASS_MEMBER(m_Stride)
	MIRROR_CLASS_MEMBER(m_Speed)
	MIRROR_CLASS_END(ScriptablePatrol)

	MIRROR_CLASS_START(ScriptablePathFinder)
	MIRROR_CLASS_MEMBER(m_MovementSpeed)
	MIRROR_CLASS_MEMBER(m_DistanceToChangeTargets)
	MIRROR_CLASS_MEMBER_FLAGS(m_CurrentTransformTargetIndex, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_MEMBER_FLAGS(m_Button, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_END(ScriptablePathFinder)

	MIRROR_CLASS_START(ScriptableTesting)
	MIRROR_CLASS_END(ScriptableTesting)

	MIRROR_ABSTRACT_CLASS_START(Scriptable)
	// #TODO Look at generating empty types or not yet declared types automatically as well.
	// Would save a step when creating a new type and still allow exposing members for specific types
	MirrorSubClasses<Scriptable>(ComponentScriptsList{}, localStaticTypeInfo);
	MIRROR_CLASS_END(Scriptable)

	typedef Scriptable* m_scriptablePtr;
	MIRROR_POINTER(m_scriptablePtr)

	typedef	std::pair<eScriptTypes, Scriptable*> m_pair_eScriptTypes_ScriptablePtr;
	MIRROR_PAIR(m_pair_eScriptTypes_ScriptablePtr)

	typedef std::unordered_map<eScriptTypes, Scriptable*> m_map_eScriptTypes_ScriptablePtr;
	MIRROR_MAP(m_map_eScriptTypes_ScriptablePtr, m_pair_eScriptTypes_ScriptablePtr)

	MIRROR_CLASS_START(GUID)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_MEMBER(m_Guid)
	MIRROR_CLASS_END(GUID)

	typedef	std::pair<GUID, entt::entity> m_pair_guid_enttEntity;
	MIRROR_PAIR(m_pair_guid_enttEntity)

	typedef std::unordered_map<GUID, entt::entity> m_map_guid_entt;
	MIRROR_MAP(m_map_guid_entt, m_pair_guid_enttEntity)

	// Structs
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
	MIRROR_CLASS_MEMBER(projectFileName)
	MIRROR_CLASS_MEMBER(projectName)
	MIRROR_CLASS_MEMBER(startUpSceneName)
	MIRROR_CLASS_MEMBER(sceneFileNames)
	MIRROR_CLASS_END(Project)

	MIRROR_CLASS_START(RendererSettings)
	MIRROR_CLASS_MEMBER(drawingPrimitiveType)
	MIRROR_CLASS_END(RendererSettings)

	MIRROR_DEPENDENT_CLASS_START(Scene)
	MIRROR_CLASS_MEMBER(m_SceneFileName)
	MIRROR_CLASS_MEMBER(m_Registry)
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
	// #NOTE Unserialized as needed earlier during deserialization (parent array name instead)
	// Assigned through EntityHandle constructor
	MIRROR_CLASS_MEMBER(m_Guid)
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

	// Misc
	MIRROR_CLASS_START(EntityHandle)
	MIRROR_CLASS_END(EntityHandle)

	// Assets
	typedef std::pair<GUID, std::string> m_pair_guid_string;
	MIRROR_PAIR(m_pair_guid_string)

	typedef std::vector<m_pair_guid_string> m_vec_pair_guid_string;
	MIRROR_VECTOR(m_vec_pair_guid_string, m_pair_guid_string);

	MIRROR_CLASS_START(Mesh)
	MIRROR_CLASS_END(Mesh)

	MIRROR_CLASS_START(Shader)
	MIRROR_CLASS_END(Shader)

#ifdef _QEDITOR // #TODO Look to declare types elsewhere
	// Editor types
	typedef Editor::EditorWindowFlags EditorWindowFlags;
	MIRROR_ENUM(EditorWindowFlags)

	typedef Editor::EditorWindowTypes EditorWindowTypes;
	MIRROR_ENUM(EditorWindowTypes)

	typedef Editor::EditorWindowAssets EditorWindowAssets;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowAssets)

	typedef Editor::EditorWindowDefaultDebug EditorWindowDefaultDebug;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowDefaultDebug)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowDefaultDebug)

	typedef Editor::EditorWindowDockingContext EditorWindowDockingContext;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowDockingContext)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowDockingContext)

	typedef Editor::EditorWindowEntityInspector EditorWindowEntityInspector;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowEntityInspector)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowEntityInspector)

	typedef Editor::EditorWindowImGuiDemo EditorWindowImGuiDemo;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiDemo)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowImGuiDemo)

	typedef Editor::EditorWindowMenuBar EditorWindowMenuBar;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowMenuBar)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowMenuBar)

	typedef Editor::EditorWindowSceneControls EditorWindowSceneControls;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneControls)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowSceneControls)

	typedef Editor::EditorWindowSceneGraph EditorWindowSceneGraph;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneGraph)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowSceneGraph)

	typedef Editor::EditorWindowSceneView EditorWindowSceneView;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneView)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_MEMBER(m_TextureId)
	MIRROR_CLASS_MEMBER(m_ViewId)
	MIRROR_CLASS_END(EditorWindowSceneView)

	typedef Editor::EditorWindowSettings EditorWindowSettings;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSettings)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowSettings)

	typedef Editor::EditorWindowStylePicker EditorWindowStylePicker;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowStylePicker)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_MEMBER(m_ShowMoreInfo)
	MIRROR_CLASS_MEMBER(m_UiScaling)
	MIRROR_CLASS_END(EditorWindowStylePicker)

	typedef Editor::EditorWindowMaterialEditor EditorWindowMaterialEditor;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowMaterialEditor)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowMaterialEditor)

	typedef Editor::EditorWindowFolderViewer EditorWindowFolderViewer;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowFolderViewer)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowFolderViewer)

	typedef Editor::EditorWindowNodeEditor EditorWindowNodeEditor;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowNodeEditor)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowNodeEditor)

	typedef Editor::EditorWindowShaderEditor EditorWindowShaderEditor;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowShaderEditor)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowShaderEditor)

	typedef Editor::EditorWindowPrefabScene EditorWindowPrefabScene;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowPrefabScene)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowPrefabScene)

	typedef Editor::EditorWindowPrompt EditorWindowPrompt;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowPrompt)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowPrompt)

	typedef Editor::EditorWindow EditorWindow;
	MIRROR_ABSTRACT_CLASS_START(EditorWindow)
	MIRROR_CLASS_MEMBER(m_Guid) // Order dependency! 1st // #TODO Review dependency
	MIRROR_CLASS_MEMBER(m_WindowName)
	MIRROR_CLASS_MEMBER(m_ImGuiFlags)
	MIRROR_CLASS_MEMBER(m_WindowFlags)
	MIRROR_CLASS_MEMBER(m_MinimumWidth)
	MIRROR_CLASS_MEMBER(m_MinimumHeight)
	MIRROR_CLASS_MEMBER(m_EditorWindowType)

	// #TODO Solve generic templated type list issue
	// using WindowsList = MirrorTemplateArgumentList <EditorWindowAssets>;
	// MirrorSubClasses<EditorWindow>(WindowsList{}, localStaticTypeInfo, Editor::EditorWindowTypes::Assets);
	MirrorSubClasses<EditorWindow>(Editor::EditorWindowsList{}, localStaticTypeInfo, Editor::EditorWindowTypes::Assets);
	MIRROR_CLASS_END(EditorWindow)

	typedef Editor::EditorWindow* m_editorWindowPtr;
	MIRROR_POINTER(m_editorWindowPtr)

	typedef std::pair<GUID, m_editorWindowPtr> m_pair_guid_editorWindowPtr;
	MIRROR_PAIR(m_pair_guid_editorWindowPtr)

	typedef std::unordered_map<GUID, m_editorWindowPtr> m_umap_guid_editorWindowPtr;
	MIRROR_MAP(m_umap_guid_editorWindowPtr, m_pair_guid_editorWindowPtr)

	MIRROR_CLASS_START(ProjectsData)
	MIRROR_CLASS_MEMBER(LastOpenedProjectFileName)
	MIRROR_CLASS_MEMBER(PreviousProjectFileNames)
	MIRROR_CLASS_END(ProjectsData)
#endif

}
