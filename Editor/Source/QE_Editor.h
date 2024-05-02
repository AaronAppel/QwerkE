#pragma once

namespace QwerkE {

	class GUID;

	class EntityHandle;

	namespace Editor {

		typedef unsigned int u32;
		enum EditorStateFlags : u32
		{
			EditorStateFlagsNone			= 0,

			// #TODO Refactor framework state
			EngineSettingsDirty				= 1 << 0,
			UserSettingsDirty				= 1 << 1,
			ProjectStateDirty				= 1 << 2,
			SceneStateDirty					= 1 << 3,
			StyleSettingsDirty				= 1 << 4,

			UiMainMenuShowingImGuiExample	= 1 << 100,

			Placeholder1					= 1 << 200,

			Placeholder2					= 1 << 300,
		};

		void Run(unsigned int argc, char** argv);
		void Stop();

		bool StillRunning();

		const EditorStateFlags& GetEditorStateFlags();
		void SetEditorStateFlags(const EditorStateFlags& flags);
		void ToggleEditorStateFlags(const EditorStateFlags& flags);

		bool ShowingEditorUI();

		// Editor windows
		void OpenEditorWindow(u32 editorWindowType);
		void CloseEditorWindow(const GUID& guid);

		void ResetEditorWindowReferences();
		void OnEntitySelected(const EntityHandle& entity);

	}

}