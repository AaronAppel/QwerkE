#pragma once

#include <array>
#include <unordered_map>
#include <string>
#include <vector>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_TypeIds.h"
#endif

#include "QC_Guid.h"

#include "QF_QKey.h"
#include "QF_GameActions.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_Input.h"
#include "QF_Mesh.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

#include "QF_ComponentHelpers.h"

#include "QF_ScriptHelpers.h"

enum FieldSerializationFlags : MIRROR_FIELD_FLAG_TYPE
{
	_None = 0,

	// View, read, and write functionality is enabled by default (inclusive) for all types
	_HideInInspector = 1 << 0,						// Serialize but don't show in editor UI
	_InspectorOnly = 1 << 1,						// Do not serialize the value
	_InspectorViewOnly = 1 << 2,					// Do not allow changes using editor UI

	// Drag and drop functionality is enabled by default (inclusive) for all types
	// #TODO Payload disabled not yet supported/checked
	_InspectorDisableDragAndDropTarget = 1 << 3,	// Cannot be modified by drag and drop payload
	// _InSpectorDisableDragAndDropSource = 1 << 4,	// Cannot be used to provide/create a drag and drop payload
};

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code over time

/* START */
// Serialized (value dependent) type IDs
MIRROR_TYPE_ID(158, bgfxFramework::Mesh)
MIRROR_TYPE_ID(159, QwerkE::Shader)
MIRROR_TYPE_ID(160, QwerkE::Mesh)
MIRROR_TYPE_ID(161, QwerkE::Scene)
/* END */

// #NOTE Framework Ids start at +1000

// Non-serialized (value independent) type IDs
// ImGui
MIRROR_TYPE_ID(1004, ImVec2)
MIRROR_TYPE_ID(1005, ImVec4)
MIRROR_TYPE_ID(1006, ImVec4[55])
MIRROR_TYPE_ID(1007, ImGuiStyle)

MIRROR_TYPE_ID(1008, entt::registry)
MIRROR_TYPE_ID(1009, entt::entity)

// QC Types
MIRROR_TYPE_ID(1010, QwerkE::Time::Timer)

MIRROR_TYPE_ID(1011, vec3f)
MIRROR_TYPE_ID(1012, vec2f)

// Function pointers
MIRROR_TYPE_ID(1013, void(*)(void))

// Enums
MIRROR_TYPE_ID(1014, QwerkE::QKey)
MIRROR_TYPE_ID(1015, QwerkE::eComponentTags)

// Collections
MIRROR_TYPE_ID(1016, float[16])

MIRROR_TYPE_ID(1017, std::pair<QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(1018, std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(1019, std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(1020, std::pair<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(1021, std::pair<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(1022, std::pair<const QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(1023, std::pair<const QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(1024, std::pair<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(1025, std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(1026, std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(1027, std::pair<std::string, std::string>);
MIRROR_TYPE_ID(1028, std::pair<QwerkE::GUID, std::pair<std::string, std::string>>);
MIRROR_TYPE_ID(1029, std::pair<QwerkE::GUID, std::string[11]>)
MIRROR_TYPE_ID(1030, std::pair<QwerkE::GUID, std::vector<std::string>>)
MIRROR_TYPE_ID(1031, std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE_ID(1032, std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)

MIRROR_TYPE_ID(1033, std::vector<std::string>)
MIRROR_TYPE_ID(1034, std::vector<std::string*>)
MIRROR_TYPE_ID(1035, std::vector<std::pair<QwerkE::GUID, std::string>>)
MIRROR_TYPE_ID(1036, std::vector<entt::entity>)
MIRROR_TYPE_ID(1037, std::vector<std::pair<QwerkE::GUID, std::pair<std::string, std::string>>>)
MIRROR_TYPE_ID(1038, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>)

MIRROR_TYPE_ID(1039, std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(1040, std::unordered_map<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(1041, std::unordered_map<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(1042, std::unordered_map<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(1043, std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(1044, std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE_ID(1045, std::unordered_map<QwerkE::GUID, std::string>)

MIRROR_TYPE_ID(1046, QwerkE::Input::GameActions)
MIRROR_TYPE_ID(1047, QwerkE::EntityHandle)

MIRROR_TYPE_ID(1048, QwerkE::ComponentScript)
MIRROR_TYPE_ID(1049, QwerkE::ComponentTransform)
MIRROR_TYPE_ID(1050, QwerkE::ComponentMesh)
MIRROR_TYPE_ID(1051, QwerkE::ComponentLight)
MIRROR_TYPE_ID(1052, QwerkE::ComponentInfo)
MIRROR_TYPE_ID(1053, QwerkE::ComponentCamera)
MIRROR_TYPE_ID(1054, QwerkE::RendererSettings)

MIRROR_TYPE_ID(1055, QwerkE::EngineSettings)

MIRROR_TYPE_ID(1056, QwerkE::ScriptGuiButton)

MIRROR_TYPE_ID(1057, QwerkE::eScriptTypes)
MIRROR_TYPE_ID(1058, const QwerkE::eScriptTypes)
MIRROR_TYPE_ID(1059, QwerkE::Scriptable)
MIRROR_TYPE_ID(1060, QwerkE::Scriptable*)
MIRROR_TYPE_ID(1061, QwerkE::ScriptableTesting)
MIRROR_TYPE_ID(1062, QwerkE::ScriptablePathFinder)
MIRROR_TYPE_ID(1063, QwerkE::ScriptablePatrol)
MIRROR_TYPE_ID(1064, QwerkE::GUID)
MIRROR_TYPE_ID(1065, const QwerkE::GUID)
MIRROR_TYPE_ID(1066, QwerkE::ScriptableCamera)
MIRROR_TYPE_ID(1067, QwerkE::ScriptableSceneTransition)
