#include "PrintComponent.h"
#include "../Shared_Generic/Utilities/PrintFunctions.h"

PrintComponent::PrintComponent()
{
}

PrintComponent::~PrintComponent()
{
}

void PrintComponent::PrintMessage()
{
    ConsolePrint(m_Message);
}