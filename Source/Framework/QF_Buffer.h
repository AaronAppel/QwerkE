#pragma once

namespace QwerkE {

	class Buffer final
	{
	public:

		Buffer() = default;

		Buffer(const Buffer& other)
		{
			if (this == &other)
				return;

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
				m_OwnershipTransferred = false;
			}
		}

		void Fill(const char byteToWrite)
		{
			if (m_SizeBytes > 0)
			{
				memset((char*)m_Data, byteToWrite, m_SizeBytes);
			}
		}

		void Write(Buffer other)
		{
			u64 bytesToWrite = m_SizeBytes;
			if (other.m_SizeBytes < bytesToWrite)
			{
				bytesToWrite = other.m_SizeBytes;
			}
			memcpy((char*)m_Data, other.Data(), bytesToWrite);
		}

		u8* Data() { return m_Data; }
		u64 SizeInBytes() { return m_SizeBytes; }

		template <typename T>
		T* As()
		{
			// #TODO Expirment with: return static_cast<T*>(m_Data);
			return (T*)m_Data;
		}

		Buffer& operator=(const Buffer& other)
		{
			if (this == &other)
				return *this;

			Release();

			this->m_Data = other.m_Data;
			this->m_SizeBytes = other.m_SizeBytes;

			// #TODO Investigate proper way to copy buffer references, or smart pointer usage
			other.m_OwnershipTransferred = true;
			this->m_OwnershipTransferred = false;

			return *this;
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
