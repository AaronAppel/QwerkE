#include "RogueLike_Main.h"
#include "Framework.h"

int main()
{
	QwerkE::Framework::Startup();

	QwerkE::Framework::Run();

	QwerkE::Framework::TearDown();
}