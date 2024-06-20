
#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/TypeIds.h"
#endif

#include "QC_Guid.h"

#include "QF_eKeys.h"
#include "QF_GameActions.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_Input.h"
#include "QF_Mesh.h"
#include "QF_Scene.h"

#include "QF_ComponentHelpers.h"

#include "QF_ScriptHelpers.h"

enum FieldSerializationFlags : MIRROR_FIELD_FLAG_SIZE
{
	_None = 0,
	_HideInInspector = 1 << 0, // Serialize but don't show in editor UI
	_InspectorOnly = 1 << 1, // Do not serialize the value
	_InspectorViewOnly = 1 << 1, // Do not serialize the value or allow changes
};

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

MIRROR_TYPE_ID(ImVec2)
MIRROR_TYPE_ID(ImVec4)
MIRROR_TYPE_ID(ImVec4[55])
MIRROR_TYPE_ID(ImGuiStyle)

MIRROR_TYPE_ID(entt::registry)
MIRROR_TYPE_ID(entt::entity)

MIRROR_TYPE_ID(bgfxFramework::Mesh)

// QC Types
MIRROR_TYPE_ID_CLASS(QwerkE::Time::Timer)

MIRROR_TYPE_ID_CLASS(vec3f)
MIRROR_TYPE_ID_CLASS(vec2f)

// Function pointers
MIRROR_TYPE_ID_CLASS(void(*)(void))

// Enums
MIRROR_TYPE_ID_CLASS(QwerkE::eKeys)
MIRROR_TYPE_ID_CLASS(QwerkE::eComponentTags)

// Collections
MIRROR_TYPE_ID(float[16])

MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::string>)
MIRROR_TYPE_ID(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(std::pair<const QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE_ID(std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)

MIRROR_TYPE_ID(std::vector<std::string>)
MIRROR_TYPE_ID(std::vector<std::string*>)
MIRROR_TYPE_ID(std::vector<std::pair<QwerkE::GUID, std::string>>)
MIRROR_TYPE_ID(std::vector<entt::entity>)

MIRROR_TYPE_ID(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, void*>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, std::string*>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, entt::entity>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)

MIRROR_TYPE_ID_CLASS(QwerkE::Input::GameActions)
MIRROR_TYPE_ID_CLASS(QwerkE::Shader)
MIRROR_TYPE_ID_CLASS(QwerkE::Mesh)
MIRROR_TYPE_ID_CLASS(QwerkE::EntityHandle)

MIRROR_TYPE_ID_CLASS(QwerkE::ComponentScript)
MIRROR_TYPE_ID_CLASS(QwerkE::ComponentTransform)
MIRROR_TYPE_ID_CLASS(QwerkE::ComponentMesh)
MIRROR_TYPE_ID_CLASS(QwerkE::ComponentLight)
MIRROR_TYPE_ID_CLASS(QwerkE::ComponentInfo)
MIRROR_TYPE_ID_CLASS(QwerkE::ComponentCamera)
MIRROR_TYPE_ID_CLASS(QwerkE::Scene)
MIRROR_TYPE_ID_CLASS(QwerkE::RendererSettings)

MIRROR_TYPE_ID_CLASS(QwerkE::EngineSettings)

MIRROR_TYPE_ID_CLASS(QwerkE::ScriptGuiButton)

MIRROR_TYPE_ID(QwerkE::eScriptTypes)
MIRROR_TYPE_ID(const QwerkE::eScriptTypes)
MIRROR_TYPE_ID_CLASS(QwerkE::Scriptable)
MIRROR_TYPE_ID(QwerkE::Scriptable*)
MIRROR_TYPE_ID_CLASS(QwerkE::ScriptableTesting)
MIRROR_TYPE_ID_CLASS(QwerkE::ScriptablePathFinder)
MIRROR_TYPE_ID_CLASS(QwerkE::ScriptablePatrol)
MIRROR_TYPE_ID_CLASS(QwerkE::GUID)
MIRROR_TYPE_ID_CLASS(const QwerkE::GUID)
MIRROR_TYPE_ID_CLASS(QwerkE::ScriptableCamera)
MIRROR_TYPE_ID_CLASS(QwerkE::ScriptableSceneTransition)
