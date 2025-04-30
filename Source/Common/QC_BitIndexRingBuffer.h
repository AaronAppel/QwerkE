#pragma once

#include <array>
#include <assert.h>
#include <vector>

template <typename T, typename Bits>
class BitIndexRingBuffer
{
public:
	BitIndexRingBuffer() = default;

	void Write(T a_Value)
	{
		m_Values[m_Head.bits] = a_Value;
		m_Head.bits += 1;
	}

	T ReadRandom(const unsigned char a_Index)
	{
		assert(m_Values.size() > a_Index);
		return m_Values[a_Index];
	}

	T ReadMarker(const unsigned char a_Index)
	{
		assert(m_Markers.size() > a_Index);
		return m_Values[m_Markers[a_Index].bits];
	}

	const char AddMarker(const unsigned char a_StartingIndex)
	{
		m_Markers.push_back(a_StartingIndex);
		return m_Markers.size() - 1;
	}

	const unsigned char MarkerPosition(const unsigned char a_MarkerIndex)
	{
		assert(m_Markers.size() > a_MarkerIndex);
		return m_Markers[a_MarkerIndex];
	}

	void AdvanceMarker(const unsigned char a_MarkerIndex)
	{
		assert(m_Markers.size() > a_MarkerIndex);
		m_Markers[a_MarkerIndex] = m_Head;
	}

	void RemoveMarker(const unsigned char a_MarkerIndex)
	{
		assert(m_Markers.size() > a_MarkerIndex);
		m_Markers.removeat(a_MarkerIndex);
	}

	unsigned char WritesUntilNextMarkerCollision()
	{
		return 255; // #TODO Implement
	}

private:
	Bits m_Head = 0;
	std::array<T, Bits::max + 1> m_Values;
	std::vector<Bits> m_Markers;
	// #TODO Could simplify by making this class specific. Use 3 pointers and always advance [1] and [2] to match [0]
	// Rename to something like TwoStateRingBuffer, or TwoWindowRingBuffer, etc.
};
