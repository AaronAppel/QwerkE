
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/bgfx_utils.h>
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTesting.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"
#endif

#include "QC_Guid.h"

#include "QF_eKeys.h"
#include "QF_GameActions.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_Mesh.h"
#include "QF_Scene.h"

#include "QF_ComponentHelpers.h"

#include "QF_ScriptHelpers.h"

namespace QwerkE {

    using EntityComponentTypesList = MirrorTemplateArgumentList <
        ComponentCamera,
        ComponentInfo,
        ComponentMesh,
        ComponentTransform,
        ComponentScript
    >;

#ifdef _QBGFXFRAMEWORK
	typedef bgfxFramework::Mesh BgfxMesh;
	MIRROR_CLASS_START2(BgfxMesh)
	MIRROR_CLASS_END2
#endif

#ifdef _QDEARIMGUI
	MIRROR_CLASS_START2(ImVec2)
	MIRROR_CLASS_MEMBER_FLAGS2(x, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(y, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ImVec4)
	MIRROR_CLASS_MEMBER_FLAGS2(x, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(y, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(z, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(w, 0)
	MIRROR_CLASS_END2

	// #NOTE Shouldn't be needed
	// using m_imvec4_array = ImVec4[ImGuiCol_COUNT];
	// MIRROR_ARRAY(m_imvec4_array, ImVec4)

	MIRROR_CLASS_START2(ImGuiStyle)
	MIRROR_CLASS_MEMBER_FLAGS2(Alpha, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(DisabledAlpha, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(WindowPadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(WindowRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(WindowBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(WindowMinSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(WindowTitleAlign, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(WindowMenuButtonPosition, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ChildRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ChildBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(PopupRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(PopupBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(FramePadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(FrameRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(FrameBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ItemSpacing, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ItemInnerSpacing, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(CellPadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(TouchExtraPadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(IndentSpacing, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ColumnsMinSpacing, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ScrollbarSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ScrollbarRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(GrabMinSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(GrabRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(LogSliderDeadzone, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(TabRounding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(TabBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(TabMinWidthForCloseButton, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(TabBarBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(TableAngledHeadersAngle, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ColorButtonPosition, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(ButtonTextAlign, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(SelectableTextAlign, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(SeparatorTextBorderSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(SeparatorTextAlign, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(SeparatorTextPadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(DisplayWindowPadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(DisplaySafeAreaPadding, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(DockingSeparatorSize, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(MouseCursorScale, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(AntiAliasedLines, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(AntiAliasedLinesUseTex, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(AntiAliasedFill, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(CurveTessellationTol, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(CircleTessellationMaxError, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Colors, 0)
	MIRROR_CLASS_END2
#endif

#ifdef _QENTT
	// typedef entt::registry m_entt_registry;
	// MIRROR_TYPE(m_entt_registry)

	// typedef entt::entity m_enTT_Entity;
	// MIRROR_ENUM(m_enTT_Entity)
#endif

	// Enums
	// MIRROR_ENUM(eScriptTypes)
	// MIRROR_ENUM(eComponentTags)
	// MIRROR_ENUM(eKeys)

	// Vectors
	// typedef std::vector<entt::entity> m_vector_entt_entities;
	// MIRROR_VECTOR(m_vector_entt_entities, entt::entity)

	// typedef std::vector<std::string> m_vec_string;
	// MIRROR_VECTOR(m_vec_string, std::string)

	// typedef std::vector<std::string>* m_vec_string_ptr;
	// MIRROR_POINTER(m_vec_string_ptr)

	// Arrays
	// using m_arr_float16 = float[16];
	// MIRROR_ARRAY(m_arr_float16, float)

	// Maps
	// #TODO Move collections to bottom

	// Scripts
	// using CallBackFunction = void(*)(void);
	// MIRROR_TYPE(CallBackFunction)

	MIRROR_CLASS_START2(ScriptGuiButton)
	MIRROR_CLASS_MEMBER_FLAGS2(m_CallbackFunction, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ScriptableCamera)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ScriptablePatrol)
	MIRROR_CLASS_MEMBER(m_Stride)
	MIRROR_CLASS_MEMBER(m_Speed)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ScriptablePathFinder)
	MIRROR_CLASS_MEMBER_FLAGS2(m_MovementSpeed, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_DistanceToChangeTargets, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_CurrentTransformTargetIndex, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Button, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ScriptableTesting)
	MIRROR_CLASS_END2

	MIRROR_ABSTRACT_CLASS_START2(Scriptable)
	todo_MirrorSubClassUserTypes<Scriptable>(EntityComponentTypesList{}, localStaticTypeInfo);
	MIRROR_CLASS_END2

	// typedef Scriptable* m_scriptablePtr;
	// MIRROR_POINTER(m_scriptablePtr)

	// typedef	std::pair<eScriptTypes, Scriptable*> m_pair_eScriptTypes_ScriptablePtr;
	// MIRROR_PAIR(m_pair_eScriptTypes_ScriptablePtr)

	// typedef std::unordered_map<eScriptTypes, Scriptable*> m_map_eScriptTypes_ScriptablePtr;
	// MIRROR_MAP(m_map_eScriptTypes_ScriptablePtr, m_pair_eScriptTypes_ScriptablePtr)

	MIRROR_CLASS_START2(GUID)
	// #TODO Re-implement
	// MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Guid, 0)
	MIRROR_CLASS_END2

	// typedef std::pair<GUID, entt::entity> m_pair_guid_enttEntity;
	// MIRROR_PAIR(m_pair_guid_enttEntity)

	// typedef std::unordered_map<GUID, entt::entity> m_map_guid_entt;
	// MIRROR_MAP(m_map_guid_entt, m_pair_guid_enttEntity)

	// Structs
	MIRROR_CLASS_START2(vec2f)
	MIRROR_CLASS_MEMBER_FLAGS2(x, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(y, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(vec3f)
	MIRROR_CLASS_MEMBER_FLAGS2(x, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(y, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(z, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(EngineSettings)
	MIRROR_CLASS_MEMBER_FLAGS2(windowWidthPixels, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(windowHeightPixels, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(limitFramerate, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(maxFramesPerSecond, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(maxEnabledScenes, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(maxJobsAdditionalThreadCount, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(consoleOutputWindowEnabled, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(UserSettings)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(Project)
	MIRROR_CLASS_MEMBER_FLAGS2(projectFileName, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(projectName, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(startUpSceneName, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(sceneFileNames, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(RendererSettings)
	MIRROR_CLASS_MEMBER_FLAGS2(drawingPrimitiveType, 0)
	MIRROR_CLASS_END2

	// #TODO Handle dependent class
	MIRROR_CLASS_ABSTRACT_START2(Scene)
	MIRROR_CLASS_MEMBER_FLAGS2(m_SceneFileName, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Registry, 0)
	MIRROR_CLASS_END2

	// Components
	MIRROR_CLASS_START2(ComponentCamera)
	MIRROR_CLASS_MEMBER_FLAGS2(m_ShowSphere, FieldSerializationFlags::_InspectorOnly)
	MIRROR_CLASS_MEMBER_FLAGS2(m_MoveSpeed, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_LookAtPosition, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Fov, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Near, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Far, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ComponentInfo)
	MIRROR_CLASS_MEMBER_FLAGS2(m_EntityName, FieldSerializationFlags::_HideInInspector)
	// #NOTE Unserialized as needed earlier during deserialization (parent array name instead)
	// Assigned through EntityHandle constructor
	MIRROR_CLASS_MEMBER_FLAGS2(m_Guid, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Enabled, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ComponentLight)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ComponentMesh)
	MIRROR_CLASS_MEMBER_FLAGS2(m_MeshGuid, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(m_ShaderGuid, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ComponentTransform)
	MIRROR_CLASS_MEMBER_FLAGS2(m_Matrix, 0)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(ComponentScript)
	MIRROR_CLASS_MEMBER_FLAGS2(m_ScriptInstances, 0)
	MIRROR_CLASS_END2

	// Misc
	MIRROR_CLASS_START2(EntityHandle)
	MIRROR_CLASS_END2

	// Assets
	// typedef std::pair<GUID, std::string> m_pair_guid_string;
	// MIRROR_PAIR(m_pair_guid_string)

	// typedef std::vector<m_pair_guid_string> m_vec_pair_guid_string;
	// MIRROR_VECTOR(m_vec_pair_guid_string, m_pair_guid_string)

	// typedef std::pair<MirrorTypes, m_vec_pair_guid_string> m_pair_mirrorTypes_vec_pair_guid_string;
	// MIRROR_PAIR(m_pair_mirrorTypes_vec_pair_guid_string)

	// typedef std::unordered_map<MirrorTypes, m_vec_pair_guid_string> m_map_mirrorTypes_vec_pair_guid_string;
	// MIRROR_MAP(m_map_mirrorTypes_vec_pair_guid_string, m_pair_mirrorTypes_vec_pair_guid_string);

	// typedef void m_void;
	// MIRROR_POINTER(m_void)

	// typedef void* m_voidPtr;
	// MIRROR_POINTER(m_voidPtr)

	// typedef std::pair<GUID, m_voidPtr> m_pair_guid_voidPtr;
	// MIRROR_PAIR(m_pair_guid_voidPtr)

	// typedef std::unordered_map<GUID, m_voidPtr> m_map_guid_voidPtr;
	// MIRROR_MAP(m_map_guid_voidPtr, m_pair_guid_voidPtr);

	// typedef std::string* m_stringPtr;
	// MIRROR_POINTER(m_stringPtr)

	// typedef std::pair<GUID, m_stringPtr> m_pair_guid_stringPtr;
	// MIRROR_PAIR(m_pair_guid_stringPtr)

	// typedef std::unordered_map<GUID, m_stringPtr> m_map_guid_stringPtr;
	// MIRROR_MAP(m_map_guid_stringPtr, m_pair_guid_stringPtr);

	// #TODO See if empty classes can be auto-generated or implementation omitted
	MIRROR_CLASS_START2(Mesh)
	MIRROR_CLASS_END2

	MIRROR_CLASS_START2(Shader)
	MIRROR_CLASS_END2

	typedef Input::GameActions GameActions;
	MIRROR_CLASS_START2(GameActions)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_MoveForward, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_MoveBackward, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_MoveLeft, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_MoveRight, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_MoveUp, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_MoveDown, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_RotateLeft, 0)
	MIRROR_CLASS_MEMBER_FLAGS2(Camera_RotateRight, 0)
	MIRROR_CLASS_END2

}
