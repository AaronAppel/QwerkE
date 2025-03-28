
#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_TypeIds.h"
#endif

#include "QF_Mirror.h"

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

// #TESTING
#include "QE_Editor.h"

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

// #TODO See if PCH files can be used to reduce compiler load and heap requirements to generate type ids again
#if defined(MIRROR_NONCONFORMING) && defined(MIRROR_GENERATE_TYPE_IDS)

// Enums
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowFlags)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowTypes)

// Pointers
MIRROR_TYPE_ID(std::pair<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(std::pair<const QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(std::unordered_map<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)

MIRROR_TYPE_ID(QwerkE::ProjectsData)
MIRROR_TYPE_ID(QwerkE::UserSettings)
MIRROR_TYPE_ID(QwerkE::Project)

MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowAssets)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowDefaultDebug)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowDockingContext)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowEntityInspector)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowImGuiDemo)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowMenuBar)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowSceneControls)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowSceneGraph)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowSceneView)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowSettings)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowStylePicker)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowMaterialEditor)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowFolderViewer)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowNodeEditor)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowShaderEditor)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowPrefabScene)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindowPrompt)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindow)
MIRROR_TYPE_ID(QwerkE::Editor::EditorWindow*)

#else

// Enums                                      #NOTE Editor Ids start at +500
// MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 501,)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 502, QwerkE::Editor::EditorWindowFlags)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 503, QwerkE::Editor::EditorWindowTypes)

// Pointers
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 504, std::pair<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 505, std::pair<const QwerkE::GUID, QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 506, std::unordered_map<QwerkE::GUID, QwerkE::Editor::EditorWindow*>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 507, QwerkE::ProjectsData)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 508, QwerkE::UserSettings)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 509, QwerkE::Project)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 510, QwerkE::Editor::EditorWindowAssets)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 511, QwerkE::Editor::EditorWindowDefaultDebug)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 512, QwerkE::Editor::EditorWindowDockingContext)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 513, QwerkE::Editor::EditorWindowEntityInspector)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 514, QwerkE::Editor::EditorWindowImGuiDemo)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 515, QwerkE::Editor::EditorWindowMenuBar)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 516, QwerkE::Editor::EditorWindowSceneControls)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 517, QwerkE::Editor::EditorWindowSceneGraph)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 518, QwerkE::Editor::EditorWindowSceneView)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 519, QwerkE::Editor::EditorWindowSettings)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 520, QwerkE::Editor::EditorWindowStylePicker)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 521, QwerkE::Editor::EditorWindowMaterialEditor)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 522, QwerkE::Editor::EditorWindowFolderViewer)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 523, QwerkE::Editor::EditorWindowNodeEditor)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 524, QwerkE::Editor::EditorWindowShaderEditor)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 525, QwerkE::Editor::EditorWindowPrefabScene)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 526, QwerkE::Editor::EditorWindowPrompt)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 527, QwerkE::Editor::EditorWindow)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 528, QwerkE::Editor::EditorWindow*)

#endif // DEBUG
