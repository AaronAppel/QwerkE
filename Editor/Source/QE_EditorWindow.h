#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QC_Guid.h"

#include "QF_EntityHandle.h"
#include "QF_Window.h"

#include "QE_Editor.h"

typedef int ImGuiWindowFlags;

namespace QwerkE {

	namespace Editor {

		typedef unsigned int u32;
		enum EditorWindowFlags : u32
		{
			EditorWindowFlagsNone	= 0,
			Headless				= 1 << 0,
			ExactNameNoguid			= 1 << 1,
			Hidden					= 1 << 2,
		};

		QC_ENUM(EditorWindowTypes, u32,
			EditorWindowTypesInvalid = 0,
			Assets,
			DefaultDebug,
			DockingContext,
			EntityInspector,
			ImGuiDemo,
			MenuBar,
			SceneControls,
			SceneGraph,
			SceneView,
			Settings = 10,
			StylePicker,
			MaterialEditor,
			FolderViewer,
			NodeEditor,
			ShaderEditor
			// #NOTE Serialized, so don't change ordering
		)

		class EditorWindow
		{
		public:
			virtual ~EditorWindow() = default;

			void EditorWindow::Draw()
			{
				if (!Editor::ShowingEditorUI() || Window::IsMinimized() ||
					m_WindowFlags & EditorWindowFlags::Hidden)
					return;

				// #TODO Check if the window is also EditorWindowFlags::IsOpen?
				if (m_WindowFlags & EditorWindowFlags::Headless)
				{
					DrawInternal();
					return;
				}

				bool isOpen = true;
				if (ImGui::Begin(m_WindowName.c_str(), &isOpen, m_ImGuiFlags))
				{
					DrawInternal();
				}
				ImGui::End();

				if (!isOpen)
				{
					Close();
				}
			}

			virtual bool IsUnique() { return false; }

			GUID Guid() { return m_Guid; }
			EditorWindowTypes Type() { return m_EditorWindowType; }

			virtual void OnEntitySelected(const EntityHandle& entity) { }

			void ToggleHidden() { m_WindowFlags = (EditorWindowFlags)(m_WindowFlags ^ Hidden); }

		protected:
			EditorWindow(std::string windowName, EditorWindowTypes editorWindowType, GUID guid = GUID(), EditorWindowFlags flags = EditorWindowFlags::EditorWindowFlagsNone)
				:	m_WindowName(windowName),
					m_EditorWindowType(editorWindowType),
					m_WindowFlags(flags),
					m_Guid(guid)
			{
				EditorWindowFlags avoidAppendingGuidToName = (EditorWindowFlags)(m_WindowFlags & ExactNameNoguid);
				if (!avoidAppendingGuidToName)
				{
					m_WindowName += "##";
					m_WindowName += std::to_string(m_Guid);
				}
			}

			void Close() { Editor::CloseEditorWindow(m_Guid); }

			virtual void DrawInternal() = 0;

			ImGuiWindowFlags m_ImGuiFlags = 0;
			EditorWindowFlags m_WindowFlags = EditorWindowFlagsNone;

			const GUID& GetGuid() { return m_Guid; }

		private:
			MIRROR_PRIVATE_MEMBERS

			std::string m_WindowName = Constants::gc_DefaultStringValue;
			GUID m_Guid; // #NOTE Window is static so has order dependency with GUID engine init

			// #TODO Handle resizing window
			u16 m_MinimumWidth = 0;
			u16 m_MinimumHeight = 0;

			EditorWindowTypes m_EditorWindowType;
		};

	}

}