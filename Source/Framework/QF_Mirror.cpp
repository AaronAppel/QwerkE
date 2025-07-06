#include "QF_Mirror.h"

#include <stdint.h>
#include <map>

#include "Libraries/Mirror/Source/MIR_Mirror.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

// #TODO Move this logic to Mirror
template <typename SuperClass, typename... SubClass>
static void MirrorSubClass(Mirror::TypeInfo& localStaticTypeInfo)
{
	([&]()
	{
		const Mirror::TypeInfo* subclassTypeInfo = Mirror::InfoForType<SubClass>();
		localStaticTypeInfo.derivedTypes.push_back(subclassTypeInfo);
		const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->superTypeInfo = &localStaticTypeInfo;
		const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc =
			[](const void* pointerToInstance) -> bool {
			SubClass* subClass = (SubClass*)pointerToInstance;
			return dynamic_cast<SubClass*>(*(SuperClass**)pointerToInstance) != nullptr;
		};
	}(), ...);
}

template<typename SuperClass, typename... SubClass>
static void MirrorSubClasses(TemplateArgumentList<SubClass...>, Mirror::TypeInfo& localStaticTypeInfo)
{
	MirrorSubClass<SuperClass, SubClass...>(localStaticTypeInfo);
}

#ifdef _QDEARIMGUI
MIRROR_CLASS(ImVec2)
MIRROR_CLASS_MEMBER(x)
MIRROR_CLASS_MEMBER(y)
MIRROR_CLASS_END

MIRROR_CLASS(ImVec4)
MIRROR_CLASS_MEMBER(x)
MIRROR_CLASS_MEMBER(y)
MIRROR_CLASS_MEMBER(z)
MIRROR_CLASS_MEMBER(w)
MIRROR_CLASS_END

MIRROR_TYPE(ImVec4[ImGuiCol_COUNT])

MIRROR_CLASS(ImGuiStyle)
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
MIRROR_CLASS_END
#endif

#ifdef _QENTT
MIRROR_TYPE(entt::registry) // #TODO Deprecate MIRROR_TYPE
MIRROR_TYPE(entt::entity)
#endif

// std::filesystem
MIRROR_TYPE(std::filesystem::path)

// GUID
MIRROR_CLASS(QwerkE::GUID)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(const QwerkE::GUID)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_MEMBER(m_Guid)
MIRROR_CLASS_END

// Enums
MIRROR_TYPE(QwerkE::eScriptTypes)
MIRROR_TYPE(const QwerkE::eScriptTypes)
MIRROR_TYPE(QwerkE::eComponentTags)
MIRROR_TYPE(QwerkE::QKey)

// Arrays
MIRROR_TYPE(float[16])

// Pairs
MIRROR_TYPE(std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE(std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE(std::pair<QwerkE::GUID, entt::entity>)
MIRROR_TYPE(std::pair<QwerkE::GUID, std::string>)
MIRROR_TYPE(std::pair<const QwerkE::GUID, std::string>)
MIRROR_TYPE(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE(std::pair<QwerkE::GUID, void*>)
MIRROR_TYPE(std::pair<QwerkE::GUID, std::string*>)
MIRROR_TYPE(std::pair<const QwerkE::GUID, std::string*>)
MIRROR_TYPE(std::pair<std::string, std::string>)
MIRROR_TYPE(std::pair<QwerkE::GUID, std::pair<std::string, std::string>>)
MIRROR_TYPE(std::pair<QwerkE::GUID, std::vector<std::string>>)
MIRROR_TYPE(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)

// Vectors
MIRROR_TYPE(std::vector<entt::entity>)
MIRROR_TYPE(std::vector<std::string>)
MIRROR_TYPE(std::vector<std::string*>)
MIRROR_TYPE(std::vector<std::pair<QwerkE::GUID, std::string>>)
MIRROR_TYPE(std::vector<std::pair<QwerkE::GUID, std::pair<std::string, std::string>>>)
MIRROR_TYPE(std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>)

// Maps
MIRROR_TYPE(std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIRROR_TYPE(std::unordered_map<QwerkE::GUID, std::string*>)
MIRROR_TYPE(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIRROR_TYPE(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIRROR_TYPE(std::unordered_map<QwerkE::GUID, std::string>)

// Function pointers
// MIRROR_TYPE(void(*)(void))
// #TODO Remove expanded code
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void(*)(void)>() {
	static_assert(sizeof(void(*)(void)) <= 0xffffui16, "Size is larger than member can hold!"); static Mirror::TypeInfo localStaticTypeInfo; if (!localStaticTypeInfo.stringName.empty()) {
		return &localStaticTypeInfo;
	}
	localStaticTypeInfo.category = GetCategory<void(*)(void)>();
	localStaticTypeInfo.id = Mirror::IdForType<void(*)(void)>();
	localStaticTypeInfo.stringName = typeid(void(*)(void)).name();
	localStaticTypeInfo.size = sizeof(void(*)(void)); switch (localStaticTypeInfo.category) {
	case TypeInfoCategory_Collection:
		SetCollectionLambdas<void(*)(void)>(&localStaticTypeInfo, is_stl_container_impl::is_stl_container<void(*)(void)>::type());
	case TypeInfoCategory_Class: SetConstructionLambda<void(*)(void)>(&localStaticTypeInfo, std::is_class<void(*)(void)>::type()); break;
	// #TODO Don't remove pointer if the type is a function pointer. Maybe use is_function<std::remove_pointer_t<void(*)(void)>>::type
	// case TypeInfoCategory_Pointer: localStaticTypeInfo.pointerDereferencedTypeInfo = Mirror::InfoForType<std::remove_pointer_t<void(*)(void)>>();
	case TypeInfoCategory_Pointer:
		assert(std::is_pointer_v<void(*)(void)>);
		// ERROR: unresolved external symbol... missing type info for Mirror::InfoForType<void __cdecl(void)>(void)
		// localStaticTypeInfo.pointerDereferencedTypeInfo = Mirror::InfoForType<std::remove_pointer_t<void(*)(void)>>();
		break;
	case TypeInfoCategory_Primitive: SetConstructionLambda<void(*)(void)>(&localStaticTypeInfo, std::is_same<void(*)(void), std::string>::type()); break;
	} return &localStaticTypeInfo;
}

// Scripts
MIRROR_CLASS(QwerkE::ScriptableCamera)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ScriptGuiButton)
MIRROR_CLASS_MEMBER_FLAGS(m_CallbackFunction, FieldSerializationFlags::_InspectorOnly)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ScriptablePatrol)
MIRROR_CLASS_MEMBER(m_Stride)
MIRROR_CLASS_MEMBER(m_Speed)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ScriptablePathFinder)
MIRROR_CLASS_MEMBER(m_MovementSpeed)
MIRROR_CLASS_MEMBER(m_DistanceToChangeTargets)
MIRROR_CLASS_MEMBER_FLAGS(m_CurrentTransformTargetIndex, FieldSerializationFlags::_InspectorOnly)
MIRROR_CLASS_MEMBER_FLAGS(m_Button, FieldSerializationFlags::_InspectorOnly)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ScriptableTesting)
MIRROR_CLASS_MEMBER(m_GuiButton)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ScriptableSceneTransition)
MIRROR_CLASS_MEMBER(m_SceneToTransitionToGuid)
MIRROR_CLASS_END

// MIRROR_ABSTRACT_CLASS_START(QwerkE::Scriptable)
MIRROR_CLASS(QwerkE::Scriptable)
// #TODO Look at generating empty types or not yet declared types automatically as well.
// Would save a step when creating a new type and still allow exposing members for specific types
MirrorSubClasses<QwerkE::Scriptable>(QwerkE::ComponentScriptsList{}, localStaticTypeInfo);
MIRROR_CLASS_END
MIRROR_TYPE(QwerkE::Scriptable*)

// Structs
MIRROR_CLASS(vec2f)
MIRROR_CLASS_MEMBER(x)
MIRROR_CLASS_MEMBER(y)
MIRROR_CLASS_END

MIRROR_CLASS(vec3f)
MIRROR_CLASS_MEMBER(x)
MIRROR_CLASS_MEMBER(y)
MIRROR_CLASS_MEMBER(z)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Framework::StartUpData)
MIRROR_CLASS_MEMBER(windowWidth)
MIRROR_CLASS_MEMBER(windowHeight)
MIRROR_CLASS_MEMBER(windowOpenPositionX)
MIRROR_CLASS_MEMBER(windowOpenPositionY)
MIRROR_CLASS_MEMBER(vSyncEnabled)
MIRROR_CLASS_MEMBER(windowAutoFocusOnStart)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::RendererSettings)
MIRROR_CLASS_MEMBER(drawingPrimitiveType)
MIRROR_CLASS_END

// MIRROR_DEPENDENT_CLASS_START(QwerkE::Scene)
MIRROR_CLASS(QwerkE::Scene)
// #TODO MIRROR_CONSTRUCT_USING_MEMBER(m_SceneGuid)
MIRROR_CLASS_MEMBER(m_SceneGuid)
MIRROR_CLASS_MEMBER(m_SceneFileName)
MIRROR_CLASS_MEMBER(m_Registry)
MIRROR_CLASS_END

// Components
MIRROR_CLASS(QwerkE::ComponentCamera)
MIRROR_CLASS_MEMBER_FLAGS(m_ShowSphere, FieldSerializationFlags::_InspectorOnly)
MIRROR_CLASS_MEMBER(m_MoveSpeed)
MIRROR_CLASS_MEMBER_FLAGS(m_LookAtPosition, FieldSerializationFlags::_InspectorViewOnly)
MIRROR_CLASS_MEMBER(m_Fov)
MIRROR_CLASS_MEMBER(m_Near)
MIRROR_CLASS_MEMBER(m_Far)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ComponentInfo)
MIRROR_CLASS_MEMBER_FLAGS(m_EntityName, FieldSerializationFlags::_HideInInspector)
// #NOTE Unserialized as needed earlier during deserialization (parent array name instead)
// Assigned through EntityHandle constructor
MIRROR_CLASS_MEMBER(m_Guid)
MIRROR_CLASS_MEMBER(m_Enabled)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ComponentLight)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ComponentMesh)
MIRROR_CLASS_MEMBER(m_MeshGuid)
MIRROR_CLASS_MEMBER(m_ShaderGuid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ComponentTransform)
MIRROR_CLASS_MEMBER(m_Matrix)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::ComponentScript)
MIRROR_CLASS_MEMBER(m_ScriptInstances)
MIRROR_CLASS_END

// Misc
MIRROR_CLASS(QwerkE::EntityHandle)
MIRROR_CLASS_END

// Assets
MIRROR_CLASS(QwerkE::Mesh)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Shader)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Texture)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::FrameBuffer)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Input::GameActions)
MIRROR_CLASS_MEMBER(Camera_MoveForward)
MIRROR_CLASS_MEMBER(Camera_MoveBackward)
MIRROR_CLASS_MEMBER(Camera_MoveLeft)
MIRROR_CLASS_MEMBER(Camera_MoveRight)
MIRROR_CLASS_MEMBER(Camera_MoveUp)
MIRROR_CLASS_MEMBER(Camera_MoveDown)
MIRROR_CLASS_MEMBER(Camera_RotateLeft)
MIRROR_CLASS_MEMBER(Camera_RotateRight)
MIRROR_CLASS_END