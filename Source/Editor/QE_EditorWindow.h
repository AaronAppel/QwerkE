#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
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
			Singleton				= 1 << 3,
			AlignCentered			= 1 << 4,
		};

		QC_ENUM(EditorWindowTypes, u32,
			// #NOTE Order matters! Serialized! Match enum EditorWindowsList order
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
			Settings,
			StylePicker,
			MaterialEditor,
			FolderViewer,
			NodeEditor,
			ShaderEditor,
			PrefabScene,
			Prompt,
			Console
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

				// #TODO Find why new windows are still too small, and if this check does anything
				if ((m_WindowFlags & EditorWindowFlags::Hidden) == 0)
				{
					ImGui::SetNextWindowSizeConstraints({ m_MinimumWidth , m_MinimumHeight }, { FLT_MAX, FLT_MAX });
				}

				// #TODO Check if the window is also EditorWindowFlags::IsOpen?
				if (m_WindowFlags & EditorWindowFlags::Headless)
				{
					DrawInternal();
					return;
				}

				if (m_WindowFlags & EditorWindowFlags::AlignCentered)
				{
					const vec2f& windowSize = Window::GetSize();
					vec2f newWindowPosition = vec2f(windowSize.x / 2.f, windowSize.y / 2.f);
					newWindowPosition.x -= m_MinimumWidth / 2.f;
					newWindowPosition.y -= m_MinimumHeight / 2.f;
					ImGui::SetNextWindowPos(ImVec2(newWindowPosition.x, newWindowPosition.y));
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
			GUID* GuidAddress() { return &m_Guid; }
			EditorWindowTypes Type() { return m_EditorWindowType; } // #TODO Make static
			EditorWindowFlags WindowFlags() { return m_WindowFlags; }

			virtual void OnEntitySelected(EntityHandle& entity) { }
			virtual void OnSceneReload() { }

			void ToggleHidden() { m_WindowFlags = (EditorWindowFlags)(m_WindowFlags ^ Hidden); }

		protected:
			EditorWindow(std::string windowName, EditorWindowTypes editorWindowType, GUID guid = GUID(), EditorWindowFlags flags = EditorWindowFlags::EditorWindowFlagsNone)
				:	m_WindowName(windowName),
					m_EditorWindowType(editorWindowType),
					m_WindowFlags(flags),
					m_Guid(guid)
			{
				EditorWindowFlags avoidAppendingGuidToName = (EditorWindowFlags)(m_WindowFlags & ExactNameNoguid);
				if (!m_Guid)
				{
					int bp = 0;
				}

				if (!avoidAppendingGuidToName)
				{
					m_WindowName += "##";
					m_WindowName += std::to_string(m_Guid);
				}
			}

			void Close() { Editor::CloseEditorWindow(m_Guid); }

			virtual void DrawInternal() = 0;

			ImGuiWindowFlags m_ImGuiFlags = ImGuiWindowFlags_::ImGuiWindowFlags_None;
			EditorWindowFlags m_WindowFlags = EditorWindowFlags::EditorWindowFlagsNone;

			const GUID& GetGuid() { return m_Guid; }

			const float m_StartingWidth = 100.f;
			const float m_StartingHeight = 150.f;

			float m_MinimumWidth = m_StartingWidth;
			float m_MinimumHeight = m_StartingHeight;

			std::string m_WindowName = Constants::gc_DefaultStringValue;

		private:
			MIRROR_PRIVATE_MEMBERS

			GUID m_Guid = GUID::Invalid; // #NOTE Singleton windows are static and have GUID dependency with init

			EditorWindowTypes m_EditorWindowType;
		};

	}

}