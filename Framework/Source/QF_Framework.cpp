#include "QF_Framework.h"

#include "QF_Assets.h"
#include "QF_Events.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

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