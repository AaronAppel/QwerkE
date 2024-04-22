#include "QE_EditorWindowHelpers.h"

namespace QwerkE {

	namespace Editor {

		EditorWindow* NewEditorWindowByType(EditorWindowTypes editorWindowType)
		{
            switch (editorWindowType)
            {
                // Unique
            case EditorWindowTypes::DefaultDebug:
                return new EditorWindowDefaultDebug(GUID());
            case EditorWindowTypes::DockingContext:
                return new EditorWindowDockingContext(GUID());
            case EditorWindowTypes::ImGuiDemo:
                return new EditorWindowImGuiDemo(GUID());
            case EditorWindowTypes::MenuBar:
                return new EditorWindowMenuBar(GUID());

                // Instanced
            case EditorWindowTypes::Assets:
                return new EditorWindowAssets();
            case EditorWindowTypes::EntityInspector:
                return new EditorWindowEntityInspector();
            case EditorWindowTypes::SceneControls:
                return new EditorWindowSceneControls();
            case EditorWindowTypes::SceneGraph:
                return new EditorWindowSceneGraph();
            case EditorWindowTypes::SceneView:
                {
                    constexpr u8 defaultTextureId = 4;
                    constexpr u8 defaultViewId = 2;
                    return new EditorWindowSceneView(defaultTextureId, defaultViewId);
                }
                break;
            case EditorWindowTypes::Settings:
                return new EditorWindowSettings();
            case EditorWindowTypes::StylePicker:
                return new EditorWindowStylePicker();
            case EditorWindowTypes::MaterialEditor:
                return new EditorWindowMaterialEditor();
            case EditorWindowTypes::FolderViewer:
                return new EditorWindowFolderViewer();
            case EditorWindowTypes::NodeEditor:
                return new EditorWindowNodeEditor();
            case EditorWindowTypes::ShaderEditor:
                return new EditorWindowShaderEditor();
            }

            LOG_ERROR("{0} Unsupported EditorWindow type!", __FUNCTION__);
            return nullptr;
		}

	}

}