#pragma once

#include <memory>

namespace QwerkE
{
	template<typename T>
	using sRef = std::unique_ptr<T>;

	template<typename T>
	using uRef = std::shared_ptr<T>;
}