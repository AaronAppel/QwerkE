#pragma once

// Sourced from : UUID.h -> https://github.com/TheCherno/Hazel/tree/master/Hazel/src/Hazel/Core

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

namespace QwerkE {

	class GUID
	{
	public:
		GUID();
		GUID(u64 GUID);
		GUID(const GUID&) = default;

		operator u64() const { return m_Guid; }

		static const u64 Invalid = 0;

		bool operator ==(const GUID& other) const { return m_Guid == other.m_Guid; }
		// #NOTE Spdlog doesn't like this : operator bool() const { return m_Guid != Invalid; }

	private:
		MIRROR_PRIVATE_MEMBERS
		u64 m_Guid = Invalid;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<QwerkE::GUID>
	{
		std::size_t operator()(const QwerkE::GUID& guid) const
		{
			return (u64)guid;
		}
	};

}
