#pragma once

#include <array>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Ids.h"
#endif

#include "QF_Guid.h"

#include "QF_QKey.h"
#include "QF_GameActions.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_FrameBuffer.h"
#include "QF_Input.h"
#include "QF_Mesh.h"
#include "QF_Physics.h"
#include "QF_Scene.h"
#include "QF_Sound.h"
#include "QF_Shader.h"
#include "QF_Texture.h"

#include "QF_ComponentHelpers.h"

#include "QF_ScriptHelpers.h"

// #TODO Wrap in #ifndef MIRROR_OMIT_FLAGS
enum FieldSerializationFlags : MIR_FIELD_FLAG_TYPE
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
MIR_TYPE_ID(159, QwerkE::Shader)
MIR_TYPE_ID(160, QwerkE::Mesh)
MIR_TYPE_ID(161, QwerkE::Scene)
MIR_TYPE_ID(162, QwerkE::Texture)
MIR_TYPE_ID(163, QwerkE::FrameBuffer)
MIR_TYPE_ID(164, QwerkE::Sound)
/* END */

// #NOTE Framework Ids start at +1000

// Non-serialized (value independent) type IDs
// std::filesystem
MIR_TYPE_ID(1000, std::filesystem::path)

// ImGui
MIR_TYPE_ID(1004, ImVec2)
MIR_TYPE_ID(1005, ImVec4)
MIR_TYPE_ID(1006, ImVec4[ImGuiCol_COUNT])
MIR_TYPE_ID(1007, ImGuiStyle)

MIR_TYPE_ID(1008, entt::registry)
MIR_TYPE_ID(1009, entt::entity)

// QC Types
MIR_TYPE_ID(1010, QwerkE::Time::Timer)

MIR_TYPE_ID(1011, vec3f)
MIR_TYPE_ID(1012, vec2f)

// Function pointers
MIR_TYPE_ID(1013, void(*)(void))

// Enums
MIR_TYPE_ID(1014, QwerkE::QKey)
MIR_TYPE_ID(1015, QwerkE::eComponentTags)

// Collections
MIR_TYPE_ID(1016, float[16])

MIR_TYPE_ID(1017, std::pair<QwerkE::GUID, std::string>)
MIR_TYPE_ID(1018, std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIR_TYPE_ID(1019, std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIR_TYPE_ID(1020, std::pair<QwerkE::GUID, void*>)
MIR_TYPE_ID(1021, std::pair<QwerkE::GUID, std::string*>)
MIR_TYPE_ID(1022, std::pair<const QwerkE::GUID, std::string>)
MIR_TYPE_ID(1023, std::pair<const QwerkE::GUID, std::string*>)
MIR_TYPE_ID(1024, std::pair<QwerkE::GUID, entt::entity>)
MIR_TYPE_ID(1025, std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIR_TYPE_ID(1026, std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIR_TYPE_ID(1027, std::pair<std::string, std::string>);
MIR_TYPE_ID(1028, std::pair<QwerkE::GUID, std::pair<std::string, std::string>>);
MIR_TYPE_ID(1029, std::pair<QwerkE::GUID, std::string[11]>)
MIR_TYPE_ID(1030, std::pair<QwerkE::GUID, std::vector<std::string>>)
MIR_TYPE_ID(1031, std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIR_TYPE_ID(1032, std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)

MIR_TYPE_ID(1033, std::vector<std::string>)
MIR_TYPE_ID(1034, std::vector<std::string*>)
MIR_TYPE_ID(1035, std::vector<std::pair<QwerkE::GUID, std::string>>)
MIR_TYPE_ID(1036, std::vector<entt::entity>)
MIR_TYPE_ID(1037, std::vector<std::pair<QwerkE::GUID, std::pair<std::string, std::string>>>)
MIR_TYPE_ID(1038, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>)

MIR_TYPE_ID(1039, std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIR_TYPE_ID(1040, std::unordered_map<QwerkE::GUID, void*>)
MIR_TYPE_ID(1041, std::unordered_map<QwerkE::GUID, std::string*>)
MIR_TYPE_ID(1042, std::unordered_map<QwerkE::GUID, entt::entity>)
MIR_TYPE_ID(1043, std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIR_TYPE_ID(1044, std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIR_TYPE_ID(1045, std::unordered_map<QwerkE::GUID, std::string>)

MIR_TYPE_ID(1046, QwerkE::Input::GameActions)
MIR_TYPE_ID(1047, QwerkE::EntityHandle)

MIR_TYPE_ID(1048, QwerkE::ComponentScript)
MIR_TYPE_ID(1049, QwerkE::ComponentTransform)
MIR_TYPE_ID(1050, QwerkE::ComponentMesh)
MIR_TYPE_ID(1051, QwerkE::ComponentLight)
MIR_TYPE_ID(1052, QwerkE::ComponentInfo)
MIR_TYPE_ID(1053, QwerkE::ComponentCamera)
MIR_TYPE_ID(1054, QwerkE::RendererSettings)

// Unused MIR_TYPE_ID(1055, QwerkE::EngineSettings)

MIR_TYPE_ID(1056, QwerkE::ScriptGuiButton)

MIR_TYPE_ID(1057, QwerkE::eScriptTypes)
MIR_TYPE_ID(1058, const QwerkE::eScriptTypes)
MIR_TYPE_ID(1059, QwerkE::Scriptable)
MIR_TYPE_ID(1060, QwerkE::Scriptable*)
MIR_TYPE_ID(1061, QwerkE::ScriptableTesting)
MIR_TYPE_ID(1062, QwerkE::ScriptablePathFinder)
MIR_TYPE_ID(1063, QwerkE::ScriptablePatrol)
MIR_TYPE_ID(1064, QwerkE::GUID)
MIR_TYPE_ID(1065, const QwerkE::GUID)
MIR_TYPE_ID(1066, QwerkE::ScriptableCamera)
MIR_TYPE_ID(1067, QwerkE::ScriptableSceneTransition)

MIR_TYPE_ID(1067, QwerkE::Framework::StartUpData)
MIR_TYPE_ID(1068, QwerkE::ComponentPhysics)
MIR_TYPE_ID(1069, QwerkE::ComponentAudio)
MIR_TYPE_ID(1070, QwerkE::Physics::BodyShapes)
