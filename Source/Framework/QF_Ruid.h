#pragma once

namespace QwerkE {

	class RUID
	{
	public:
		RUID() : m_Ruid(s_NextRuid++) { }

		static constexpr unsigned long long Invalid = 0;
		static constexpr unsigned long long Max = 0xffffffffffffffffui64; // UINT64_MAX

		operator unsigned long long() const { return m_Ruid; }

		bool operator ==(const RUID& other) const { return m_Ruid == other.m_Ruid; }
		// #NOTE Spdlog doesn't like this : operator bool() const { return m_Ruid != Invalid; }

	private:
		unsigned long long m_Ruid = Invalid;
		inline static unsigned long long s_NextRuid = Invalid + 1;
	};

}
