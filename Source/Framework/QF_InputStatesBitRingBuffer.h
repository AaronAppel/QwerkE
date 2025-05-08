#pragma once

#include <assert.h>
#include <bitset>

#include "QC_BitIndexRingBuffer.h"

#include "QF_QKey.h"

namespace QwerkE {

	template <typename T, typename Bits>
	class InputStatesBitRingBuffer
	{
	public:
		InputStatesBitRingBuffer() // #TODO Merge with BitIndexRingBuffer
		{
			m_KeysBuffer.AddMarker(0);
			m_KeysBuffer.AddMarker(0);
		}

		void Advance()
		{
			m_KeysBuffer.SetMarkerPosition(m_LastFrameStartMarkerIndex, m_KeysBuffer.MarkerPosition(m_LastFrameEndMarkerIndex));
			m_KeysBuffer.SetMarkerPosition(m_LastFrameEndMarkerIndex, m_KeysBuffer.HeadIndex());
		}

		void Write(const T a_Key, const QKeyState a_KeyState)
		{
			unsigned char thisFrame = InputThisFrame();
			unsigned char lastframe = InputLastFrame();
			unsigned char writesRemaining = Bits::SIZE - thisFrame - lastframe;

			ASSERT(0 < writesRemaining, "Collision detected!");
			m_KeysStates[m_KeysBuffer.HeadIndex()] = a_KeyState;
			m_KeysBuffer.Write(a_Key);

			if (QKeyState::e_KeyStateDown == a_KeyState)
			{
				++m_DownCount;
			}
			else
			{
				--m_DownCount;
			}
		}

		T ReadKey(unsigned char a_Index)
		{
			return m_KeysBuffer.ReadRandom(a_Index);
		}

		bool ReadKeyState(unsigned char a_Index)
		{
			assert(a_Index < Bits::SIZE);
			return m_KeysStates[a_Index];
		}

		bool KeyThisFrame(const T a_Key, const QKeyState a_KeyState, const bool a_Any = false)
		{
			if (m_KeysBuffer.HeadIndex() == m_KeysBuffer.MarkerPosition(m_LastFrameEndMarkerIndex))
			{
				return false;
			}

			unsigned char thisFrame = InputThisFrame();
			Bits index = m_KeysBuffer.HeadIndex() - 1;

			// #TODO Read left to write? Would that be more CPU cache efficient?
			// Bits index = m_KeysBuffer.MarkerPosition(0);
			// ++index.value;
			// ++thisFrame;

			while (thisFrame > 0)
			{
				const T currentIndexAsEnum = m_KeysBuffer.ReadRandom(index.value);
				if (a_Any || currentIndexAsEnum == a_Key)
				{
					if (a_KeyState == m_KeysStates[index.value])
					{
						return true;
					}
				}
				--index.value;
				--thisFrame;
			}
			return false;
		}

		unsigned char Head()
		{
			return m_KeysBuffer.HeadIndex();
		}

		unsigned char LastFrameEnd()
		{
			return m_KeysBuffer.MarkerPosition(m_LastFrameEndMarkerIndex);
		}

		unsigned char LastFrameStart()
		{
			return m_KeysBuffer.MarkerPosition(1);
		}

		unsigned char InputThisFrame()
		{
			return WritesUntilCollision(m_KeysBuffer.HeadIndex(), m_KeysBuffer.MarkerPosition(m_LastFrameEndMarkerIndex));
		}

		unsigned char InputLastFrame()
		{
			return WritesUntilCollision(m_KeysBuffer.MarkerPosition(m_LastFrameEndMarkerIndex), m_KeysBuffer.MarkerPosition(1));
		}

		unsigned char DownKeys()
		{
			return m_DownCount;
		}

		constexpr unsigned char Size()
		{
			return Bits::SIZE;
		}

		void Clear()
		{
			m_KeysBuffer.Reset();
			m_KeysStates.reset();
		}

		InputStatesBitRingBuffer& operator=(const InputStatesBitRingBuffer& a_Other)
		{
			m_DownCount = a_Other.m_DownCount;
			m_KeysBuffer = a_Other.m_KeysBuffer;
			m_KeysStates = a_Other.m_KeysStates;
			return *this;
		}

	private:
		unsigned char WritesUntilCollision(const unsigned char a_LeadingIndex, const unsigned char a_TrailingIndex)
		{
			if (a_LeadingIndex >= a_TrailingIndex)
			{
				return a_LeadingIndex - a_TrailingIndex;
			}
			return Bits::SIZE - (a_TrailingIndex - a_LeadingIndex);
		}

		unsigned char m_DownCount = 0;
		const unsigned char m_LastFrameEndMarkerIndex = 0;
		const unsigned char m_LastFrameStartMarkerIndex = 1; // #TODO Review need for last frame start index. Could simplify collection without it. Depends on future feature needs

		BitIndexRingBuffer<T, Bits> m_KeysBuffer;
		std::bitset<Bits::SIZE> m_KeysStates;
	};

}
