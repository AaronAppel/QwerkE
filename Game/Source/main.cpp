
#include "QC_Time.h"

#include "QF_ForcedIncludes.h" // #TODO Review how to solve includes

#include "QF_Framework.h"
#include "QF_Renderer.h"
#include "QF_Window.h"

int main(unsigned int argc, char** argv)
{
	QwerkE::Framework::Initialize(argc, argv);

	while (!QwerkE::Window::CloseRequested())
	{
		QwerkE::Time::StartFrame();

		QwerkE::Framework::StartFrame();

		QwerkE::Framework::Update((float)QwerkE::Time::PreviousFrameDuration());

		QwerkE::Framework::EndFrame();
	}

	QwerkE::Framework::Shutdown();
	return 0;
}