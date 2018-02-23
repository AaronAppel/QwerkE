#include "stdafx.h"

Barrel::Barrel(float limit, float pourValue) :
	m_Limit(limit),
	m_PourValue(pourValue)
{
}

Barrel::~Barrel()
{
}

void Barrel::Pour()
{
	if (m_CurrentVolume < m_Limit)
	{
		m_CurrentVolume += m_PourValue;
	}
}