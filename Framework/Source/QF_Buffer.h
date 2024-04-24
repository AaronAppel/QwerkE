#pragma once

namespace QwerkE {

	class Buffer final
	{
	public:

		Buffer() = default;
		Buffer(const Buffer& other)
		{
			this->m_Data = other.m_Data;
			this->m_SizeBytes = other.m_SizeBytes;

			// #TODO Investigate proper way to copy buffer references, or smart pointer usage
			other.m_OwnershipTransferred = true;
			this->m_OwnershipTransferred = false;
		}

		Buffer(u64 bytes)
		{
			Allocate(bytes);
		}

		virtual ~Buffer()
		{
			if (!m_OwnershipTransferred)
			{
				Release();
			}
		}

		void Allocate(u64 bytes)
		{
			m_Data = new u8[bytes];
			m_SizeBytes = bytes;
		}

		void Release()
		{
			if (m_SizeBytes > 0)
			{
				delete[] m_Data;
				m_SizeBytes = 0;
			}
		}

		u8* Data() { return m_Data; }
		u64 SizeInBytes() { return m_SizeBytes; }

		template <typename T>
		T* As()
		{
			// return static_cast<T*>(m_Data);
			return (T*)m_Data;
		}

		operator bool() const
		{
			return m_Data != nullptr;
		}

	protected:
		u8* m_Data = nullptr;
		u64 m_SizeBytes = 0;

		mutable bool m_OwnershipTransferred = false;
	};

}
