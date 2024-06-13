#pragma once

#include "Structs.h"

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
// #TODO Look to forward declare std::string instead of including header file
// namespace std { class basic_string; using string = basic_string<char, char_traits<char>, allocator<char>>; }
MIRROR_TYPE_ID(std::string)
MIRROR_TYPE_ID(std::string*)
MIRROR_TYPE_ID(const std::string)
MIRROR_TYPE_ID(const std::string*)
