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
MIR_CLASS(ImVec2)
	MIR_CLASS_MEMBER(x)
	MIR_CLASS_MEMBER(y)
MIR_CLASS_END

MIR_CLASS(ImVec4)
	MIR_CLASS_MEMBER(x)
	MIR_CLASS_MEMBER(y)
	MIR_CLASS_MEMBER(z)
	MIR_CLASS_MEMBER(w)
MIR_CLASS_END

MIR_TYPE(ImVec4[ImGuiCol_COUNT])

MIR_CLASS(ImGuiStyle)
	MIR_CLASS_MEMBER(Alpha)
	MIR_CLASS_MEMBER(DisabledAlpha)
	MIR_CLASS_MEMBER(WindowPadding)
	MIR_CLASS_MEMBER(WindowRounding)
	MIR_CLASS_MEMBER(WindowBorderSize)
	MIR_CLASS_MEMBER(WindowMinSize)
	MIR_CLASS_MEMBER(WindowTitleAlign)
	MIR_CLASS_MEMBER(WindowMenuButtonPosition)
	MIR_CLASS_MEMBER(ChildRounding)
	MIR_CLASS_MEMBER(ChildBorderSize)
	MIR_CLASS_MEMBER(PopupRounding)
	MIR_CLASS_MEMBER(PopupBorderSize)
	MIR_CLASS_MEMBER(FramePadding)
	MIR_CLASS_MEMBER(FrameRounding)
	MIR_CLASS_MEMBER(FrameBorderSize)
	MIR_CLASS_MEMBER(ItemSpacing)
	MIR_CLASS_MEMBER(ItemInnerSpacing)
	MIR_CLASS_MEMBER(CellPadding)
	MIR_CLASS_MEMBER(TouchExtraPadding)
	MIR_CLASS_MEMBER(IndentSpacing)
	MIR_CLASS_MEMBER(ColumnsMinSpacing)
	MIR_CLASS_MEMBER(ScrollbarSize)
	MIR_CLASS_MEMBER(ScrollbarRounding)
	MIR_CLASS_MEMBER(GrabMinSize)
	MIR_CLASS_MEMBER(GrabRounding)
	MIR_CLASS_MEMBER(LogSliderDeadzone)
	MIR_CLASS_MEMBER(TabRounding)
	MIR_CLASS_MEMBER(TabBorderSize)
	MIR_CLASS_MEMBER(TabMinWidthForCloseButton)
	MIR_CLASS_MEMBER(TabBarBorderSize)
	MIR_CLASS_MEMBER(TableAngledHeadersAngle)
	MIR_CLASS_MEMBER(ColorButtonPosition)
	MIR_CLASS_MEMBER(ButtonTextAlign)
	MIR_CLASS_MEMBER(SelectableTextAlign)
	MIR_CLASS_MEMBER(SeparatorTextBorderSize)
	MIR_CLASS_MEMBER(SeparatorTextAlign)
	MIR_CLASS_MEMBER(SeparatorTextPadding)
	MIR_CLASS_MEMBER(DisplayWindowPadding)
	MIR_CLASS_MEMBER(DisplaySafeAreaPadding)
	MIR_CLASS_MEMBER(DockingSeparatorSize)
	MIR_CLASS_MEMBER(MouseCursorScale)
	MIR_CLASS_MEMBER(AntiAliasedLines)
	MIR_CLASS_MEMBER(AntiAliasedLinesUseTex)
	MIR_CLASS_MEMBER(AntiAliasedFill)
	MIR_CLASS_MEMBER(CurveTessellationTol)
	MIR_CLASS_MEMBER(CircleTessellationMaxError)
MIR_CLASS_MEMBER(Colors)
MIR_CLASS_END
#endif

#ifdef _QENTT
MIR_TYPE(entt::registry) // #TODO Deprecate MIR_TYPE
MIR_TYPE(entt::entity)
#endif

// std::filesystem
MIR_TYPE(std::filesystem::path)

// GUID
MIR_CLASS(QwerkE::GUID)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(const QwerkE::GUID)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_Guid)
MIR_CLASS_END

// Enums
MIR_TYPE(QwerkE::eScriptTypes)
MIR_TYPE(const QwerkE::eScriptTypes)
MIR_TYPE(QwerkE::eComponentTags)
MIR_TYPE(QwerkE::QKey)
MIR_TYPE(QwerkE::Physics::BodyShapes)

// Arrays
MIR_TYPE(float[16])

// Pairs
MIR_TYPE(std::pair<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIR_TYPE(std::pair<const QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIR_TYPE(std::pair<QwerkE::GUID, entt::entity>)
MIR_TYPE(std::pair<QwerkE::GUID, std::string>)
MIR_TYPE(std::pair<const QwerkE::GUID, std::string>)
MIR_TYPE(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIR_TYPE(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIR_TYPE(std::pair<QwerkE::GUID, void*>)
MIR_TYPE(std::pair<QwerkE::GUID, std::string*>)
MIR_TYPE(std::pair<const QwerkE::GUID, std::string*>)
MIR_TYPE(std::pair<std::string, std::string>)
MIR_TYPE(std::pair<QwerkE::GUID, std::pair<std::string, std::string>>)
MIR_TYPE(std::pair<QwerkE::GUID, std::vector<std::string>>)
MIR_TYPE(std::pair<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIR_TYPE(std::pair<const size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)

// Vectors
MIR_TYPE(std::vector<entt::entity>)
MIR_TYPE(std::vector<std::string>)
MIR_TYPE(std::vector<std::string*>)
MIR_TYPE(std::vector<std::pair<QwerkE::GUID, std::string>>)
MIR_TYPE(std::vector<std::pair<QwerkE::GUID, std::pair<std::string, std::string>>>)
MIR_TYPE(std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>)

// Maps
MIR_TYPE(std::unordered_map<QwerkE::eScriptTypes, QwerkE::Scriptable*>)
MIR_TYPE(std::unordered_map<QwerkE::GUID, std::string*>)
MIR_TYPE(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::string>>>)
MIR_TYPE(std::unordered_map<size_t, std::vector<std::pair<QwerkE::GUID, std::vector<std::string>>>>)
MIR_TYPE(std::unordered_map<QwerkE::GUID, std::string>)

// Function pointers
// MIR_TYPE(void(*)(void))
// #TODO Remove expanded code
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void(*)(void)>() {
	static_assert(sizeof(void(*)(void)) <= 0xffffui16, "Size is larger than member can hold!");
	static Mirror::TypeInfo localStaticTypeInfo;
	if (!localStaticTypeInfo.stringName.empty()) {
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
	}
	return &localStaticTypeInfo;
}

// Scripts
MIR_CLASS(QwerkE::ScriptableCamera)
MIR_CLASS_END

MIR_CLASS(QwerkE::ScriptGuiButton)
	MIR_CLASS_MEMBER_FLAGS(m_CallbackFunction, FieldSerializationFlags::_InspectorOnly)
MIR_CLASS_END

MIR_CLASS(QwerkE::ScriptablePatrol)
	MIR_CLASS_MEMBER(m_Stride)
	MIR_CLASS_MEMBER(m_Speed)
MIR_CLASS_END

MIR_CLASS(QwerkE::ScriptablePathFinder)
	MIR_CLASS_MEMBER(m_MovementSpeed)
	MIR_CLASS_MEMBER(m_DistanceToChangeTargets)
	MIR_CLASS_MEMBER_FLAGS(m_CurrentTransformTargetIndex, FieldSerializationFlags::_InspectorOnly)
	MIR_CLASS_MEMBER_FLAGS(m_Button, FieldSerializationFlags::_InspectorOnly)
MIR_CLASS_END

MIR_CLASS(QwerkE::ScriptableTesting)
	MIR_CLASS_MEMBER(m_GuiButton)
MIR_CLASS_END

MIR_CLASS(QwerkE::ScriptableSceneTransition)
	MIR_CLASS_MEMBER(m_SceneToTransitionToGuid)
MIR_CLASS_END

// MIRROR_ABSTRACT_CLASS_START(QwerkE::Scriptable)
MIR_CLASS(QwerkE::Scriptable)
	// #TODO Look at generating empty types or not yet declared types automatically as well.
	// Would save a step when creating a new type and still allow exposing members for specific types
	MirrorSubClasses<QwerkE::Scriptable>(QwerkE::ComponentScriptsList{}, localStaticTypeInfo);
MIR_CLASS_END

MIR_TYPE(QwerkE::Scriptable*)

// Structs
MIR_CLASS(vec2f)
	MIR_CLASS_MEMBER(x)
	MIR_CLASS_MEMBER(y)
MIR_CLASS_END

MIR_CLASS(vec3f)
	MIR_CLASS_MEMBER(x)
	MIR_CLASS_MEMBER(y)
	MIR_CLASS_MEMBER(z)
MIR_CLASS_END

MIR_CLASS(QwerkE::Framework::StartUpData)
	MIR_CLASS_MEMBER(windowWidth)
	MIR_CLASS_MEMBER(windowHeight)
	MIR_CLASS_MEMBER(windowOpenPositionX)
	MIR_CLASS_MEMBER(windowOpenPositionY)
	MIR_CLASS_MEMBER(vSyncEnabled)
	MIR_CLASS_MEMBER(windowAutoFocusOnStart)
MIR_CLASS_END

MIR_CLASS(QwerkE::RendererSettings)
	MIR_CLASS_MEMBER(drawingPrimitiveType)
MIR_CLASS_END

// MIRROR_DEPENDENT_CLASS_START(QwerkE::Scene)
MIR_CLASS(QwerkE::Scene)
	// #TODO MIR_CONSTRUCT_USING_MEMBER(m_SceneGuid)
	MIR_CLASS_MEMBER(m_SceneGuid)
	MIR_CLASS_MEMBER(m_SceneFileName)
	MIR_CLASS_MEMBER(m_Registry)
MIR_CLASS_END

// Components
MIR_CLASS(QwerkE::ComponentAudio)
	MIR_CLASS_MEMBER(m_SoundGuid)
MIR_CLASS_END

MIR_CLASS(QwerkE::ComponentCamera)
	MIR_CLASS_MEMBER_FLAGS(m_ShowSphere, FieldSerializationFlags::_InspectorOnly)
	MIR_CLASS_MEMBER(m_MoveSpeed)
	MIR_CLASS_MEMBER_FLAGS(m_LookAtPosition, FieldSerializationFlags::_InspectorViewOnly)
	MIR_CLASS_MEMBER(m_Fov)
	MIR_CLASS_MEMBER(m_Near)
	MIR_CLASS_MEMBER(m_Far)
MIR_CLASS_END

MIR_CLASS(QwerkE::ComponentInfo)
	MIR_CLASS_MEMBER_FLAGS(m_EntityName, FieldSerializationFlags::_HideInInspector)
	// #NOTE Unserialized as needed earlier during deserialization (parent array name instead)
	// Assigned through EntityHandle constructor
	MIR_CLASS_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_Enabled)
MIR_CLASS_END

MIR_CLASS(QwerkE::ComponentLight)
MIR_CLASS_END

MIR_CLASS(QwerkE::ComponentMesh)
	MIR_CLASS_MEMBER(m_MeshGuid)
	MIR_CLASS_MEMBER(m_ShaderGuid)
	MIR_CLASS_MEMBER(m_TextureGuid)
MIR_CLASS_END

MIR_CLASS(QwerkE::ComponentPhysics)
	MIR_CLASS_MEMBER(m_Shape)
MIR_CLASS_END

MIR_CLASS(QwerkE::ComponentTransform)
	MIR_CLASS_MEMBER(m_Matrix)
MIR_CLASS_END

MIR_CLASS(QwerkE::EntityHandle)
MIR_CLASS_END
MIR_CLASS(QwerkE::ComponentScript)
    MIR_CLASS_MEMBER(m_EntityHandle)
	MIR_CLASS_MEMBER(m_ScriptInstances)
MIR_CLASS_END

// Misc

// Assets
MIR_CLASS(QwerkE::Mesh)
MIR_CLASS_END

MIR_CLASS(QwerkE::Shader)
MIR_CLASS_END

MIR_CLASS(QwerkE::Sound)
MIR_CLASS_END

MIR_CLASS(QwerkE::Texture)
MIR_CLASS_END

MIR_CLASS(QwerkE::FrameBuffer)
MIR_CLASS_END

MIR_CLASS(QwerkE::Input::GameActions)
	MIR_CLASS_MEMBER(Camera_MoveForward)
	MIR_CLASS_MEMBER(Camera_MoveBackward)
	MIR_CLASS_MEMBER(Camera_MoveLeft)
	MIR_CLASS_MEMBER(Camera_MoveRight)
	MIR_CLASS_MEMBER(Camera_MoveUp)
	MIR_CLASS_MEMBER(Camera_MoveDown)
	MIR_CLASS_MEMBER(Camera_RotateLeft)
	MIR_CLASS_MEMBER(Camera_RotateRight)
MIR_CLASS_END