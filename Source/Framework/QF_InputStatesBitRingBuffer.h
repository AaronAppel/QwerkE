#pragma once

#include <assert.h>
#include <bitset>

#include "QC_BitIndexRingBuffer.h"

#include "QF_QKey.h"

namespace QwerkE {

	template <typename Bits>
	class InputStatesBitRingBuffer
	{
	public:
		void Write(const QKey a_Key, const bool a_KeyState)
		{
			m_KeysStates[m_KeysBuffer.HeadIndex()] = a_KeyState;
			m_KeysBuffer.Write(a_Key);
		}

		QKey ReadKey(unsigned char a_Index)
		{
			return m_KeysBuffer.ReadRandom(a_Index);
		}

		bool ReadKeyState(unsigned char a_Index)
		{
			assert(a_Index <= Bits::MAX);
			return m_KeysStates[a_Index];
		}

		constexpr unsigned char Size()
		{
			return Bits::MAX;
		}

	private:
		BitIndexRingBuffer<QKey, Bits> m_KeysBuffer;
		std::bitset<Bits::MAX + 1> m_KeysStates;
		// bool m_KeysStates[Bits::max > 8 ? Bits::max / 8 : 1];
	};

}
