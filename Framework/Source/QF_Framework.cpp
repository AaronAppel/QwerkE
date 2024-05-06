#include "QF_Framework.h"

#include "QC_System.h"

#include "QF_Assets.h"
#include "QF_Events.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

#include "../../Editor/Source/QE_ProgramArgs.h" // #TODO Move to framework domain

namespace QwerkE {

	namespace Framework {

		static std::map<std::string, const char*> s_ProgramArgumentPairs;

		void local_ProgramArguments(unsigned int argc, char** argv);

		eOperationResult Initialize(unsigned int argc, char** argv)
		{
			local_ProgramArguments(argc, argv);

			Log::Initialize();

			Events::Initialize();

			Window::Initialize();

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

		void local_ProgramArguments(unsigned int argc, char** argv)
		{
			ProgramArgsToPairs(argc, argv, s_ProgramArgumentPairs);

			// Set application directories
			if (s_ProgramArgumentPairs.find(key_NullAssetsDirPath) != s_ProgramArgumentPairs.end())
			{
				Paths::SetNullAssetsDir(s_ProgramArgumentPairs[key_NullAssetsDirPath]);
			}
			if (s_ProgramArgumentPairs.find(key_AssetsDirPath) != s_ProgramArgumentPairs.end())
			{
				Paths::SetAssetsDir(s_ProgramArgumentPairs[key_AssetsDirPath]);
			}

			if (s_ProgramArgumentPairs.find(key_ApplicationTitle) == s_ProgramArgumentPairs.end())
			{
				s_ProgramArgumentPairs.insert(std::pair<const char*, const char*>(key_ApplicationTitle, EngineName));
			}

			if (s_ProgramArgumentPairs.find(key_ProjectFileName) == s_ProgramArgumentPairs.end())
			{
				s_ProgramArgumentPairs.insert(std::pair<const char*, const char*>(key_ProjectFileName, "Project1"));
			}
			else
			{
				// #TODO Load last opened project
			}

			if (s_ProgramArgumentPairs.find(key_UserName) == s_ProgramArgumentPairs.end())
			{
				std::string userName = System::UserName();
				s_ProgramArgumentPairs.insert(std::pair<const char*, const char*>(key_UserName, strdup(userName.c_str())));
			}

			s_ProgramArgumentPairs.insert(std::pair<const char*, const char*>("WorkspaceRootDir", WorkspaceRootDir));

			if (true) { OutputProgramPairsInfo(s_ProgramArgumentPairs); }
		}

	}

}