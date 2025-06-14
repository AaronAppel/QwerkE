#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_TypeIds.h"
#endif

#include "QF_Mirror.h" // #TOD Review need

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

// #TESTING
#include "QE_Editor.h"

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

// #NOTE Editor Ids start at +2000
MIRROR_TYPE_ID(2002, QwerkE::Editor::EditorWindowFlags)
MIRROR_TYPE_ID(2003, QwerkE::Editor::EditorWindowTypes)

MIRROR_TYPE_ID(2007, QwerkE::ProjectsData)
MIRROR_TYPE_ID(2008, QwerkE::UserSettings)
MIRROR_TYPE_ID(2009, QwerkE::Project)

MIRROR_TYPE_ID(2010, QwerkE::Editor::EditorWindowAssets)
MIRROR_TYPE_ID(2011, QwerkE::Editor::EditorWindowConsole)
MIRROR_TYPE_ID(2012, QwerkE::Editor::EditorWindowDebugDefault)
MIRROR_TYPE_ID(2013, QwerkE::Editor::EditorWindowDockingContext)
MIRROR_TYPE_ID(2014, QwerkE::Editor::EditorWindowEntityInspector)
MIRROR_TYPE_ID(2015, QwerkE::Editor::EditorWindowImGuiDemo)
MIRROR_TYPE_ID(2016, QwerkE::Editor::EditorWindowMenuBar)
MIRROR_TYPE_ID(2017, QwerkE::Editor::EditorWindowSceneControls)
MIRROR_TYPE_ID(2018, QwerkE::Editor::EditorWindowSceneGraph)
MIRROR_TYPE_ID(2019, QwerkE::Editor::EditorWindowSceneView)
MIRROR_TYPE_ID(2020, QwerkE::Editor::EditorWindowSettings)
MIRROR_TYPE_ID(2021, QwerkE::Editor::EditorWindowStylePicker)
MIRROR_TYPE_ID(2022, QwerkE::Editor::EditorWindowMaterialEditor)
MIRROR_TYPE_ID(2023, QwerkE::Editor::EditorWindowFolderViewer)
MIRROR_TYPE_ID(2024, QwerkE::Editor::EditorWindowNodeEditor)
MIRROR_TYPE_ID(2025, QwerkE::Editor::EditorWindowShaderEditor)
MIRROR_TYPE_ID(2026, QwerkE::Editor::EditorWindowPrefabScene)
MIRROR_TYPE_ID(2027, QwerkE::Editor::EditorWindowPrompt)
MIRROR_TYPE_ID(2029, QwerkE::Editor::EditorWindow)
MIRROR_TYPE_ID(2030, QwerkE::Editor::EditorWindow*)
MIRROR_TYPE_ID(2031, std::vector<QwerkE::Editor::EditorWindow*>)
MIRROR_TYPE_ID(2032, QwerkE::Editor::EditorWindowHexEditor)
MIRROR_TYPE_ID(2033, QwerkE::Editor::EditorWindowImGuiExtendedDemo)
MIRROR_TYPE_ID(2034, QwerkE::Editor::EditorWindowNotifications)
MIRROR_TYPE_ID(2035, QwerkE::Editor::EditorWindowTimeline)
MIRROR_TYPE_ID(2036, QwerkE::Editor::EditorWindowOptions)
MIRROR_TYPE_ID(2037, QwerkE::Editor::EditorWindowSizingFlags)
MIRROR_TYPE_ID(2038, QwerkE::Editor::EditorWindowDebugInput)
MIRROR_TYPE_ID(2039, QwerkE::Editor::EditorWindowDebugRenderer)
MIRROR_TYPE_ID(2040, QwerkE::Editor::EditorWindowDebugEvents)
MIRROR_TYPE_ID(2041, QwerkE::Editor::EditorWindowDebugWindows)
MIRROR_TYPE_ID(2042, QwerkE::Editor::EditorWindowDebugPhysics)
MIRROR_TYPE_ID(2043, QwerkE::Editor::EditorWindowDebugMemory)
MIRROR_TYPE_ID(2044, QwerkE::Editor::EditorWindowDebugProfiler)
MIRROR_TYPE_ID(2045, QwerkE::Editor::EditorWindowInputMapping)
MIRROR_TYPE_ID(2046, QwerkE::Editor::EditorWindowFileEditor)
MIRROR_TYPE_ID(2047, QwerkE::Editor::EditorWindowStatusBar)
MIRROR_TYPE_ID(2048, QwerkE::Editor::EditorWindowWelcomeWindow)