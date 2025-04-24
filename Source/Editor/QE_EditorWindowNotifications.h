#pragma once

#include "Libraries/imgui-notify/imgui_notify.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowNotifications : public EditorWindow
		{
		public:
			EditorWindowNotifications(GUID guid) :
                EditorWindow("Notifications",
                    EditorWindowTypes::Notifications,
                    guid,
                    (EditorWindowFlags)(EditorWindowFlags::Singleton))
            { }

            bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
                ImGui::SetNextWindowPos({ 50, 50 }, ImGuiCond_Once);
                ImGui::SetNextWindowSize({ 550, 550 }, ImGuiCond_Once);

                if (ImGui::CollapsingHeader("Examples without title", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    if (ImGui::Button("Success"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Hello World! This is a success! %s", "We can also format here:)" });
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Warning"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Warning, 3000, "Hello World! This is a warning!" });
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Error"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Error, 3000, "Hello World! This is an error!" });
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Loading"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Loading, 3000, "Texture: Dummy.txt" });
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Info"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Hello World! This is an info!" });
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Info long"))
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation" });
                    }
                }

                if (ImGui::CollapsingHeader("Do it yourself", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    static char title[4096] = "A wonderful quote!";
                    ImGui::InputTextMultiline("Title", title, sizeof(title));

                    static char content[4096] = "Ours is a friendship forged once in this life, and again in the next. Goodbye, my brother. \n- Dembe Zuma";
                    ImGui::InputTextMultiline("Content", content, sizeof(content));

                    static int duration = 5000; // 5 seconds
                    ImGui::InputInt("Duration (ms)", &duration, 100);
                    if (duration < 0) duration = 0; // Shouldn't be negative

                    static char* type_str[] = { "None", "Success", "Warning", "Error", "Info", "Loading" };
                    static ImGuiToastType type = ImGuiToastType_Success;
                    IM_ASSERT(type < ImGuiToastType_COUNT);

                    if (ImGui::BeginCombo("Type", type_str[type]))
                    {
                        for (auto n = 0; n < IM_ARRAYSIZE(type_str); n++)
                        {
                            const bool is_selected = (type == n);

                            if (ImGui::Selectable(type_str[n], is_selected))
                                type = n;

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }

                    static bool enable_title = true, enable_content = true;
                    ImGui::Checkbox("Enable title", &enable_title);
                    ImGui::SameLine();
                    ImGui::Checkbox("Enable content", &enable_content);

                    if (ImGui::Button("Show"))
                    {
                        ImGuiToast toast(type, duration);

                        if (enable_title)
                            toast.set_title(title);

                        if (enable_content)
                            toast.set_content(content);

                        ImGui::InsertNotification(toast);
                    }
                }
			}

            ImGuiID m_ImGuiId = ImGui::InvalidId();
		};

	}

}