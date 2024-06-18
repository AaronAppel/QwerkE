
#ifdef _QMIRROR
#include "Libraries/Mirror/Source/TypeIds.h"
#endif

#include "QF_Mirror.h"

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

#include "QE_Projects.h"

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

// Enums
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowFlags)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowTypes)

// Pointers
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(std::pair<const QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)

MIRROR_TYPE_ID_CLASS(QwerkE::ProjectsData)
MIRROR_TYPE_ID_CLASS(QwerkE::UserSettings)
MIRROR_TYPE_ID_CLASS(QwerkE::Project)

MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowAssets)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowDefaultDebug)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowDockingContext)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowEntityInspector)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowImGuiDemo)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowMenuBar)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowSceneControls)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowSceneGraph)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowSceneView)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowSettings)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowStylePicker)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowMaterialEditor)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowFolderViewer)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowNodeEditor)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowShaderEditor)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowPrefabScene)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindowPrompt)
MIRROR_TYPE_ID_CLASS(QwerkE::Editor::EditorWindow)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindow*)
