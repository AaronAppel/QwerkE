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
		InputStatesBitRingBuffer() // #TODO Merge with BitIndexRingBuffer
		{
			m_KeysBuffer.AddMarker(0);
			m_KeysBuffer.AddMarker(0);
		}

		void Advance()
		{
			m_KeysBuffer.SetMarkerPosition(1, m_KeysBuffer.MarkerPosition(0));
			m_KeysBuffer.SetMarkerPosition(0, m_KeysBuffer.HeadIndex());
		}

		void Write(const QKey a_Key, const QKeyState a_KeyState)
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

		QKey ReadKey(unsigned char a_Index)
		{
			return m_KeysBuffer.ReadRandom(a_Index);
		}

		bool ReadKeyState(unsigned char a_Index)
		{
			assert(a_Index < Bits::SIZE);
			return m_KeysStates[a_Index];
		}

		bool KeyThisFrame(const QKey a_Key, const QKeyState a_KeyState)
		{
			if (m_KeysBuffer.HeadIndex() == m_KeysBuffer.MarkerPosition(0))
			{
				return false;
			}
			else if (QKey::e_Any == a_Key && QKeyState::e_KeyStateAny == a_KeyState)
			{
				return true;
			}

			unsigned char thisFrame = InputThisFrame();
			Bits index = m_KeysBuffer.HeadIndex() - 1;
			while (thisFrame > 0)
			{
				if (QKey::e_Any == a_Key || a_Key == m_KeysBuffer.ReadRandom(index.value))
				{
					if (QKeyState::e_KeyStateAny == a_KeyState || a_KeyState == m_KeysStates[index.value])
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
			return m_KeysBuffer.MarkerPosition(0);
		}

		unsigned char LastFrameStart()
		{
			return m_KeysBuffer.MarkerPosition(1);
		}

		unsigned char InputThisFrame()
		{
			return WritesUntilCollision(m_KeysBuffer.HeadIndex(), m_KeysBuffer.MarkerPosition(0));
		}

		unsigned char InputLastFrame()
		{
			return WritesUntilCollision(m_KeysBuffer.MarkerPosition(0), m_KeysBuffer.MarkerPosition(1));
		}

		unsigned char DownKeys()
		{
			return m_DownCount;
		}

		constexpr unsigned char Size()
		{
			return Bits::SIZE;
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
		BitIndexRingBuffer<QKey, Bits> m_KeysBuffer;
		std::bitset<Bits::SIZE> m_KeysStates;
	};

}
