#pragma once

#include "MIR_Structs.h"

#if defined(MIRROR_NONCONFORMING) && defined(MIRROR_GENERATE_TYPE_IDS)

// Void
MIRROR_TYPE_ID(void)
MIRROR_TYPE_ID(void*)

// Mutable
MIRROR_TYPE_ID(signed char)
MIRROR_TYPE_ID(signed short)
MIRROR_TYPE_ID(signed int)
MIRROR_TYPE_ID(signed long long)
MIRROR_TYPE_ID(unsigned char)
MIRROR_TYPE_ID(unsigned short)
MIRROR_TYPE_ID(unsigned int)
MIRROR_TYPE_ID(unsigned long long)
MIRROR_TYPE_ID(float)
MIRROR_TYPE_ID(double)
MIRROR_TYPE_ID(long double)
MIRROR_TYPE_ID(char)
MIRROR_TYPE_ID(bool)

// Const
MIRROR_TYPE_ID(const signed char)
MIRROR_TYPE_ID(const signed short)
MIRROR_TYPE_ID(const signed int)
MIRROR_TYPE_ID(const signed long long)
MIRROR_TYPE_ID(const unsigned char)
MIRROR_TYPE_ID(const unsigned short)
MIRROR_TYPE_ID(const unsigned int)
MIRROR_TYPE_ID(const unsigned long long)
MIRROR_TYPE_ID(const float)
MIRROR_TYPE_ID(const double)
MIRROR_TYPE_ID(const long double)
MIRROR_TYPE_ID(const char)
MIRROR_TYPE_ID(const bool)

// Mutable pointers
MIRROR_TYPE_ID(signed char*)
MIRROR_TYPE_ID(signed short*)
MIRROR_TYPE_ID(signed int*)
MIRROR_TYPE_ID(signed long long*)
MIRROR_TYPE_ID(unsigned char*)
MIRROR_TYPE_ID(unsigned short*)
MIRROR_TYPE_ID(unsigned int*)
MIRROR_TYPE_ID(unsigned long long*)
MIRROR_TYPE_ID(float*)
MIRROR_TYPE_ID(double*)
MIRROR_TYPE_ID(long double*)
MIRROR_TYPE_ID(char*)
MIRROR_TYPE_ID(bool*)

// Const pointers
MIRROR_TYPE_ID(const signed char*)
MIRROR_TYPE_ID(const signed short*)
MIRROR_TYPE_ID(const signed int*)
MIRROR_TYPE_ID(const signed long long*)
MIRROR_TYPE_ID(const unsigned char*)
MIRROR_TYPE_ID(const unsigned short*)
MIRROR_TYPE_ID(const unsigned int*)
MIRROR_TYPE_ID(const unsigned long long*)
MIRROR_TYPE_ID(const float*)
MIRROR_TYPE_ID(const double*)
MIRROR_TYPE_ID(const long double*)
MIRROR_TYPE_ID(const char*)
MIRROR_TYPE_ID(const bool*)

// String
#include <string>
MIRROR_TYPE_ID(std::string)
MIRROR_TYPE_ID(std::string*)
MIRROR_TYPE_ID(const std::string)
MIRROR_TYPE_ID(const std::string*)

#else

MIRROR_TYPE_ID(0, void) // Void

// Mutable
MIRROR_TYPE_ID(1, signed char)
MIRROR_TYPE_ID(2, signed short)
MIRROR_TYPE_ID(3, signed int)
MIRROR_TYPE_ID(4, signed long long)
MIRROR_TYPE_ID(5, unsigned char)
MIRROR_TYPE_ID(6, unsigned short)
MIRROR_TYPE_ID(7, unsigned int)
MIRROR_TYPE_ID(8, unsigned long long)
MIRROR_TYPE_ID(9, float)
MIRROR_TYPE_ID(10, double)
MIRROR_TYPE_ID(11, long double)
MIRROR_TYPE_ID(12, char)
MIRROR_TYPE_ID(13, bool)

// Const
MIRROR_TYPE_ID(14, const signed char)
MIRROR_TYPE_ID(15, const signed short)
MIRROR_TYPE_ID(16, const signed int)
MIRROR_TYPE_ID(17, const signed long long)
MIRROR_TYPE_ID(18, const unsigned char)
MIRROR_TYPE_ID(19, const unsigned short)
MIRROR_TYPE_ID(20, const unsigned int)
MIRROR_TYPE_ID(21, const unsigned long long)
MIRROR_TYPE_ID(22, const float)
MIRROR_TYPE_ID(23, const double)
MIRROR_TYPE_ID(24, const long double)
MIRROR_TYPE_ID(25, const char)
MIRROR_TYPE_ID(26, const bool)

MIRROR_TYPE_ID(27, void*) // Void pointer

// Mutable pointers
MIRROR_TYPE_ID(28, signed char*)
MIRROR_TYPE_ID(29, signed short*)
MIRROR_TYPE_ID(30, signed int*)
MIRROR_TYPE_ID(31, signed long long*)
MIRROR_TYPE_ID(32, unsigned char*)
MIRROR_TYPE_ID(33, unsigned short*)
MIRROR_TYPE_ID(34, unsigned int*)
MIRROR_TYPE_ID(35, unsigned long long*)
MIRROR_TYPE_ID(36, float*)
MIRROR_TYPE_ID(37, double*)
MIRROR_TYPE_ID(38, long double*)
MIRROR_TYPE_ID(39, char*)
MIRROR_TYPE_ID(40, bool*)

// Const pointers
MIRROR_TYPE_ID(41, const signed char*)
MIRROR_TYPE_ID(42, const signed short*)
MIRROR_TYPE_ID(43, const signed int*)
MIRROR_TYPE_ID(44, const signed long long*)
MIRROR_TYPE_ID(45, const unsigned char*)
MIRROR_TYPE_ID(46, const unsigned short*)
MIRROR_TYPE_ID(47, const unsigned int*)
MIRROR_TYPE_ID(48, const unsigned long long*)
MIRROR_TYPE_ID(49, const float*)
MIRROR_TYPE_ID(50, const double*)
MIRROR_TYPE_ID(51, const long double*)
MIRROR_TYPE_ID(52, const char*)
MIRROR_TYPE_ID(53, const bool*)

// String
#include <string>
MIRROR_TYPE_ID(54, std::string)
MIRROR_TYPE_ID(55, std::string*)
MIRROR_TYPE_ID(56, const std::string)
MIRROR_TYPE_ID(57, const std::string*)

// #TODO Choose how users increment from this value
#define MIRROR_USER_TYPE_ID_START (uint8_t)58

#endif