
#include "QC_Time.h"

#include "QF_ForcedIncludes.h"

#include "QF_Assets.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Log.h"
#include "QF_Window.h"

// #NOTE Testing RCCpp
#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/RuntimeObjectSystem.h"

#include "Libraries/RCCppQwerkE/StdioLogSystem.h"
#include "Libraries/RCCppQwerkE/SystemTable.h"

#include "RCCppMainLoop.h"

static StdioLogSystem g_Logger;
static SystemTable g_SystemTable;

bool RCCppInit();
void RCCppCleanup();
void RCCppUpdate();

const int POWERSAVEDRAWNUM = 3;
int powerSaveCountDown = POWERSAVEDRAWNUM;
void ResetPowerSaveCountDown() { powerSaveCountDown = 3; }
//

int main(unsigned int numberOfArguments, char** commandLineArguments)
{
	QwerkE::Framework::SetCommandLineArgs( numberOfArguments, commandLineArguments);
	QwerkE::Framework::Initialize();

	RCCppInit(); // #NOTE Testing RCCpp

	// #TODO Update to new loading process
	// Remove #include "QF_Scenes.h"
	// QwerkE::Assets::Load<QwerkE::Scene>(QwerkE::Paths::Scene("NewScene1.qscene"));
	QwerkE::Scenes::CreateSceneFromFile(QwerkE::Paths::Scene("NewScene1.qscene"));

	QwerkE::Time::WriteAppStartTime();

	bool wasCompiling = false;

	while (!QwerkE::Window::CloseRequested())
	{
		QwerkE::Time::StartFrame();

		QwerkE::Framework::StartFrame();

		RCCppUpdate(); // #NOTE Testing RCCpp
		bool isCompiling = g_pSys->pRuntimeObjectSystem->GetIsCompiling();

		QwerkE::Renderer::StartImGui(); // #TODO Remove ImGui from Game code

		if (wasCompiling && !isCompiling)
		{
			QwerkE::Renderer::OnRecompile();
			QwerkE::Renderer::StartImGui();
		}

		if (!isCompiling)
		{
			g_SystemTable.pRCCppMainLoopI->MainLoop(); // #NOTE Testing RCCpp
		}

		QwerkE::Framework::Update(static_cast<float>(QwerkE::Time::PreviousFrameDuration()));

		if (QwerkE::Input::FrameKeyAction(QwerkE::eKeys::eKeys_Escape, QwerkE::eKeyState::eKeyState_Press))
		{
			QwerkE::Window::RequestClose(); // #TODO For convenience during development
		}

		QwerkE::Renderer::EndImGui(); // #TODO Ensure ImGui is removed from release builds

		constexpr u8 imguiRenderViewId = 0; // #TODO Review hard coded viewId 1
		constexpr u8 gameRenderViewId = 1;
		QwerkE::Framework::RenderView(imguiRenderViewId);

		QwerkE::Framework::EndFrame();

		// #NOTE Testing RCCpp
		// Power save
		if (g_pSys->power_save)
		{
			if (powerSaveCountDown)
			{
				--powerSaveCountDown;
				// glfwPollEvents();
			}
			else
			{
				ResetPowerSaveCountDown();
				// glfwWaitEvents();
			}
		}
		//

		wasCompiling = isCompiling;
	}

	RCCppCleanup(); // #NOTE Testing RCCpp
	QwerkE::Framework::Shutdown();
	return 0;
}

// #NOTE Testing RCCpp
bool RCCppInit()
{
	// g_SystemTable.pImContext = ImGui::GetCurrentContext();

	g_SystemTable.pRuntimeObjectSystem = new RuntimeObjectSystem;
	if (!g_SystemTable.pRuntimeObjectSystem->Initialise(&g_Logger, &g_SystemTable))
	{
		delete g_SystemTable.pRuntimeObjectSystem;
		g_SystemTable.pRuntimeObjectSystem = 0;
		return false;
	}

	g_SystemTable.pRuntimeObjectSystem->CleanObjectFiles();
#ifndef _WIN32
	g_SystemTable.pRuntimeObjectSystem->SetAdditionalCompileOptions("-std=c++11");
#endif

	// #TODO #ifndef _QDEBUG ... #endif // _QDEBUG

	// Set include files to find files required to re-compile
	// Projects source code
	const std::string rootDir = QwerkE::Paths::RepoRootDir();
	// g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Common").c_str());
	g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Framework").c_str());
	// g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Editor").c_str());

	g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source").c_str());
	g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Libraries").c_str());

	// RCCpp files
	g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Libraries\\RCCppRuntimeCompiler\\RuntimeCompiler").c_str());
	g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Libraries\\RCCppRuntimeObjectSystem\\RuntimeObjectSystem").c_str());

	// 3rd party includes
	g_SystemTable.pRuntimeObjectSystem->AddIncludeDir((rootDir + "\\Source\\Libraries\\imgui").c_str());

	g_SystemTable.pRuntimeObjectSystem->SetAutoCompile(true);

	return true;
}

void RCCppCleanup()
{
	delete g_SystemTable.pRuntimeObjectSystem;
}

void RCCppUpdate()
{
	// Check status of any compile
	if (g_SystemTable.pRuntimeObjectSystem->GetIsCompiledComplete())
	{
		// Load module when compile completes
		g_SystemTable.pRuntimeObjectSystem->LoadCompiledModule();
	}

	if (!g_SystemTable.pRuntimeObjectSystem->GetIsCompiling())
	{
		float deltaTime = static_cast<float>(QwerkE::Time::PreviousFrameDuration());
		g_SystemTable.pRuntimeObjectSystem->GetFileChangeNotifier()->Update(deltaTime);
	}
	else
	{
		ResetPowerSaveCountDown();
	}
}

//
