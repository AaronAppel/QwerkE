#include "QF_Framework.h"

#include "QF_Assets.h"
#include "QF_Events.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

#include <string>
#include "QC_Guid.h"
#include "QF_Serialization.h" // #TESTING
#include "QF_Settings.h" // #TESTING

namespace QwerkE {

	namespace Framework {

		eOperationResult Initialize()
		{
			Log::Initialize();

			Events::Initialize();

			Window::Initialize();

			Renderer::Initialize();

			Input::Initialize();

			Assets::Initialize();

			Scenes::Initialize();

			{	// #TESTING
				// int value = 5;
				// EngineSettings value;
				// std::unordered_map<int, bool> value;
				std::vector<std::pair<GUID, std::string>> value;
				value.emplace_back(std::make_pair(GUID(), "ABC"));
				value.emplace_back(std::make_pair(GUID(), "XYZ"));
				// Serialization::NewSerializeObjectToFile(value, "NewSerialization");
			}

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

	}

}