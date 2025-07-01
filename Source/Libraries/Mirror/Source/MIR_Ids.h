#pragma once

#include "MIR_Structs.h"

// Void
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 0, void)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 1, void*)

// Mutable primitives
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 2, signed char)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 3, signed short)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 4, signed int)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 5, signed long long)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 6, unsigned char)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 7, unsigned short)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 8, unsigned int)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 9, unsigned long long)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 10, float)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 11, double)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 12, long double)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 13, char)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 14, bool)

// Const primitives
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 15, const signed char)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 16, const signed short)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 17, const signed int)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 18, const signed long long)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 19, const unsigned char)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 20, const unsigned short)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 21, const unsigned int)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 22, const unsigned long long)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 23, const float)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 24, const double)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 25, const long double)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 26, const char)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 27, const bool)

// Mutable pointers
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 28, signed char*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 29, signed short*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 30, signed int*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 31, signed long long*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 32, unsigned char*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 33, unsigned short*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 34, unsigned int*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 35, unsigned long long*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 36, float*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 37, double*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 38, long double*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 39, char*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 40, bool*)

// Const pointers
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 41, const signed char*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 42, const signed short*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 43, const signed int*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 44, const signed long long*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 45, const unsigned char*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 46, const unsigned short*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 47, const unsigned int*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 48, const unsigned long long*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 49, const float*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 50, const double*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 51, const long double*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 52, const char*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 53, const bool*)

// std::string
#include <string>
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 54, std::string)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 55, std::string*)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 56, const std::string)
MIRROR_TYPE_ID(MIRROR_TYPE_ID_MAX - 57, const std::string*)

#define MIRROR_USER_TYPE_ID_MAX MIRROR_TYPE_ID_MAX - 58
