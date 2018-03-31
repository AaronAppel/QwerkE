#ifndef _ServiceLocator_H_
#define _ServiceLocator_H_

#include "Engine_Enums.h"
#include "Utilities/PrintFunctions.h"
#include "Systems/ResourceManager.h"
#include "Systems/Input/InputManager.h"
#include "glfw/glfw3.h"

class ResourceManager;
class InputManager;
class GLFWwindow;

namespace QwerkE
{
	// TODO: Improve service accessing
	class ServiceLocator
	{
	public:
		// Services need to be instantiated, then registered
		// TODO: Services...
		// Audio, Networking, Graphics (Renderer, GUI), Utilities (Conversion, FileIO, Printing),
		// Physics, Event, Debug, Memory, Window, Application, Input, Resources
		static void RegisterService(eEngineServices serviceType, void* service);

		static void* UnregisterService(eEngineServices serviceType); // TODO: Potentially dangerous!

		static void* GetService(eEngineServices serviceType);
		// TODO: Improve logic and code design
		static eEngineMessage ServicesLoaded(); // determine if all services are loaded

	private:
		static ResourceManager* m_ResourceManager;
		static InputManager* m_InputManager;
		static GLFWwindow* m_AppWindow;
	};
}

#endif //!_ServiceLocator_H_
