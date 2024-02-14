#pragma once

#include <stdint.h>

#include "QF_Platform.h"

#ifdef _Q32Bit // https://en.cppreference.com/w/cpp/language/types
typedef char s8;
typedef short s16;
typedef long int s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long int u32;
typedef unsigned long long u64;

#elif defined (_Q64Bit)
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#else
#pragma warning "Define platform architecture!"
#endif

static_assert(sizeof(s8)  == 8  / 8, "TODO Error message");
static_assert(sizeof(s16) == 16 / 8, "");
static_assert(sizeof(s32) == 32 / 8, "");
static_assert(sizeof(s64) == 64 / 8, "");

static_assert(sizeof(u8)  == 8  / 8, "");
static_assert(sizeof(u16) == 16 / 8, "");
static_assert(sizeof(u32) == 32 / 8, "");
static_assert(sizeof(u64) == 64 / 8, "");
