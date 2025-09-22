#pragma once

#include "MIR_Structs.h"

// Void
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 0, void)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 1, void*)

// Mutable primitives
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 2, signed char)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 3, signed short)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 4, signed int)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 5, signed long long)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 6, unsigned char)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 7, unsigned short)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 8, unsigned int)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 9, unsigned long long)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 10, float)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 11, double)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 12, long double)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 13, char)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 14, bool)

// Const primitives
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 15, const signed char)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 16, const signed short)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 17, const signed int)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 18, const signed long long)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 19, const unsigned char)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 20, const unsigned short)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 21, const unsigned int)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 22, const unsigned long long)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 23, const float)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 24, const double)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 25, const long double)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 26, const char)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 27, const bool)

// Mutable pointers
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 28, signed char*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 29, signed short*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 30, signed int*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 31, signed long long*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 32, unsigned char*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 33, unsigned short*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 34, unsigned int*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 35, unsigned long long*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 36, float*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 37, double*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 38, long double*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 39, char*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 40, bool*)

// Const pointers
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 41, const signed char*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 42, const signed short*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 43, const signed int*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 44, const signed long long*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 45, const unsigned char*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 46, const unsigned short*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 47, const unsigned int*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 48, const unsigned long long*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 49, const float*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 50, const double*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 51, const long double*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 52, const char*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 53, const bool*)

// std::string
#include <string>
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 54, std::string)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 55, std::string*)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 56, const std::string)
MIR_TYPE_ID(MIR_TYPE_ID_MAX - 57, const std::string*)

#define MIRROR_USER_TYPE_ID_MAX MIR_TYPE_ID_MAX - 58
