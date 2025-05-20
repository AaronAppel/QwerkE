
#include "QC_Time.h"

#include "QF_ForcedIncludes.h"

#include "QF_Assets.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

int main(unsigned int numberOfArguments, char** commandLineArguments)
{
	QwerkE::Framework::SetCommandLineArgs(numberOfArguments, commandLineArguments);
	QwerkE::Framework::Initialize();

	// #TODO Update to new loading process
	// Remove #include "QF_Scenes.h"
	// QwerkE::Assets::Load<QwerkE::Scene>(QwerkE::Paths::Scene("NewScene1.qscene"));
	QwerkE::Scenes::CreateSceneFromFile(QwerkE::Paths::Scene("NewScene1.qscene"));

	QwerkE::Time::WriteAppStartTime();

	while (!QwerkE::Window::CloseRequested())
	{
		QwerkE::Time::StartFrame();

		QwerkE::Framework::StartFrame();

		QwerkE::Renderer::StartImGui(); // #TODO Remove ImGui from Game code

		QwerkE::Framework::Update(static_cast<float>(QwerkE::Time::PreviousFrameDuration()));

		if (QwerkE::Input::KeyPressed(QwerkE::QKey::e_Escape))
		{
			QwerkE::Window::RequestClose(); // #TODO For convenience during development
		}

		QwerkE::Renderer::EndImGui(); // #TODO Ensure ImGui is removed from release builds

		QwerkE::Framework::RenderView(1); // #TODO Review hard coded viewId 1

		QwerkE::Framework::EndFrame();
	}

	QwerkE::Framework::Shutdown();
	return 0;
}