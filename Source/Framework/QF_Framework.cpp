#include "QF_Framework.h"

#include "QC_ProgramArgs.h"

#include "QF_Assets.h"
#include "QF_Events.h"

#include "QF_Input.h"

#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

namespace QwerkE {

	namespace Framework {

		static std::map<std::string, const char*> s_ProgramArgumentPairs;

		// #TODO Review moving Scene and Scenes to Editor as Framework users may handle their own scenes
		// #TODO Review moving scenes to framework, deprecating QF_Scenes.h/cpp
		// static std::vector<Scene*> s_ActiveScenes;

		// #TODO Review moving CLI arguments to Editor as Framework users may just want to handle CLI themselves, or not at all
		void local_ProgramArguments(unsigned int numberOfArguments, char** commandLineArguments);

		void SetCommandLineArgs(unsigned int numberOfArguments, char** commandLineArguments)
		{
			assert(numberOfArguments > 0 && commandLineArguments);
			local_ProgramArguments(numberOfArguments, commandLineArguments);
		}

		eOperationResult Initialize(const StartUpData& startUpData)
		{
			ASSERT(!s_ProgramArgumentPairs.empty(), "Initialize() called before SetCommandLineArgs()!");

			if (startUpData.numberOfCommandLineArguments && startUpData.commandLineArguments)
			{
				QwerkE::Framework::SetCommandLineArgs(startUpData.numberOfCommandLineArguments, startUpData.commandLineArguments);
			}

			// #TODO Assign any startUpData arguments from command line?

			// #TODO Review return values. Could chain successful calls together using return value like
			// if (Log::Initialize() && Events::Initialize() && ... etc) { return eOperationResult::Success }

			Log::Initialize();
			Events::Initialize();
			Window::Initialize(startUpData);
			Renderer::Initialize();
			Input::Initialize();
			Assets::Initialize(); // #TODO bgfx init order dependency for mesh creation
			Scenes::Initialize();
			return eOperationResult::Success;
		}

		eOperationResult Shutdown()
		{
			Scenes::Shutdown();
			Window::Shutdown();
			Assets::Shutdown(); // #TODO bgfx shutdown order dependency pthread_mutex_unlock(_mutex) in bx/mutex.cpp line 95
			Renderer::Shutdown();
			Events::Shutdown();
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

		const std::map<std::string, const char*>& GetProgramArgumentPairs()
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
