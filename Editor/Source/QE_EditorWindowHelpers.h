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
#include "QE_EditorWindowSceneControls.h"
#include "QE_EditorWindowSceneGraph.h"
#include "QE_EditorWindowSceneView.h"
#include "QE_EditorWindowSettings.h"
#include "QE_EditorWindowShaderEditor.h"
#include "QE_EditorWindowStylePicker.h"

namespace QwerkE {

	namespace Editor {

		EditorWindow* NewEditorWindowByType(EditorWindowTypes editorWindowType);

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
			EditorWindowStylePicker
        >;

	}

}