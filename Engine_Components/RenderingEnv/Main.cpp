#include "Main.h"

int main()
{

#ifdef Qwerk_Engine
    QwerkE::Engine::Run();
#else
    QwerkE::Framework::Startup();

    QwerkE::Framework::Run();

    QwerkE::Framework::TearDown();
#endif // Qwerk_Engine

    return 0;
}