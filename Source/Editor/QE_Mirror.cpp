#include "QE_Mirror.h"

#include "Libraries/Mirror/Source/MIR_Mirror.h"

#include "QE_Editor.h"
#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

MIRROR_TYPE(QwerkE::Editor::EditorWindowFlags)
MIRROR_CLASS(QwerkE::Editor::EditorWindowTypes)
MIRROR_CLASS_END

// #TODO Move to Mirror
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
MIRROR_CLASS_MEMBER(startInPlayMode)
MIRROR_CLASS_MEMBER(showWelcomeWindow)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Project)
MIRROR_CLASS_MEMBER(projectFileName)
MIRROR_CLASS_MEMBER(projectName)
MIRROR_CLASS_MEMBER(startUpSceneName)
MIRROR_CLASS_MEMBER(scenesList)
MIRROR_CLASS_END

MIRROR_TYPE(QwerkE::Editor::EditorWindowSizingFlags)

MIRROR_CLASS(QwerkE::Editor::EditorWindowOptions)
MIRROR_CLASS_MEMBER(m_SizingFlags)
MIRROR_CLASS_MEMBER(m_WidthMinimum)
MIRROR_CLASS_MEMBER(m_WidthMaximum)
MIRROR_CLASS_MEMBER(m_HeightMinimum)
MIRROR_CLASS_MEMBER(m_HeightMaximum)
MIRROR_CLASS_MEMBER(m_LockWidth)
MIRROR_CLASS_MEMBER(m_LockHeight)
MIRROR_CLASS_MEMBER(m_RatioWidth)
MIRROR_CLASS_MEMBER(m_RatioHeight)
MIRROR_CLASS_MEMBER(m_IncrementWidth)
MIRROR_CLASS_MEMBER(m_IncrementHeight)
MIRROR_CLASS_MEMBER(m_ItemScaling)
MIRROR_CLASS_MEMBER(m_FontScaling)
// #TODO ImGuiStyle m_Style;
// #TODO ImFont m_Font;
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowAssets)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowConsole)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugDefault)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugDefault)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugEvents)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugEvents)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugInput)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugInput)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugMemory)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugMemory)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugPhysics)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugPhysics)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugProfiler)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugProfiler)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugRenderer)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugRenderer)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowDebugWindows)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugWindows)
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

MIRROR_CLASS(QwerkE::Editor::EditorWindowFileEditor)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowFileEditor)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowFolderViewer)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowFolderViewer)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_MEMBER(m_UiScalar)
MIRROR_CLASS_MEMBER(m_CurrentDirectory)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowHexEditor)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowHexEditor)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowInputMapping)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowInputMapping)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowImGuiDemo)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiDemo)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowImGuiExtendedDemo)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiExtendedDemo)
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
MIRROR_CLASS_MEMBER(m_EditorCamera)
MIRROR_CLASS_MEMBER(m_EditorCameraTransform)
MIRROR_CLASS_MEMBER(m_IsLastFocusedSceneView)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowSettings)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSettings)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowStatusBar)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowStatusBar)
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

MIRROR_CLASS(QwerkE::Editor::EditorWindowNodeEditor)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowNodeEditor)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowNotifications)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowNotifications)
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

MIRROR_CLASS(QwerkE::Editor::EditorWindowTimeline)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowTimeline)
MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
MIRROR_CLASS_END

MIRROR_CLASS(QwerkE::Editor::EditorWindowWelcomeWindow)
// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowWelcomeWindow)
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
MIRROR_CLASS_MEMBER(m_WindowOptions)
// MIRROR_CLASS_MEMBER(m_EditorWindowType)
// #TODO Solve generic templated type list issue
// using WindowsList = Mirror::TypesList <EditorWindowAssets>;
MirrorSubClasses<QwerkE::Editor::EditorWindow>(QwerkE::Editor::EditorWindowsList{}, localStaticTypeInfo, QwerkE::Editor::EditorWindowTypes::EditorWindowTypesInvalid + 1);
MIRROR_CLASS_END
MIRROR_TYPE(QwerkE::Editor::EditorWindow*)

MIRROR_CLASS(QwerkE::ProjectsData)
MIRROR_CLASS_MEMBER(LastOpenedProjectFileName)
MIRROR_CLASS_MEMBER(PreviousProjectFileNames)
MIRROR_CLASS_END

MIRROR_TYPE(std::vector<QwerkE::Editor::EditorWindow*>)
