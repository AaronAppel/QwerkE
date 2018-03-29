#include "ServiceLocator.h"

namespace QwerkE
{
	// init variables
	ResourceManager* QwerkE::ServiceLocator::m_ResourceManager = nullptr;
	InputManager* QwerkE::ServiceLocator::m_InputManager = nullptr;

	void ServiceLocator::RegisterService(eEngineServices serviceType, void* service)
	{
		switch (serviceType)
		{
		case eEngineServices::Resource_Manager:
			m_ResourceManager = (ResourceManager*)service;
			break;
		case eEngineServices::Input_Manager:
			m_InputManager = (InputManager*)service;
			break;
		default:
			ConsolePrint("ServiceLocator::RegisterService(): Invalid service!");
			break;
		}
	}

	void ServiceLocator::UnregisterService(eEngineServices serviceType) // TODO: Potentially dangerous!
	{
		switch (serviceType)
		{
		case eEngineServices::Resource_Manager:
			m_ResourceManager = nullptr;
			break;
		case eEngineServices::Input_Manager:
			m_ResourceManager = nullptr;
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
			return m_ResourceManager;
			break;
		case eEngineServices::Input_Manager:
			return m_InputManager;
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
				if (m_ResourceManager == nullptr)
					return eEngineMessage::_QFail; // not loaded
				break;
			case eEngineServices::Input_Manager:
				if (m_InputManager == nullptr)
					return eEngineMessage::_QFail; // not loaded
				break;
			}

			i = (eEngineServices)((int)i + 1); // increment service
		}
		return eEngineMessage::_QSuccess;
	}
}