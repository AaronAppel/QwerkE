#ifndef _ServiceLocator_H_
#define _ServiceLocator_H_

// Class ServiceLocator provides global access for systems
// necessary throughout the code such as memory management,
// audio playback, input, etc.
// To add a system add it to RegisterService(), UnregisterService,
// GetService(), and ServicesLoaded().

#include "../Engine_Enums.h"
#include "./../../Shared_Generic/Utilities/PrintFunctions.h"
#include "ResourceManager.h"
#include "Input/InputManager.h"

class ResourceManager;
class InputManager;
class GLFWwindow;
class EventManager;
class SceneManager;
class Factory;
class Editor;
class PhysicsManager;
class MessageManager;
class Renderer;
class AudioManager;
class Debugger;
class Time;
class JobManager;

namespace QwerkE
{
	// TODO: Improve service accessing
	// Think of toggling register/unregister
	class ServiceLocator
	{
	public:
		// Services need to be instantiated, then registered
		// TODO: Services...
		// Audio, Networking, Graphics, Utilities (some type of file managers?),
		// Debug, Memory, Application
		static void RegisterService(eEngineServices serviceType, void* service);

		static void* UnregisterService(eEngineServices serviceType); // TODO: Potentially dangerous!

		static void* GetService(eEngineServices serviceType);
		// TODO: Improve logic and code design
		static eEngineMessage ServicesLoaded(); // determine if all services are loaded

		static void LockServices(bool locking) { m_IsLocked = locking; }

	private:
		static bool m_IsLocked; // TODO: Improve security/error prevention

		static ResourceManager* m_ResourceManager;
		static InputManager* m_InputManager;
		static GLFWwindow* m_AppWindow;
        static EventManager* m_EventManager;
        static SceneManager* m_SceneManager;
        static Factory* m_Factory;
        static Editor* m_Editor;
		static PhysicsManager* m_PhysicsManager;
		static MessageManager* m_MessageManager;
		static Renderer* m_Renderer;
        static AudioManager* m_AudioManager;
        static Debugger* m_Debugger;
        static Time* m_Time;
		static JobManager* m_JobManager;
	};
}

#endif //!_ServiceLocator_H_
