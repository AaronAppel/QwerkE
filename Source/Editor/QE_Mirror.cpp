#include "QE_Mirror.h"

#include "Libraries/Mirror/Source/MIR_Mirror.h"

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

// #TESTING
#include "QE_Editor.h"

// Enums
MIRROR_TYPE(QwerkE::Editor::EditorWindowFlags)
MIRROR_CLASS(QwerkE::Editor::EditorWindowTypes)
MIRROR_CLASS_END

// Pointers
MIRROR_TYPE(std::pair<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE(std::pair<const QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE(std::unordered_map<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)

template <typename SuperClass, typename... SubClass>
static void MirrorSubClass(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
{
	uint16_t enumValue = enumStartOffset;
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
			++enumValue;
		}(), ...);
}

template<typename SuperClass, typename... SubClass>
static void MirrorSubClasses(TemplateArgumentList<SubClass...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
{
	MirrorSubClass<SuperClass, SubClass...>(localStaticTypeInfo, enumStartOffset);
}

MIRROR_CLASS(QwerkE::UserSettings)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Project)
MIRROR_CLASS_MEMBER(projectFileName)
MIRROR_CLASS_MEMBER(projectName)
MIRROR_CLASS_MEMBER(startUpSceneName)
MIRROR_CLASS_MEMBER(scenesList)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowAssets)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowConsole)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDefaultDebug)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDefaultDebug)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDockingContext)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDockingContext)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowEntityInspector)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowEntityInspector)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowImGuiDemo)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiDemo)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowMenuBar)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowMenuBar)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowSceneControls)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneControls)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowSceneGraph)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneGraph)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowSceneView)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneView)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_MEMBER(m_TextureId)
MIRROR_CLASS_MEMBER(m_ViewId)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowSettings)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSettings)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowStylePicker)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowStylePicker)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowMaterialEditor)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowMaterialEditor)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowFolderViewer)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowFolderViewer)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowNodeEditor)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowNodeEditor)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowShaderEditor)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowShaderEditor)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowPrefabScene)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowPrefabScene)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowPrompt)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowPrompt)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindow)
// #TODO Review MIRROR_ABSTRACT_CLASS_START(EditorWindow)
MIRROR_CLASS_MEMBER(m_Guid) // Order dependency! 1st // #TODO Review dependency
MIRROR_CLASS_MEMBER(m_WindowName)
MIRROR_CLASS_MEMBER(m_ImGuiFlags)
MIRROR_CLASS_MEMBER(m_WindowFlags)
MIRROR_CLASS_MEMBER(m_MinimumWidth)
MIRROR_CLASS_MEMBER(m_MinimumHeight)
// MIRROR_CLASS_MEMBER(m_EditorWindowType)
// #TODO Solve generic templated type list issue
// using WindowsList = MirrorTemplateArgumentList <EditorWindowAssets>;
MirrorSubClasses<QwerkE::Editor::EditorWindow>(QwerkE::Editor::EditorWindowsList{}, localStaticTypeInfo, QwerkE::Editor::EditorWindowTypes::EditorWindowTypesInvalid + 1);
MIRROR_CLASS_END
MIRROR_TYPE(QwerkE::Editor::EditorWindow*)

MIRROR_CLASS(QwerkE::ProjectsData)
MIRROR_CLASS_MEMBER(LastOpenedProjectFileName)
MIRROR_CLASS_MEMBER(PreviousProjectFileNames)
MIRROR_CLASS_END
