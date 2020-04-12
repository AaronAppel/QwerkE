#include "PrintComponent.h"
#include "../../../QwerkE_Framework/Source/Debug/Log/Log.h"

PrintComponent::PrintComponent()
{
}

PrintComponent::~PrintComponent()
{
}

void PrintComponent::PrintMessage()
{
    LOG_INFO(m_Message);
}