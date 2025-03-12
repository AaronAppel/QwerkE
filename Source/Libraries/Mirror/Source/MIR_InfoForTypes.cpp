#include "MIR_Mirror.h"

// Void
MIRROR_TYPE_VOID(void)
MIRROR_TYPE(void*)

// Mutable
MIRROR_TYPE(signed char)
MIRROR_TYPE(signed short)
MIRROR_TYPE(signed int)
MIRROR_TYPE(signed long long)
MIRROR_TYPE(unsigned char)
MIRROR_TYPE(unsigned short)
MIRROR_TYPE(unsigned int)
MIRROR_TYPE(unsigned long long)
MIRROR_TYPE(float)
MIRROR_TYPE(double)
MIRROR_TYPE(long double)
MIRROR_TYPE(char)
MIRROR_TYPE(bool)

// Const
MIRROR_TYPE(const signed char)
MIRROR_TYPE(const signed short)
MIRROR_TYPE(const signed int)
MIRROR_TYPE(const signed long long)
MIRROR_TYPE(const unsigned char)
MIRROR_TYPE(const unsigned short)
MIRROR_TYPE(const unsigned int)
MIRROR_TYPE(const unsigned long long)
MIRROR_TYPE(const float)
MIRROR_TYPE(const double)
MIRROR_TYPE(const long double)
MIRROR_TYPE(const char)
MIRROR_TYPE(const bool)

// Mutable pointers
MIRROR_TYPE(signed char*)
MIRROR_TYPE(signed short*)
MIRROR_TYPE(signed int*)
MIRROR_TYPE(signed long long*)
MIRROR_TYPE(unsigned char*)
MIRROR_TYPE(unsigned short*)
MIRROR_TYPE(unsigned int*)
MIRROR_TYPE(unsigned long long*)
MIRROR_TYPE(float*)
MIRROR_TYPE(double*)
MIRROR_TYPE(long double*)
MIRROR_TYPE(char*)
MIRROR_TYPE(bool*)

// Const pointers
MIRROR_TYPE(const signed char*)
MIRROR_TYPE(const signed short*)
MIRROR_TYPE(const signed int*)
MIRROR_TYPE(const signed long long*)
MIRROR_TYPE(const unsigned char*)
MIRROR_TYPE(const unsigned short*)
MIRROR_TYPE(const unsigned int*)
MIRROR_TYPE(const unsigned long long*)
MIRROR_TYPE(const float*)
MIRROR_TYPE(const double*)
MIRROR_TYPE(const long double*)
MIRROR_TYPE(const char*)
MIRROR_TYPE(const bool*)

// String
#include <string>
MIRROR_TYPE(std::string)
MIRROR_TYPE(std::string*)
MIRROR_TYPE(const std::string)
MIRROR_TYPE(const std::string*)
