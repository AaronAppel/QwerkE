#pragma once

namespace QwerkE {

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

			UiMainMenuShowingImGuiExample	= 1 << 10,

			Placeholder1					= 1 << 20,

			Placeholder2					= 1 << 30,
		};

		void Run(unsigned int argc, char** argv);
		void Stop();

		bool StillRunning();

		void ResetReferences();

		const EditorStateFlags& GetEditorStateFlags();
		void SetEditorStateFlags(const EditorStateFlags& flags);

		bool ShowingEditorUI();

		void OpenNewEditorWindow(u32 editorWindowType);

	}

}