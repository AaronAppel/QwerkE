#include "EventManager.h"
#include "Utilities/PrintFunctions.h"
#include "Utilities/Helpers.h"

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