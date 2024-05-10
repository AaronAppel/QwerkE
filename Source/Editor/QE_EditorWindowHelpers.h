#pragma once

#include "QE_EditorWindow.h"
#include "QE_EditorWindowAssets.h"
#include "QE_EditorWindowDefaultDebug.h"
#include "QE_EditorWindowDockingContext.h"
#include "QE_EditorWindowEntityInspector.h"
#include "QE_EditorWindowFolderViewer.h"
#include "QE_EditorWindowImGuiDemo.h"
#include "QE_EditorWindowMaterialEditor.h"
#include "QE_EditorWindowMenuBar.h"
#include "QE_EditorWindowNodeEditor.h"
#include "QE_EditorWindowPrefabScene.h"
#include "QE_EditorWindowPrompt.h"
#include "QE_EditorWindowSceneControls.h"
#include "QE_EditorWindowSceneGraph.h"
#include "QE_EditorWindowSceneView.h"
#include "QE_EditorWindowSettings.h"
#include "QE_EditorWindowShaderEditor.h"
#include "QE_EditorWindowStylePicker.h"

namespace QwerkE {

	namespace Editor {

        using EditorWindowsList = TemplateArgumentList <
			// #NOTE Order matters! Match enum EditorWindowTypes order
			EditorWindowAssets,
			EditorWindowDefaultDebug,
			EditorWindowDockingContext,
			EditorWindowEntityInspector,
			EditorWindowFolderViewer,
			EditorWindowImGuiDemo,
			EditorWindowMaterialEditor,
			EditorWindowMenuBar,
			EditorWindowNodeEditor,
			EditorWindowSceneControls,
			EditorWindowSceneGraph,
			EditorWindowSceneView,
			EditorWindowSettings,
			EditorWindowShaderEditor,
			EditorWindowStylePicker,
			EditorWindowPrefabScene,
			EditorWindowPrompt
        >;

		template <typename... T>
		EditorWindow* NewEditorWindowByType(EditorWindowTypes editorWindowType)
		{
			EditorWindow* returnPointer = nullptr;
			([&]()
			{
				if (returnPointer)
					return;

				T stackInstance = T(GUID()); // #TODO Add a static Type() function to each EditorWindow type
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