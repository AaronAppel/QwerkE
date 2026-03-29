#include "QE_Mirror.h"

#include "Libraries/Mirror/Source/MIR_Mirror.h"

#include "QE_Editor.h"
#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

MIR_TYPE(std::tuple<int, bool, std::string, vec3f>)

MIR_TYPE(QwerkE::Editor::EditorWindowFlags)

MIR_CLASS(QwerkE::Editor::EditorWindowTypes)
MIR_CLASS_END

// #TODO Move to Mirror
template <typename SuperClass, typename... SubClass>
static void MirrorSubClass(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
{
	uint16_t enumValue = enumStartOffset;
	([&]()
	{
		static_assert(std::is_base_of<SuperClass, SubClass>::value, "SubClass class must derive from SuperClass.");
		const Mirror::TypeInfo* subclassTypeInfo = Mirror::InfoForType<SubClass>();
		localStaticTypeInfo.derivedTypes.push_back(subclassTypeInfo);
		const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->superTypeInfo = &localStaticTypeInfo;
		const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc =
			[](const void* pointerToInstance) -> bool {
			SubClass* subClass = (SubClass*)pointerToInstance;
			return dynamic_cast<SubClass*>(*(SuperClass**)pointerToInstance) != nullptr;
			};
		++enumValue; // #TODO Review deprecated enum
	}(), ...);
}

template<typename SuperClass, typename... SubClass>
static void MirrorSubClasses(TemplateArgumentList<SubClass...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
{
	MirrorSubClass<SuperClass, SubClass...>(localStaticTypeInfo, enumStartOffset);
}

struct EditorSettings // #NOTE Single instance. Cannot be overridden and is used to initialize other settings
{
	std::string lastUserSettingsFileName = "Default.quser";
};

MIR_CLASS(QwerkE::EditorSettings)
	MIR_CLASS_MEMBER(showWelcomeWindow)
	MIR_CLASS_MEMBER(startUpData)
	MIR_CLASS_MEMBER(limitFramerate)
	MIR_CLASS_MEMBER(maxFramesPerSecond)
	MIR_CLASS_MEMBER(maxEnabledScenes)
	MIR_CLASS_MEMBER(maxJobsAdditionalThreadCount)
	MIR_CLASS_MEMBER(startInPlayMode)
MIR_CLASS_END

MIR_CLASS(QwerkE::Project)
	MIR_CLASS_MEMBER(projectFileName)
	MIR_CLASS_MEMBER(projectName)
	MIR_CLASS_MEMBER(startUpSceneName)
	MIR_CLASS_MEMBER(scenesList)
MIR_CLASS_END

MIR_TYPE(QwerkE::Editor::EditorWindowSizingFlags)

MIR_CLASS(QwerkE::Editor::EditorWindowOptions)
	MIR_CLASS_MEMBER(m_SizingFlags)
	MIR_CLASS_MEMBER(m_WidthMinimum)
	MIR_CLASS_MEMBER(m_WidthMaximum)
	MIR_CLASS_MEMBER(m_HeightMinimum)
	MIR_CLASS_MEMBER(m_HeightMaximum)
	MIR_CLASS_MEMBER(m_LockWidth)
	MIR_CLASS_MEMBER(m_LockHeight)
	MIR_CLASS_MEMBER(m_RatioWidth)
	MIR_CLASS_MEMBER(m_RatioHeight)
	MIR_CLASS_MEMBER(m_IncrementWidth)
	MIR_CLASS_MEMBER(m_IncrementHeight)
	MIR_CLASS_MEMBER(m_ItemScaling)
	MIR_CLASS_MEMBER(m_FontScaling)
	// #TODO ImGuiStyle m_Style;
	// #TODO ImFont m_Font;
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowAssets)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_ViewingLoadedAssets)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowConsole)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugDefault)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugDefault)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_TextureId)
	MIR_CLASS_MEMBER(m_Scaler)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugEvents)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugEvents)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugInput)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugInput)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugMemory)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugMemory)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugPhysics)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugPhysics)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugProfiler)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugProfiler)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugRenderer)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugRenderer)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDebugWindows)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDebugWindows)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowDockingContext)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowDockingContext)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowEntityInspector)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowEntityInspector)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_LastSelectedEntityGuid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowFileEditor)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowFileEditor)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowFolderViewer)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowFolderViewer)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_UiScalar)
	MIR_CLASS_MEMBER(m_CurrentDirectory)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowHexEditor)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowHexEditor)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowInputMapping)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowInputMapping)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowImGuiDemo)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiDemo)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowImGuiExtendedDemo)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiExtendedDemo)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowMenuBar)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowMenuBar)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowPhysics)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowPhysics)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowSceneGraph)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneGraph)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowSceneView)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneView)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_EditorCamera)
	MIR_CLASS_MEMBER(m_EditorCameraTransform)
	MIR_CLASS_MEMBER(m_EditorCameraPitch)
	MIR_CLASS_MEMBER(m_EditorCameraYaw)
	MIR_CLASS_MEMBER(m_IsLastFocusedSceneView)
	MIR_CLASS_MEMBER(m_CurrentSceneGuid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::eSettingsOptions)
	MIR_CLASS_MEMBER(_value)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowSettings)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowSettings)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_SettingsEditorOption)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowStatusBar)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowStatusBar)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowStylePicker)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowStylePicker)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowMaterialEditor)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowMaterialEditor)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowNodeEditor)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowNodeEditor)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowNotifications)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowNotifications)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowShaderEditor)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowShaderEditor)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIR_CLASS_MEMBER(m_CurrentShaderFilePath)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowPrefabScene)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowPrefabScene)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowPrompt)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowPrompt)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowTimeline)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowTimeline)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindowWelcomeWindow)
	// #TODO Review MIRROR_DEPENDENT_CLASS_START(EditorWindowWelcomeWindow)
	MIR_CONSTRUCT_USING_MEMBER(m_Guid)
MIR_CLASS_END

MIR_CLASS(QwerkE::Editor::EditorWindow)
	// #TODO Review MIRROR_ABSTRACT_CLASS_START(EditorWindow)
	MIR_CLASS_MEMBER(m_Guid) // Order dependency! 1st // #TODO Review dependency
	MIR_CLASS_MEMBER(m_WindowName)
	MIR_CLASS_MEMBER(m_ImGuiFlags)
	MIR_CLASS_MEMBER(m_WindowFlags)
	MIR_CLASS_MEMBER(m_MinimumWidth)
	MIR_CLASS_MEMBER(m_MinimumHeight)
	MIR_CLASS_MEMBER(m_WindowOptions)
	// MIR_CLASS_MEMBER(m_EditorWindowType)
	// #TODO Solve generic templated type list issue
	// using WindowsList = Mirror::TypesList <EditorWindowAssets>;
	MirrorSubClasses<QwerkE::Editor::EditorWindow>(QwerkE::Editor::EditorWindowsList{}, localStaticTypeInfo, QwerkE::Editor::EditorWindowTypes::EditorWindowTypesInvalid + 1);
MIR_CLASS_END
MIR_TYPE(QwerkE::Editor::EditorWindow*)

MIR_CLASS(QwerkE::ProjectsData)
	MIR_CLASS_MEMBER(LastOpenedProjectFileName)
	MIR_CLASS_MEMBER(PreviousProjectFileNames)
MIR_CLASS_END

MIR_TYPE(std::vector<QwerkE::Editor::EditorWindow*>)
