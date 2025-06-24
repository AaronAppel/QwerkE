#pragma once

#include <unordered_map>

namespace QwerkE {

	class GUID;
	class EntityHandle;

	namespace Editor {

		enum EditorWindowPromptTypes : uint8_t;

		class EditorWindow;
		class EditorWindowEntityInspector;
		class EditorWindowSceneView;

		typedef unsigned int u32; // 0 to 4,294,967,295
		enum EditorStateFlags : u32
		{
			EditorStateFlagsNone			= 0,

			// #TODO Refactor framework state
			EngineSettingsDirty				= 1 << 0,
			UserSettingsDirty				= 1 << 1,
			ProjectStateDirty				= 1 << 2,
			SceneStateDirty					= 1 << 3,
			StyleSettingsDirty				= 1 << 4,

			UiMainMenuShowingImGuiExample	= 1 << 10,

			Placeholder1					= 1 << 20,

			Placeholder2					= 1 << 30,
		};

		const EditorStateFlags& GetEditorStateFlags();
		void SetEditorStateFlags(const EditorStateFlags& flags);
		void ToggleEditorStateFlags(const EditorStateFlags& flags);

		void RequestRestart();

		bool ShowingEditorUI();

		u32 CurrentFrame();

		// Editor windows
		void NewEditorWindow(u32 editorWindowType);
		void OpenEditorWindowPrompt(EditorWindowPromptTypes a_PromptType);
		void CloseEditorWindow(const GUID& guid);

		const std::vector<EditorWindow*>& GetOpenWindows();

		const EditorWindowEntityInspector* LastFocusedInspectorWindow();
		const EditorWindowSceneView* LastFocusedSceneViewWindow();
		const EditorWindow* GetLastFocusedWindow();

		void OnEditorWindowFocused(const EditorWindow* const focusedWindow);

		// Editor window callbacks
		void OnEntitySelected(EntityHandle& entity);
		void OnSceneReloaded();

	}

}