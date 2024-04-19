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
			IsOpen					= 1 << 0,
			Hidden					= 1 << 1,
			ExactNameNoguid			= 1 << 2,
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
				// #TODO Check if the window is also EditorWindowFlags::IsOpen?
				if (m_WindowFlags & EditorWindowFlags::Hidden)
				{
					DrawInternal();
					return;
				}

				if (!Editor::ShowingEditorUI() || Window::IsMinimized())
					return;

				// #TODO IsOpen not working
				bool isOpen = m_WindowFlags & EditorWindowFlags::IsOpen;
				m_IsOpen; // #TODO Deprecate member

				if (ImGui::Begin(m_WindowName.c_str(), &isOpen, m_ImGuiFlags))
				{
					DrawInternal();
				}
				ImGui::End();
				u32 result = isOpen ? m_WindowFlags | EditorWindowFlags::IsOpen : m_WindowFlags & ~EditorWindowFlags::IsOpen;
				m_WindowFlags = (EditorWindowFlags)result;
			}

			virtual bool IsUnique() { return false; }

			GUID Guid() { return m_Guid; }
			EditorWindowTypes Type() { return m_EditorWindowType; }

			virtual void OnEntitySelected(const EntityHandle& entity) { }

		protected:
			EditorWindow(std::string windowName, EditorWindowTypes editorWindowType, GUID guid = GUID::Invalid, EditorWindowFlags flags = EditorWindowFlags::IsOpen)
				:	m_WindowName(windowName),
					m_EditorWindowType(editorWindowType),
					m_WindowFlags(flags)
			{
				if (guid != GUID::Invalid)
				{
					m_Guid = guid; // #TODO Look at initializing m_Guid(guid) in initializer list
				}

				EditorWindowFlags avoidAppendingGuidToName = (EditorWindowFlags)(m_WindowFlags & ExactNameNoguid);
				if (!avoidAppendingGuidToName)
				{
					m_WindowName += "##";
					m_WindowName += std::to_string(m_Guid);
				}
			}

			virtual void DrawInternal() = 0;

			ImGuiWindowFlags m_ImGuiFlags = 0;
			EditorWindowFlags m_WindowFlags = EditorWindowFlagsNone;

			const GUID& GetGuid() { return m_Guid; }

		private:
			MIRROR_PRIVATE_MEMBERS

			std::string m_WindowName;
			GUID m_Guid; // #NOTE Window is static so has order dependency with GUID engine init
			bool m_IsOpen = true;

			// #TODO Handle resizing window
			u16 m_MinimumWidth = 0;
			u16 m_MinimumHeight = 0;

			EditorWindowTypes m_EditorWindowType;
		};

	}

}