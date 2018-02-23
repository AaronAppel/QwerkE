#ifndef __Barrel_H__
#define __Barrel_H__

// Simplified container for incrementing

class Barrel
{
public:
	Barrel(float limit, float pourValue);
    ~Barrel();

	void Pour(); // increment
	void Empty() { m_CurrentVolume = 0.0f; }; // reset

	bool IsFull() { return m_CurrentVolume >= m_Limit; }

	float GetLimit() { return m_Limit; };
	float GetVolume() { return m_CurrentVolume; };

private:
	float m_Limit = 0.0f; // max volume
	float m_CurrentVolume = 0.0f; // current value
	float m_PourValue = 0.0f; // amount to increments
};

#endif //!__Barrel_H__
