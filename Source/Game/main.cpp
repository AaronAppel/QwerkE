
#include "QC_Time.h"

#include "QF_ForcedIncludes.h" // #TODO Review how to solve includes

#include "QF_EntityHandle.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

int main(unsigned int numberOfArguments, char** commandLineArguments)
{
	QwerkE::Framework::SetCommandLineArgs( numberOfArguments, commandLineArguments);
	QwerkE::Framework::Initialize();

	QwerkE::Scenes::CreateSceneFromFile(QwerkE::Paths::Scene("NewScene1.qscene"));

	QwerkE::Time::WriteAppStartTime();

	while (!QwerkE::Window::CloseRequested())
	{
		QwerkE::Time::StartFrame();

		QwerkE::Framework::StartFrame();

		QwerkE::Renderer::StartImGui(); // #TODO Remove ImGui from Game code

		QwerkE::Framework::Update((float)QwerkE::Time::PreviousFrameDuration());

		if (QwerkE::Input::FrameKeyAction(QwerkE::eKeys::eKeys_Escape, QwerkE::eKeyState::eKeyState_Press))
		{
			QwerkE::Window::RequestClose(); // #TODO For convenience during development
		}

		QwerkE::Renderer::EndImGui(); // #TODO Remove ImGui from Game code

		QwerkE::Framework::RenderView(1);

		QwerkE::Framework::EndFrame();
	}

	QwerkE::Framework::Shutdown();
	return 0;
}