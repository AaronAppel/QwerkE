#pragma once

// Sourced from : UUID.h -> https://github.com/TheCherno/Hazel/tree/master/Hazel/src/Hazel/Core

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

namespace QwerkE {

	class GUID
	{
	public:
		GUID();
		GUID(unsigned long long GUID);
		GUID(const GUID&) = default;

		operator unsigned long long() const { return m_Guid; }

		static const unsigned long long Invalid = 0;

		bool operator ==(const GUID& other) const { return m_Guid == other.m_Guid; }
		// #NOTE Spdlog doesn't like this : operator bool() const { return m_Guid != Invalid; }

	private:
		MIRROR_PRIVATE_MEMBERS
		unsigned long long m_Guid = Invalid;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<QwerkE::GUID>
	{
		unsigned long long operator()(const QwerkE::GUID& guid) const
		{
			return (unsigned long long)guid;
		}
	};

}
