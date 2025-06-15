#pragma once

#include "Libraries/imgui/imgui_internal.h"

#include "QF_Window.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowDockingContext : public EditorWindow
		{
		public:
            EditorWindowDockingContext(GUID guid) :
                EditorWindow("Docking Context",
                    EditorWindowTypes::DockingContext,
                    guid,
                    (EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Singleton))
            { }

            bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
                // #TODO Checkout: https://github.com/ocornut/imgui/issues/6779#issuecomment-1704863221
                // ImGui::DockSpaceOverViewport();

                // #NOTE Copied from imgui docking example
                static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

                static bool opt_fullscreen = true;
                if (opt_fullscreen)
                {
                    ImGuiViewport* viewport = ImGui::GetMainViewport();
                    ImGui::SetNextWindowPos(viewport->Pos);
                    ImGui::SetNextWindowSize(viewport->Size);
                    ImGui::SetNextWindowViewport(viewport->ID);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
                    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                }

                static bool open = true;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
                ImGui::Begin("DockSpace Demo", &open, window_flags);

                if (ImGui::IsWindowFocused() && this != Editor::GetLastFocusedWindow())
                {
                    Editor::OnEditorWindowFocused(this);
                }

                ImGui::PopStyleVar();
                if (opt_fullscreen)
                {
                    ImGui::PopStyleVar(2);
                }

                if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
                {
                    if (!m_ImGuiId)
                    {
                        m_ImGuiId = ImGui::GetID("QwerkEDockSpace");
                    }
                    ImGui::DockSpace(m_ImGuiId, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_None);
                }

                ImGui::End();
			}

            ImGuiID m_ImGuiId = ImGui::InvalidId();
		};

	}

}