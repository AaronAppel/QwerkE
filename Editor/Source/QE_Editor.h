#pragma once

namespace QwerkE {

	namespace Editor {

		typedef unsigned long long u64;
		enum EditorDirtyFlags : u64
		{
			None				= 0,

			// #TODO Refactor framework state
			EngineSettingsState	= 1 << 0,
			UserSettingsState	= 1 << 1,
			ProjectState		= 1 << 2,
			SceneState			= 1 << 3,
		};

		void Run(unsigned int argc, char** argv);
		void Stop();

		bool StillRunning();

		void OnReset();

		const EditorDirtyFlags& DirtyFlags();
	}

}