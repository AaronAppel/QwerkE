#include "QF_Framework.h"

#include "QC_ProgramArgs.h"

#include "QF_Assets.h"
#include "QF_Events.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

namespace QwerkE {

	namespace Framework {

		static std::map<std::string, const char*> s_ProgramArgumentPairs;

		// #TODO Review moving scenes to framework, deprecating QF_Scenes.h/cpp
		// static std::vector<Scene*> s_ActiveScenes;

		void local_ProgramArguments(unsigned int numberOfArguments, char** commandLineArguments);

		void SetCommandLineArgs(unsigned int numberOfArguments, char** commandLineArguments)
		{
			assert(numberOfArguments > 0 && commandLineArguments);
			local_ProgramArguments(numberOfArguments, commandLineArguments);
		}

		eOperationResult Initialize(uint16_t windowWidth, uint16_t windowHeight)
		{
			Log::Initialize();
			Events::Initialize();
			Window::Initialize(windowWidth, windowHeight);
			Renderer::Initialize();
			Input::Initialize();
			Assets::Initialize();
			Scenes::Initialize();
			return eOperationResult::Success;
		}

		eOperationResult Shutdown()
		{
			Scenes::Shutdown();
			Assets::Shutdown();
			Events::Shutdown();
			Renderer::Shutdown();
			Window::Shutdown();
			Log::Shutdown();
			return eOperationResult::Success;
		}

		void StartFrame()
		{
			Input::NewFrame();
			Events::ProcessEvents();
			// Jobs::ProcessTasks();
			Window::NewFrame();
		}

		void Update(float deltatime)
		{
			Scenes::Update(deltatime);
		}

		void RenderView(u16 viewId)
		{
			Scenes::DrawCurrentScene(viewId);
		}

		void EndFrame()
		{
			Renderer::EndFrame();
		}

		std::map<std::string, const char*>& GetProgramArgumentPairs()
		{
			return s_ProgramArgumentPairs;
		}

		void local_ProgramArguments(unsigned int numberOfArguments, char** commandLineArguments)
		{
			ProgramArgsToPairs(numberOfArguments, commandLineArguments, s_ProgramArgumentPairs);

			Paths::SetExePath(s_ProgramArgumentPairs[key_ExePath]);

			if (s_ProgramArgumentPairs.find(key_OverrideNullAssetsDirPath) != s_ProgramArgumentPairs.end())
			{
				Paths::SetNullAssetsDir(s_ProgramArgumentPairs[key_OverrideNullAssetsDirPath]);
			}
			if (s_ProgramArgumentPairs.find(key_OverrideAssetsDirPath) != s_ProgramArgumentPairs.end())
			{
				Paths::SetAssetsDir(s_ProgramArgumentPairs[key_OverrideAssetsDirPath]);
			}

			if (true) { OutputProgramPairsInfo(s_ProgramArgumentPairs); }
		}

	}

}