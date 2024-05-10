
#include "Libraries/QwerkECommon/QC_Time.h"

#include "QF_ForcedIncludes.h" // #TODO Review how to solve includes

#include "QF_Framework.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

int main(unsigned int argc, char** argv)
{
	QwerkE::Framework::StartUpArguments startUpArgs(
		argc,
		argv,
		1920,
		1080
	);

	QwerkE::Framework::Initialize(startUpArgs);

	QwerkE::Scenes::CreateSceneFromFile(QwerkE::Paths::Scene("GameScene1.qscene"));

	QwerkE::Time::WriteAppStartTime();

	while (!QwerkE::Window::CloseRequested())
	{
		QwerkE::Time::StartFrame();

		QwerkE::Framework::StartFrame();

		QwerkE::Framework::Update((float)QwerkE::Time::PreviousFrameDuration());

		QwerkE::Framework::RenderView(1);

		QwerkE::Framework::EndFrame();
	}

	QwerkE::Framework::Shutdown();
	return 0;
}