
#include <array>
#include <unordered_map>
#include <string>
#include <vector>

#ifdef _QBGFXFRAMEWORK
// #include <Libraries/bgfxFramework/include/bgfxFramework/bgfx_utils.h>
// struct bgfxFramework::Mesh;
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_TypeIds.h"
#endif

#include "QC_Guid.h"

#include "QF_eKeys.h"
#include "QF_GameActions.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_Input.h"
#include "QF_Mesh.h"
#include "QF_Scene.h"
#include "QF_Settings.h"

#include "QF_ComponentHelpers.h"

#include "QF_ScriptHelpers.h"

enum FieldSerializationFlags : MIRROR_FIELD_FLAG_SIZE
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

#if defined(MIRROR_NONCONFORMING) && defined(MIRROR_GENERATE_TYPE_IDS)

// #NOTE Serialized type IDs 1st to avoid re-ordering issues
MIRROR_TYPE_ID(QwerkE::Shader)			// #TODO Fix value dependency in Assets registry
MIRROR_TYPE_ID(QwerkE::Mesh)			// #TODO Fix value dependency in Assets registry
MIRROR_TYPE_ID(bgfxFramework::Mesh)		// #TODO Fix value dependency in Assets registry
MIRROR_TYPE_ID(QwerkE::Scene)			// #TODO Fix value dependency in Assets registry

// dear imgui
MIRROR_TYPE_ID(ImVec2)
MIRROR_TYPE_ID(ImVec4)
MIRROR_TYPE_ID(ImVec4[55])
MIRROR_TYPE_ID(ImGuiStyle)

// entt
MIRROR_TYPE_ID(entt::registry)
MIRROR_TYPE_ID(entt::entity)

// QC Types
MIRROR_TYPE_ID(QwerkE::Time::Timer)

MIRROR_TYPE_ID(vec3f)
MIRROR_TYPE_ID(vec2f)

// Function pointers
MIRROR_TYPE_ID(void(*)(void))

// Enums
MIRROR_TYPE_ID(QwerkE::eKeys)
MIRROR_TYPE_ID(QwerkE::eComponentTags)

// Collections
MIRROR_TYPE_ID(float[16])

MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(std::pair<const QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(std::pair<const QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(std::pair<std::string, std::string>);
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::pair<std::string, std::string>>);
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::string[11]>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::vector<std::string>>)
MIRROR_TYPE_ID(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE_ID(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)

MIRROR_TYPE_ID(std::vector<std::string>)
MIRROR_TYPE_ID(std::vector<std::string*>)
MIRROR_TYPE_ID(std::vector<std::pair<QwerkE::GUID, std::string>>)
MIRROR_TYPE_ID(std::vector<entt::entity>)
MIRROR_TYPE_ID(std::vector<std::pair<QwerkE::GUID, std::pair<std::string, std::string>>>)
MIRROR_TYPE_ID(std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>)

MIRROR_TYPE_ID(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, std::string>)

MIRROR_TYPE_ID(QwerkE::Input::GameActions)
MIRROR_TYPE_ID(QwerkE::EntityHandle)

MIRROR_TYPE_ID(QwerkE::ComponentScript)
MIRROR_TYPE_ID(QwerkE::ComponentTransform)
MIRROR_TYPE_ID(QwerkE::ComponentMesh)
MIRROR_TYPE_ID(QwerkE::ComponentLight)
MIRROR_TYPE_ID(QwerkE::ComponentInfo)
MIRROR_TYPE_ID(QwerkE::ComponentCamera)
MIRROR_TYPE_ID(QwerkE::RendererSettings)

MIRROR_TYPE_ID(QwerkE::EngineSettings)

MIRROR_TYPE_ID(QwerkE::ScriptGuiButton)

MIRROR_TYPE_ID(QwerkE::eScriptTypes)
MIRROR_TYPE_ID(const QwerkE::eScriptTypes)
MIRROR_TYPE_ID(QwerkE::Scriptable)
MIRROR_TYPE_ID(QwerkE::Scriptable*)
MIRROR_TYPE_ID(QwerkE::ScriptableTesting)
MIRROR_TYPE_ID(QwerkE::ScriptablePathFinder)
MIRROR_TYPE_ID(QwerkE::ScriptablePatrol)
MIRROR_TYPE_ID(QwerkE::GUID)
MIRROR_TYPE_ID(const QwerkE::GUID)
MIRROR_TYPE_ID(QwerkE::ScriptableCamera)
MIRROR_TYPE_ID(QwerkE::ScriptableSceneTransition)

#else

// #NOTE Framework Ids start at +100

// Value dependent (serialized IDs)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 100, bgfxFramework::Mesh)	// #TODO Fix value dependency in Assets registry
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 101, QwerkE::Shader)			// #TODO Fix value dependency in Assets registry
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 102, QwerkE::Mesh)			// #TODO Fix value dependency in Assets registry
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 103, QwerkE::Scene)			// #TODO Fix value dependency in Assets registry

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 104, ImVec2)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 105, ImVec4)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 106, ImVec4[55])
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 107, ImGuiStyle)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 108, entt::registry)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 109, entt::entity)

// QC Types
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 110, QwerkE::Time::Timer)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 111, vec3f)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 112, vec2f)

// Function pointers
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 113, void(*)(void))

// Enums
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 114, QwerkE::eKeys)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 115, QwerkE::eComponentTags)

// Collections
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 116, float[16])

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 117, std::pair<QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 118, std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 119, std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 120, std::pair<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 121, std::pair<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 122, std::pair<const QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 123, std::pair<const QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 124, std::pair<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 125, std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 126, std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 127, std::pair<std::string, std::string>);
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 128, std::pair<QwerkE::GUID, std::pair<std::string, std::string>>);
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 129, std::pair<QwerkE::GUID, std::string[11]>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 130, std::pair<QwerkE::GUID, std::vector<std::string>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 131, std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 132, std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 133, std::vector<std::string>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 134, std::vector<std::string*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 135, std::vector<std::pair<QwerkE::GUID, std::string>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 136, std::vector<entt::entity>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 137, std::vector<std::pair<QwerkE::GUID, std::pair<std::string, std::string>>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 138, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 139, std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 140, std::unordered_map<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 141, std::unordered_map<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 142, std::unordered_map<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 143, std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 144, std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 145, std::unordered_map<QwerkE::GUID, std::string>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 146, QwerkE::Input::GameActions)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 147, QwerkE::EntityHandle)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 148, QwerkE::ComponentScript)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 149, QwerkE::ComponentTransform)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 150, QwerkE::ComponentMesh)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 151, QwerkE::ComponentLight)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 152, QwerkE::ComponentInfo)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 153, QwerkE::ComponentCamera)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 154, QwerkE::RendererSettings)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 155, QwerkE::EngineSettings)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 156, QwerkE::ScriptGuiButton)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 157, QwerkE::eScriptTypes)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 158, const QwerkE::eScriptTypes)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 159, QwerkE::Scriptable)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 160, QwerkE::Scriptable*)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 161, QwerkE::ScriptableTesting)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 162, QwerkE::ScriptablePathFinder)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 163, QwerkE::ScriptablePatrol)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 164, QwerkE::GUID)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 165, const QwerkE::GUID)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 166, QwerkE::ScriptableCamera)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 167, QwerkE::ScriptableSceneTransition)

#endif
