#include "QE_EditorWindowConsole.h"

#include "Libraries/imgui-console/imgui_console.h"

csys::ItemLog& operator<<(csys::ItemLog& log, ImVec4& vec)
{
    log << "ImVec4: [" << vec.x << ", "
        << vec.y << ", "
        << vec.z << ", "
        << vec.w << "]";
    return log;
}

static void imvec4_setter(ImVec4& my_type, std::vector<int> vec)
{
    if (vec.size() < 4) return;

    my_type.x = vec[0] / 255.f;
    my_type.y = vec[1] / 255.f;
    my_type.z = vec[2] / 255.f;
    my_type.w = vec[3] / 255.f;
}

namespace QwerkE {

    namespace Editor {

        EditorWindowConsole::EditorWindowConsole(GUID guid) :
            EditorWindow("Console",
                EditorWindowTypes::Console,
                guid,
                // #TODO Avoid constructing already serialized data
                // m_WindowFlags)
                (EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Singleton))
        {
            // #TODO Update editor last focused on console window focused
            // if (ImGui::IsWindowFocused() && this != Editor::GetLastFocusedWindow())
            // {
            //     Editor::OnEditoreWindowFocused(this);
            // }

            m_Console.System().RegisterVariable("background_color", m_ClearColor, imvec4_setter);

            // #TODO Move to scripts folder or find a better way to handle console script files
            const std::string path = "B:/QwerkE/Source/Libraries/imgui-console/console.script";
            m_Console.System().RegisterScript("test_script", path);

            m_Console.System().RegisterCommand("random_background_color", "Assigns a random color to the background application",
                [this]()
                {
                    m_ClearColor.x = (rand() % 256) / 256.f;
                    m_ClearColor.y = (rand() % 256) / 256.f;
                    m_ClearColor.z = (rand() % 256) / 256.f;
                    m_ClearColor.w = (rand() % 256) / 256.f;
                });
            m_Console.System().RegisterCommand("reset_background_color", "Reset background color to its original value",
                [this, val = m_ClearColor]()
                {
                    m_ClearColor = val;
                });

            m_Console.System().Log(csys::ItemType::INFO) << "Welcome to imgui-console!" << csys::endl;
        }

        EditorWindowConsole::~EditorWindowConsole()
        {
            m_Console.UnregisterImGuiHandler();
        }

	}

}