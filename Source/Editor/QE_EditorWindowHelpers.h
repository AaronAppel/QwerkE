#pragma once

#include "QE_EditorWindow.h"
#include "QE_EditorWindowAssets.h"
#include "QE_EditorWindowConsole.h"
#include "QE_EditorWindowDebugDefault.h"
#include "QE_EditorWindowDebugEvents.h"
#include "QE_EditorWindowDebugInput.h"
#include "QE_EditorWindowDebugMemory.h"
#include "QE_EditorWindowDebugPhysics.h"
#include "QE_EditorWindowDebugProfiler.h"
#include "QE_EditorWindowDebugRenderer.h"
#include "QE_EditorWindowDebugWindows.h"
#include "QE_EditorWindowDockingContext.h"
#include "QE_EditorWindowEntityInspector.h"
#include "QE_EditorWindowFileEditor.h"
#include "QE_EditorWindowFolderViewer.h"
#include "QE_EditorWindowHexEditor.h"
#include "QE_EditorWindowImGuiDemo.h"
#include "QE_EditorWindowImGuiExtendedDemo.h"
#include "QE_EditorWindowInputMapping.h"
#include "QE_EditorWindowMaterialEditor.h"
#include "QE_EditorWindowMenuBar.h"
#include "QE_EditorWindowNodeEditor.h"
#include "QE_EditorWindowNotifications.h"
#include "QE_EditorWindowPhysics.h"
#include "QE_EditorWindowPrefabScene.h"
#include "QE_EditorWindowPrompt.h"
#include "QE_EditorWindowSceneGraph.h"
#include "QE_EditorWindowSceneView.h"
#include "QE_EditorWindowSettings.h"
#include "QE_EditorWindowShaderEditor.h"
#include "QE_EditorWindowStatusBar.h"
#include "QE_EditorWindowStylePicker.h"
#include "QE_EditorWindowTimeline.h"
#include "QE_EditorWindowWelcomeWindow.h"

namespace QwerkE {

	namespace Editor {

        using EditorWindowsList = TemplateArgumentList <
			// #NOTE Order matters! Match enum EditorWindowTypes order
			EditorWindowAssets,
			EditorWindowDebugDefault,
			EditorWindowDockingContext,
			EditorWindowEntityInspector,
			EditorWindowFolderViewer,
			EditorWindowImGuiDemo,
			EditorWindowMaterialEditor,
			EditorWindowMenuBar,
			EditorWindowNodeEditor,
			EditorWindowSceneGraph,
			EditorWindowSceneView,
			EditorWindowSettings,
			EditorWindowShaderEditor,
			EditorWindowStylePicker,
			EditorWindowPrefabScene,
			EditorWindowPrompt,
			EditorWindowConsole,
			EditorWindowHexEditor,
			EditorWindowImGuiExtendedDemo,
			EditorWindowNotifications,
			EditorWindowTimeline,
			EditorWindowDebugInput,
			EditorWindowDebugRenderer,
			EditorWindowDebugEvents,
			EditorWindowDebugWindows,
			EditorWindowDebugPhysics,
			EditorWindowDebugMemory,
			EditorWindowDebugProfiler,
			EditorWindowInputMapping,
			EditorWindowFileEditor,
			EditorWindowStatusBar,
			EditorWindowWelcomeWindow,
			EditorWindowPhysics
        >;

		template <typename... T>
		EditorWindow* NewEditorWindowByType(EditorWindowTypes editorWindowType)
		{
			EditorWindow* returnPointer = nullptr;
			([&]()
			{
				if (returnPointer)
					return;

				// #TODO Avoid required stack editor window instance
				// A static Type() function couldn't be virtual, but a virtual method requires an instance anyways
				// Really just avoiding writing a switch statement for every window type, but it would be way more performant

				T stackInstance = T(GUID());
				if (stackInstance.Type() == editorWindowType)
				{
					returnPointer = new T(GUID());
				}
			}(), ...);
			return returnPointer;
		}

		template <typename... T>
		EditorWindow* NewEditorWindowByType(TemplateArgumentList<T...>, EditorWindowTypes editorWindowType)
		{
			if (EditorWindow* window = NewEditorWindowByType<T...>(editorWindowType))
				return window;
			return nullptr;
		}

	}

}