
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTesting.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"

namespace QwerkE {

	template <typename SuperClass, typename... SubClass>
	static void MirrorSubClass(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
	{
		uint16_t enumValue = enumStartOffset;
		([&]()
			{
				const QwerkE::Mirror::TypeInfo* subclassTypeInfo = QwerkE::Mirror::InfoForType<SubClass>();
				localStaticTypeInfo.derivedTypes.push_back(subclassTypeInfo);
				const_cast<QwerkE::Mirror::TypeInfo*>(subclassTypeInfo)->superTypeInfo = &localStaticTypeInfo;
				const_cast<QwerkE::Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc =
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

#ifdef _QEDITOR // #TODO Look to declare types elsewhere
	// Editor types
	typedef Editor::EditorWindowFlags EditorWindowFlags;
	MIRROR_ENUM(EditorWindowFlags)

	typedef Editor::EditorWindowTypes EditorWindowTypes;
	MIRROR_ENUM(EditorWindowTypes)

	typedef Editor::EditorWindowAssets EditorWindowAssets;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowAssets)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowAssets)

	typedef Editor::EditorWindowDefaultDebug EditorWindowDefaultDebug;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowDefaultDebug)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowDefaultDebug)

	typedef Editor::EditorWindowDockingContext EditorWindowDockingContext;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowDockingContext)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowDockingContext)

	typedef Editor::EditorWindowEntityInspector EditorWindowEntityInspector;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowEntityInspector)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowEntityInspector)

	typedef Editor::EditorWindowImGuiDemo EditorWindowImGuiDemo;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowImGuiDemo)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowImGuiDemo)

	typedef Editor::EditorWindowMenuBar EditorWindowMenuBar;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowMenuBar)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowMenuBar)

	typedef Editor::EditorWindowSceneControls EditorWindowSceneControls;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneControls)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowSceneControls)

	typedef Editor::EditorWindowSceneGraph EditorWindowSceneGraph;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneGraph)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowSceneGraph)

	typedef Editor::EditorWindowSceneView EditorWindowSceneView;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSceneView)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_MEMBER(m_TextureId)
	MIRROR_CLASS_MEMBER(m_ViewId)
	MIRROR_CLASS_END(EditorWindowSceneView)

	typedef Editor::EditorWindowSettings EditorWindowSettings;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowSettings)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowSettings)

	typedef Editor::EditorWindowStylePicker EditorWindowStylePicker;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowStylePicker)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_MEMBER(m_ShowMoreInfo)
	MIRROR_CLASS_MEMBER(m_UiScaling)
	MIRROR_CLASS_END(EditorWindowStylePicker)

	typedef Editor::EditorWindowMaterialEditor EditorWindowMaterialEditor;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowMaterialEditor)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowMaterialEditor)

	typedef Editor::EditorWindowFolderViewer EditorWindowFolderViewer;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowFolderViewer)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowFolderViewer)

	typedef Editor::EditorWindowNodeEditor EditorWindowNodeEditor;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowNodeEditor)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowNodeEditor)

	typedef Editor::EditorWindowShaderEditor EditorWindowShaderEditor;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowShaderEditor)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowShaderEditor)

	typedef Editor::EditorWindowPrefabScene EditorWindowPrefabScene;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowPrefabScene)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowPrefabScene)

	typedef Editor::EditorWindowPrompt EditorWindowPrompt;
	MIRROR_DEPENDENT_CLASS_START(EditorWindowPrompt)
	MIRROR_CONSTRUCT_USING_MEMBER(m_Guid)
	MIRROR_CLASS_END(EditorWindowPrompt)

	typedef Editor::EditorWindow EditorWindow;
	MIRROR_ABSTRACT_CLASS_START(EditorWindow)
	MIRROR_CLASS_MEMBER(m_Guid) // Order dependency! 1st // #TODO Review dependency
	MIRROR_CLASS_MEMBER(m_WindowName)
	MIRROR_CLASS_MEMBER(m_ImGuiFlags)
	MIRROR_CLASS_MEMBER(m_WindowFlags)
	MIRROR_CLASS_MEMBER(m_MinimumWidth)
	MIRROR_CLASS_MEMBER(m_MinimumHeight)
	MIRROR_CLASS_MEMBER(m_EditorWindowType)

	// #TODO Solve generic templated type list issue
	// using WindowsList = MirrorTemplateArgumentList <EditorWindowAssets>;
	// MirrorSubClasses<EditorWindow>(WindowsList{}, localStaticTypeInfo, Editor::EditorWindowTypes::Assets);
	MirrorSubClasses<EditorWindow>(Editor::EditorWindowsList{}, localStaticTypeInfo, Editor::EditorWindowTypes::Assets);
	MIRROR_CLASS_END(EditorWindow)

	typedef Editor::EditorWindow* m_editorWindowPtr;
	MIRROR_POINTER(m_editorWindowPtr)

	typedef std::pair<GUID, m_editorWindowPtr> m_pair_guid_editorWindowPtr;
	MIRROR_PAIR(m_pair_guid_editorWindowPtr)

	typedef std::unordered_map<GUID, m_editorWindowPtr> m_umap_guid_editorWindowPtr;
	MIRROR_MAP(m_umap_guid_editorWindowPtr, m_pair_guid_editorWindowPtr)

	MIRROR_CLASS_START(ProjectsData)
	MIRROR_CLASS_MEMBER(LastOpenedProjectFileName)
	MIRROR_CLASS_MEMBER(PreviousProjectFileNames)
	MIRROR_CLASS_END(ProjectsData)
#endif

}
