#pragma once

#include <array>
#include <assert.h>
#include <vector>

template <typename T, typename Bits>
class BitIndexRingBuffer
{
public:
	void Write(T a_Value)
	{
		m_Values[m_Head.value] = a_Value;
		m_Head.value += 1;
	}

	T ReadTop()
	{
		Bits topIndex = m_Head.value - 1;
		return m_Values[topIndex.value];
	}

	T ReadRandom(const unsigned char a_Index)
	{
		assert(m_Values.size() > a_Index);
		return m_Values[a_Index];
	}

	T ReadMarker(const unsigned char a_Index)
	{
		assert(m_Markers.size() > a_Index);
		return m_Values[m_Markers[a_Index].value];
	}

	char AddMarker(const unsigned char a_StartingIndex)
	{
		m_Markers.push_back(a_StartingIndex);
		return m_Markers.size() - 1;
	}

	unsigned char HeadIndex()
	{
		return m_Head.value;
	}

	unsigned char MarkerPosition(const unsigned char a_MarkerIndex)
	{
		assert(m_Markers.size() > a_MarkerIndex);
		return m_Markers[a_MarkerIndex].value;
	}

	void SetMarkerPosition(const unsigned char a_MarkerIndex, const unsigned char a_MarkerPosition)
	{
		assert(m_Markers.size() > a_MarkerIndex);
		m_Markers[a_MarkerIndex] = a_MarkerPosition;
	}

	void AdvanceAllMarkers()
	{
		for (size_t i = 0; i < m_Markers.size(); i++)
		{
			m_Markers[i] = m_Head;
		}
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

	constexpr unsigned char Size()
	{
		return Bits::SIZE;
	}


private:
	Bits m_Head = 0;
	std::array<T, Bits::SIZE> m_Values;
	std::vector<Bits> m_Markers;
	// #TODO Could simplify by making this class specific. Use 3 pointers and always advance [1] and [2] to match [0]
	// Rename to something like TwoStateRingBuffer, or TwoWindowRingBuffer, etc.
};
