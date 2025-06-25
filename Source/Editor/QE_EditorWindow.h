#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#include "Libraries/imgui_toggle/imgui_toggle.h"
#include "Libraries/imgui/imgui_extended.h"
#include "Libraries/imgui/imgui_internal.h"
#endif

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QC_Guid.h"

#include "QF_EntityHandle.h"
#include "QF_Input.h"
#include "QF_Window.h"

#include "QE_Editor.h"

typedef int ImGuiWindowFlags;

namespace QwerkE {

	namespace Editor {

		typedef unsigned int u32;
		enum EditorWindowFlags : u32 // #TODO Refactor to short or byte
		{
			EditorWindowFlagsNone	= 0,
			Headless				= 1 << 0,
			ExactNameNoguid			= 1 << 1,
			Hidden					= 1 << 2,
			Singleton				= 1 << 3,
			AlignCentered			= 1 << 4,
			// #TODO DefaultDebugWindow should minimize or hide but never really close as
			// debug calls will still render in a default window.
			HideInsteadOfClose		= 1 << 5,
			HideWindowOptions		= 1 << 6,
			InitWithWindowSettings	= 1 << 7,
		};

		typedef unsigned char u8;
		enum EditorWindowSizingFlags : u8
		{
			EditorWindowSizingFlagsNone = 0,
			// #TODO Reference imgui_demo.cpp(7840) ShowExampleAppConstrainedResize()

			// Considerations:
			// - Height and Width handled separately, with options to lock or relate (scale) with each other
			// - Between (min + max)
		};

		struct EditorWindowOptions
		{
			// Init
			float m_InitialWidth = 320.f;
			float m_InitialHeight = 180.f;

			float m_InitialPositionX = 0.f;
			float m_InitialPositionY = 0.f;

			// Sizing
			EditorWindowSizingFlags m_SizingFlags = EditorWindowSizingFlagsNone;
			float m_WidthMinimum = 0;
			float m_WidthMaximum = FLT_MAX;

			float m_HeightMinimum = 0;
			float m_HeightMaximum = FLT_MAX;

			bool m_LockWidth = false;
			bool m_LockHeight = false;

			s32 m_RatioWidth = 1;
			s32 m_RatioHeight = 1;

			s32 m_IncrementWidth = 1;
			s32 m_IncrementHeight = 1;

			// Style
			float m_ItemScaling = 1.f;
			float m_FontScaling = 1.f;
			ImGuiStyle m_Style;
			ImFont m_Font;

			void Edit()
			{
				if (ImGui::BeginPopup("##EditorWindowOptionsEdit", ImGuiPopupFlags_NoReopen))
				{
					constexpr float itemWidth = 100.f;
					ImGui::PushItemWidth(itemWidth);

					ImGui::Text("Height");
					ImGui::SameLine();
					ImGui::Text("   ");
					ImGui::SameLine();
					ImGui::Text("Width");

					ImGui::ScrollerFloat("##WidthMinimum", &m_WidthMinimum, 10, 100);
					ImGui::SameLine();
					ImGui::Text("Min");
					ImGui::SameLine();
					ImGui::ScrollerFloat("##HeightMinimum", &m_HeightMinimum, 10, 100);

					ImGui::ScrollerFloat("##WidthMaximum", &m_WidthMaximum, 10, 100);
					ImGui::SameLine();
					ImGui::Text("Max");
					ImGui::SameLine();
					ImGui::ScrollerFloat("##HeightMaximum", &m_HeightMaximum, 10, 100);

					ImGui::Toggle("##LockWidth", &m_LockWidth, ImGuiToggleFlags_Animated | ImGuiToggleFlags_A11y);
					ImGui::SameLine();
					ImGui::Text("Lock");
					ImGui::SameLine();
					ImGui::Toggle("##LockHeight", &m_LockHeight, ImGuiToggleFlags_Animated | ImGuiToggleFlags_A11y);

					ImGui::ScrollerInt("##RatioWidth", &m_RatioWidth, 1, 10);
					ImGui::SameLine();
					ImGui::Text("Ratio");
					ImGui::SameLine();
					ImGui::ScrollerInt("##RatioHeight", &m_RatioHeight, 1, 10);

					ImGui::ScrollerInt("##IncrementWidth", &m_IncrementHeight, 10, 100);
					ImGui::SameLine();
					ImGui::Text("Step");
					ImGui::SameLine();
					ImGui::ScrollerInt("##IncrementHeight", &m_IncrementWidth, 10, 100);

					ImGui::PopItemWidth();

					ImGui::EndPopup();
				}
			}

			void NewFrame()
			{
				// Set options
				ImGui::SetNextWindowSizeConstraints(ImVec2(m_WidthMinimum, m_HeightMinimum), ImVec2(m_WidthMaximum, m_HeightMaximum));
			}

			// #TODO Add options to each window, set them, serialize, load, and save/edit using a button pop up that is added to all inherited windows
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
			Unused, /* Previously SceneControls*/
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
			Console,
			HexEditor,
			ImGuiExtendedDemo,
			Notifications,
			Timeline,
			DebugInput,
			DebugRenderer,
			DebugEvents,
			DebugWindows,
			DebugPhysics,
			DebugMemory,
			DebugProfiler,
			InputMapping,
			FileEditor,
			StatusBar,
			WelcomeWindow
			// #NOTE Serialized, so don't change ordering
		)

		class EditorWindow
		{
		public:
			~EditorWindow()
			{
				if (this == Editor::GetLastFocusedWindow())
				{
					Editor::OnEditorWindowFocused(nullptr);
				}
			}

			void EditorWindow::Draw()
			{
				if (!Editor::ShowingEditorUI() || Window::IsMinimized() || m_WindowFlags & EditorWindowFlags::Hidden)
				{
					if (this == Editor::GetLastFocusedWindow())
					{
					    Editor::OnEditorWindowFocused(nullptr);
					}
					return;
				}

				// #TODO Find why new windows are still too small, and if this check does anything
				if ((m_WindowFlags & EditorWindowFlags::Hidden) == 0)
				{
					ImGui::SetNextWindowSizeConstraints({ m_MinimumWidth , m_MinimumHeight }, { FLT_MAX, FLT_MAX });
				}

				// #TODO Check if the window is also EditorWindowFlags::IsOpen?
				if (m_WindowFlags & EditorWindowFlags::Headless) // #TODO Headless windows need to check if focused and call Editor::OnEditoreWindowFocused(this)
				{
					DrawInternal();
					return;
				}

				if (!m_HasInitialized && m_WindowFlags & EditorWindowFlags::InitWithWindowSettings)
				{
					// #TODO InitWithWindowSettings can be removed. Always init a new window, but need to determine if it was newly created, not de-serialized.
					ImGui::SetNextWindowSize(ImVec2(m_WindowOptions.m_InitialWidth, m_WindowOptions.m_InitialHeight));
					ImGui::SetNextWindowPos(ImVec2(m_WindowOptions.m_InitialPositionX, m_WindowOptions.m_InitialPositionY));
					m_HasInitialized = true;
				}

				if (m_WindowFlags & EditorWindowFlags::AlignCentered)
				{
					const vec2f& windowSize = Window::GetSize();
					vec2f newWindowPosition = vec2f(windowSize.x / 2.f, windowSize.y / 2.f);
					newWindowPosition.x -= m_MinimumWidth / 2.f;
					newWindowPosition.y -= m_MinimumHeight / 2.f;
					ImGui::SetNextWindowPos(ImVec2(newWindowPosition.x, newWindowPosition.y));
				}

				bool isOpen = true; // #TODO Use window state for open/close? If window exists, it is either hidden or open
				if (isOpen)
				{
					m_WindowOptions.NewFrame();
				}

				if (ImGui::Begin(m_WindowName.c_str(), &isOpen, m_ImGuiFlags))
				{
					if (ImGui::IsWindowFocused() && this != Editor::GetLastFocusedWindow())
					{
						Editor::OnEditorWindowFocused(this);
					}

					// #TODO Console doesn't focus on middle and right click because it doesn't execute this code
					if (ImGui::IsWindowHovered() &&
						(Input::MousePressed(e_MouseRight) || Input::MousePressed(e_MouseMiddle)))
					{
						ImGui::FocusWindow(ImGui::GetCurrentWindow());
					}

					if (!(m_WindowFlags & EditorWindowFlags::HideWindowOptions))
					{
						const ImVec2 previousCursorPos = ImGui::GetCursorPos();
						// ImGui::SameLine(0.f, ImGui::GetContentRegionAvail().x / 2 - (10 * 7.f));
						// if (ImGui::SmallButton("Edit Window Options"))
						ImGui::SameLine(0.f, ImGui::GetContentRegionAvail().x - 7.f);
						if (ImGui::SmallButton(":"))
						{
							ImGui::OpenPopup("##EditorWindowOptionsEdit");
						}
						ImGui::SetCursorPos(previousCursorPos - ImVec2{ 0.f, 2.f });
						m_WindowOptions.Edit();
					}
					DrawInternal();
				}
				ImGui::End();

				if (!isOpen)
				{
					Close();
				}
			}

			virtual bool IsUnique() { return false; } // #TODO Can EditorWindowFlags::Singleton flag replace method or true/false?

			GUID Guid() { return m_Guid; }
			GUID* GuidAddress() { return &m_Guid; }
			EditorWindowTypes Type() const { return m_EditorWindowType; } // #TODO Make static?
			EditorWindowFlags WindowFlags() const { return m_WindowFlags; }
			const std::string& Name() const { return m_WindowName; };

			virtual void OnEditorWindowFocused(const EditorWindow* const a_FocusedWindow) { }
			virtual void OnEntitySelected(EntityHandle& entity) {}
			virtual void OnSceneReload(const Scene* a_ReloadedScene) { }

			bool IsHidden() { return (EditorWindowFlags)(m_WindowFlags & Hidden); }// #TODO Singleton check? Can only singletons be hidden?
			void ToggleHidden() { m_WindowFlags = (EditorWindowFlags)(m_WindowFlags ^ Hidden); }
			void Focus() { ImGui::SetWindowFocus(m_WindowName.data()); Editor::OnEditorWindowFocused(this); }
			// #TODO void Highlight() { ImGui::NavUpdateWindowingHighlightWindow(); }

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

			EditorWindowOptions m_WindowOptions;

			const GUID& GetGuid() { return m_Guid; }

			bool m_HasInitialized = false;

			const float m_StartingWidth = 100.f; // #TODO Review as m_WindowOptions has been added
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