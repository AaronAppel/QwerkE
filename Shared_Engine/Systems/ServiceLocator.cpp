#include "ServiceLocator.h"

namespace QwerkE
{
	// init variables
	ResourceManager* QwerkE::ServiceLocator::m_ResourceManager = nullptr;
	InputManager* QwerkE::ServiceLocator::m_InputManager = nullptr;
	GLFWwindow* QwerkE::ServiceLocator::m_AppWindow = nullptr;

	void ServiceLocator::RegisterService(eEngineServices serviceType, void* service)
	{
		switch (serviceType)
		{
		case eEngineServices::Resource_Manager:
			ServiceLocator::m_ResourceManager = (ResourceManager*)service;
			break;
		case eEngineServices::Input_Manager:
			ServiceLocator::m_InputManager = (InputManager*)service;
			break;
		case eEngineServices::AppWindow:
			ServiceLocator::m_AppWindow = (GLFWwindow*)service;
			break;
		default:
			ConsolePrint("ServiceLocator::RegisterService(): Invalid service!");
			break;
		}
	}

	void* ServiceLocator::UnregisterService(eEngineServices serviceType) // TODO: Potentially dangerous!
	{
		void* temp = nullptr;
		switch (serviceType)
		{
		case eEngineServices::Resource_Manager:
			temp = ServiceLocator::m_ResourceManager;
			ServiceLocator::m_ResourceManager = nullptr;
			return temp;
			break;
		case eEngineServices::Input_Manager:
			temp = ServiceLocator::m_InputManager;
			ServiceLocator::m_InputManager = nullptr;
			return temp;
			break;
		case eEngineServices::AppWindow:
			temp = ServiceLocator::m_AppWindow;
			ServiceLocator::m_AppWindow = nullptr;
			return temp;
			break;
		default:
			ConsolePrint("ServiceLocator::UnregisterService(): Invalid service!");
			break;
		}
	}

	void* ServiceLocator::GetService(eEngineServices serviceType)
	{
		switch (serviceType)
		{
		case eEngineServices::Resource_Manager:
			return ServiceLocator::m_ResourceManager;
			break;
		case eEngineServices::Input_Manager:
			return ServiceLocator::m_InputManager;
			break;
		case eEngineServices::AppWindow:
			return ServiceLocator::m_AppWindow;
			break;
		default:
			ConsolePrint("ServiceLocator::GetService(): Invalid service!");
			break;
		}
	}
	// TODO: Improve logic and code design
	eEngineMessage ServiceLocator::ServicesLoaded() // determine if all services are loaded
	{
		for (eEngineServices i = (eEngineServices)0; i < eEngineServices::Service_Max;)
		{
			switch (i)
			{
			case eEngineServices::Resource_Manager:
				if (ServiceLocator::m_ResourceManager == nullptr)
					return eEngineMessage::_QFail; // not loaded
				break;
			case eEngineServices::Input_Manager:
				if (ServiceLocator::m_InputManager == nullptr)
					return eEngineMessage::_QFail; // not loaded
				break;
			case eEngineServices::AppWindow:
				if (ServiceLocator::m_AppWindow == nullptr)
					return eEngineMessage::_QFail; // not loaded
				break;
			}

			i = (eEngineServices)((int)i + 1); // increment service
		}
		return eEngineMessage::_QSuccess;
	}
}