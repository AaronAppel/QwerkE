#pragma once

// Sourced from : UUID.h -> https://github.com/TheCherno/Hazel/tree/master/Hazel/src/Hazel/Core

namespace QwerkE {

	class GUID
	{
	public:
		GUID();
		GUID(u64 GUID);
		GUID(const GUID&) = default;

		operator u64() const { return m_Guid; }

		static const u64 Invalid = 0;

	private:
		u64 m_Guid;
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
