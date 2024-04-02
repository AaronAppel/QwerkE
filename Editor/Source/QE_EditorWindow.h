#pragma once

namespace QwerkE {

	namespace Editor {

		class EditorWindow
		{
		public:
			void Create();
			void Destroy();

			void Draw();

		private:
			EditorWindow() = default;

			void Initialize();
			void Shutdown();

			void EditorDraw();

			void DrawMainMenuBar();
			void DrawDockingContext();
			void DrawStylePicker(bool save);

		};

	}

}