#pragma once

void* operator new (size_t sizeBytes);
void operator delete (void* memoryLocation);

#include <memory>

namespace QwerkE {

	template<typename T>
	using sRef = std::shared_ptr<T>;

	template<typename T>
	using uRef = std::unique_ptr<T>;

	template<typename T>
	using wRef = std::weak_ptr<T>;

	// template<typename T>
	// using makeURef = std::make_unique<T>();
}