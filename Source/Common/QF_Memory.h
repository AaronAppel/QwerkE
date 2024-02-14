#pragma once

#include "QF_Defines.h"
#if QWERKE_TRACKING_ALLOCATIONS && 0 // Using "Libraries/FlatheadGames/MyMemory.h" instead
void* operator new (size_t sizeBytes);
void operator delete (void* memoryLocation);
#endif

#include <memory>

namespace QwerkE {

	template<typename T>
	using sPtr = std::shared_ptr<T>;

	template<typename T>
	using uPtr = std::unique_ptr<T>;

	template<typename T>
	using wPtr = std::weak_ptr<T>;

	// template<typename T>
	// using makeURef = std::make_unique<T>();

	class qPtr
	{
	public:

	private:

	};

}