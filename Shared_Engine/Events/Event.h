#ifndef _Event_H_
#define _Event_H_

#include <cstdint>
enum class eEventTypes : std::uint8_t
{
	eEvent_Invalid = 0,
	eEvent_System_Message,
	eEvent_Max
};

class Event // abstract
{
public:
	Event();
	~Event();

	void SetID(int id);
	int GetID();
		
	eEventTypes GetType();

	virtual void Execute() = 0;

private:
	signed int m_EventID = -1;
	eEventTypes m_EventType = eEventTypes::eEvent_Invalid;
};

#endif // !_Event_H_
