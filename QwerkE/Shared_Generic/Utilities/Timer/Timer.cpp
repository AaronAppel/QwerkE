#include "stdafx.h"

Timer::Timer()
{
}
Timer::~Timer()
{
}

void Timer::Reset(double a_TimeLength)
{
	m_ElapsedTime = 0.0f;
	m_TimeLength = (float)a_TimeLength;
	m_HasCompleted = false;
}

bool Timer::Tick(double a_Deltatime)
{
	if (m_HasCompleted == false)
	{
		m_ElapsedTime += (float)a_Deltatime;
		CheckIfCompleted();
	}
	return m_HasCompleted;
}
// private functions
void Timer::CheckIfCompleted()
{
	if (m_ElapsedTime >= m_TimeLength)
	{
		m_HasCompleted = true;
		// m_CallBack->Trigger();
	}
	else
		m_HasCompleted = false;
}