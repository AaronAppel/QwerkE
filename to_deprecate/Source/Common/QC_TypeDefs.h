#pragma once

#include <stdint.h>

#include "QF_Platform.h"

#if 1
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#elif _Q32Bit // https://en.cppreference.com/w/cpp/language/types
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

static_assert(sizeof(s8)  == 8  / 8, "s8 type size mismatch!");
static_assert(sizeof(s16) == 16 / 8, "s16 type size mismatch!");
static_assert(sizeof(s32) == 32 / 8, "s32 type size mismatch!");
static_assert(sizeof(s64) == 64 / 8, "s64 type size mismatch!");

static_assert(sizeof(u8)  == 8  / 8, "u8 type size mismatch!");
static_assert(sizeof(u16) == 16 / 8, "u16 type size mismatch!");
static_assert(sizeof(u32) == 32 / 8, "u32 type size mismatch!");
static_assert(sizeof(u64) == 64 / 8, "u64 type size mismatch!");
