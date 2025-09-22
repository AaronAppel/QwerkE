#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Ids.h"
#endif

#include "QF_Mirror.h" // #TOD Review need

#include "QE_EditorWindowHelpers.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

// #TESTING
#include "QE_Editor.h"

MIR_TYPE_ID(9000, MyClass)
MIR_TYPE_ID(9001, std::tuple<int, bool, std::string, vec3f>)

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

// #NOTE Editor Ids start at +2000
MIR_TYPE_ID(2002, QwerkE::Editor::EditorWindowFlags)
MIR_TYPE_ID(2003, QwerkE::Editor::EditorWindowTypes)

MIR_TYPE_ID(2007, QwerkE::ProjectsData)
MIR_TYPE_ID(2008, QwerkE::EditorSettings)
MIR_TYPE_ID(2009, QwerkE::Project)

MIR_TYPE_ID(2010, QwerkE::Editor::EditorWindowAssets)
MIR_TYPE_ID(2011, QwerkE::Editor::EditorWindowConsole)
MIR_TYPE_ID(2012, QwerkE::Editor::EditorWindowDebugDefault)
MIR_TYPE_ID(2013, QwerkE::Editor::EditorWindowDockingContext)
MIR_TYPE_ID(2014, QwerkE::Editor::EditorWindowEntityInspector)
MIR_TYPE_ID(2015, QwerkE::Editor::EditorWindowImGuiDemo)
MIR_TYPE_ID(2016, QwerkE::Editor::EditorWindowMenuBar)
// MIR_TYPE_ID(2017, Unused) // Previously QwerkE::Editor::EditorWindowSceneControls
MIR_TYPE_ID(2018, QwerkE::Editor::EditorWindowSceneGraph)
MIR_TYPE_ID(2019, QwerkE::Editor::EditorWindowSceneView)
MIR_TYPE_ID(2020, QwerkE::Editor::EditorWindowSettings)
MIR_TYPE_ID(2021, QwerkE::Editor::EditorWindowStylePicker)
MIR_TYPE_ID(2022, QwerkE::Editor::EditorWindowMaterialEditor)
MIR_TYPE_ID(2023, QwerkE::Editor::EditorWindowFolderViewer)
MIR_TYPE_ID(2024, QwerkE::Editor::EditorWindowNodeEditor)
MIR_TYPE_ID(2025, QwerkE::Editor::EditorWindowShaderEditor)
MIR_TYPE_ID(2026, QwerkE::Editor::EditorWindowPrefabScene)
MIR_TYPE_ID(2027, QwerkE::Editor::EditorWindowPrompt)
MIR_TYPE_ID(2029, QwerkE::Editor::EditorWindow)
MIR_TYPE_ID(2030, QwerkE::Editor::EditorWindow*)
MIR_TYPE_ID(2031, std::vector<QwerkE::Editor::EditorWindow*>)
MIR_TYPE_ID(2032, QwerkE::Editor::EditorWindowHexEditor)
MIR_TYPE_ID(2033, QwerkE::Editor::EditorWindowImGuiExtendedDemo)
MIR_TYPE_ID(2034, QwerkE::Editor::EditorWindowNotifications)
MIR_TYPE_ID(2035, QwerkE::Editor::EditorWindowTimeline)
MIR_TYPE_ID(2036, QwerkE::Editor::EditorWindowOptions)
MIR_TYPE_ID(2037, QwerkE::Editor::EditorWindowSizingFlags)
MIR_TYPE_ID(2038, QwerkE::Editor::EditorWindowDebugInput)
MIR_TYPE_ID(2039, QwerkE::Editor::EditorWindowDebugRenderer)
MIR_TYPE_ID(2040, QwerkE::Editor::EditorWindowDebugEvents)
MIR_TYPE_ID(2041, QwerkE::Editor::EditorWindowDebugWindows)
MIR_TYPE_ID(2042, QwerkE::Editor::EditorWindowDebugPhysics)
MIR_TYPE_ID(2043, QwerkE::Editor::EditorWindowDebugMemory)
MIR_TYPE_ID(2044, QwerkE::Editor::EditorWindowDebugProfiler)
MIR_TYPE_ID(2045, QwerkE::Editor::EditorWindowInputMapping)
MIR_TYPE_ID(2046, QwerkE::Editor::EditorWindowFileEditor)
MIR_TYPE_ID(2047, QwerkE::Editor::EditorWindowStatusBar)
MIR_TYPE_ID(2048, QwerkE::Editor::EditorWindowWelcomeWindow)
MIR_TYPE_ID(2049, QwerkE::Editor::eSettingsOptions)
MIR_TYPE_ID(2050, QwerkE::Editor::EditorWindowPhysics)
