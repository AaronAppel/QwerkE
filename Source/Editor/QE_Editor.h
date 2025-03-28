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

			UiMainMenuShowingImGuiExample	= 1 << 10,

			Placeholder1					= 1 << 20,

			Placeholder2					= 1 << 30,
		};

		const EditorStateFlags& GetEditorStateFlags();
		void SetEditorStateFlags(const EditorStateFlags& flags);
		void ToggleEditorStateFlags(const EditorStateFlags& flags);

		bool ShowingEditorUI();

		// Editor windows
		void OpenEditorWindow(u32 editorWindowType);
		void CloseEditorWindow(const GUID& guid);

		void OnEntitySelected(EntityHandle& entity);

	}

}