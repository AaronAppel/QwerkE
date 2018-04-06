#include "EventManager.h"
#include "../Shared_Generic/Utilities/PrintFunctions.h"
#include "../Shared_Generic/Utilities/Helpers.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::QueueEvent()
{
    OutputPrint("Event Queued %i", helpers_GetUniqueID());
}