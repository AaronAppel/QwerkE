#include "QF_Framework.h"

#include "QC_ProgramArgs.h"

#include "QF_Assets.h"
#include "QF_Events.h"

#include "QF_Input.h"

#include "QF_Audio.h"
#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Physics.h"
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
			Audio::Initialize();
			Assets::Initialize(); // #TODO Depends on Renderer (bgfx) init for mesh, shader, texture creation, Audio (OpenAL) for sound creation
			Physics::Initialize();

			Scenes::Initialize(); // #TODO Depends on physics, and Assets? Last just to be safe
			return eOperationResult::Success;
		}

		eOperationResult Shutdown()
		{
			Scenes::Shutdown(); // First just to be safe

			Physics::Shutdown();
			Window::Shutdown();
			Assets::Shutdown(); // #TODO Renderer (bgfx pthread_mutex_unlock(_mutex) in bx/mutex.cpp line 95) shutdown order dependency
			Audio::Shutdown(); // After Assets::Shutdown so Sound files have been destroyed or OpenAL will complain of lingering sound buffers
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
			Physics::StepSimulation(); // #TODO Handle fixed time step logic
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
