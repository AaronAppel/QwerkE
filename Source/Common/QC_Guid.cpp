#include "QC_Guid.h"

#include <random>

namespace QwerkE {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	GUID::GUID()
		: m_Guid(s_UniformDistribution(s_Engine))
	{ }

	GUID::GUID(uint64_t GUID)
		: m_Guid(GUID)
	{ }

}
