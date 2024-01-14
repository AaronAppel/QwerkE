#pragma once

#include "QF_Defines.h"

#include "QF_Log.h"

// https://groups.google.com/g/comp.std.c/c/d-6Mj5Lko_s?pli=1
// https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros
// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
// https://copyprogramming.com/howto/c-preprocessor-va-args-number-of-arguments

#define PP_NARG(...) PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(							\
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,		\
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,	\
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,	\
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,	\
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,	\
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,	\
_61,_62,_63,N,...) N
#define PP_RSEQ_N()							\
63,62,61,60,								\
59,58,57,56,55,54,53,52,51,50,				\
49,48,47,46,45,44,43,42,41,40,				\
39,38,37,36,35,34,33,32,31,30,				\
29,28,27,26,25,24,23,22,21,20,				\
19,18,17,16,15,14,13,12,11,10,				\
9,8,7,6,5,4,3,2,1,0

#if _DEBUG

// void PrintCliackableOutputWindowError(const char* message...) {} // #TODO Create a formatted output message that's clickable in Visual Studio

// #TODO Find a way to support overloading macros by number of arguments
#define FOO2(x,y) ...
#define FOO3(x,y,z) ...
// #define FOO(x,y) FOO2(x,y)
// #define FOO(x,y,z) FOO3(x,y,z)
// #define GET_MACRO(_1,_2,_3,NAME,...) NAME
// #define FOO(...) GET_MACRO(__VA_ARGS__, FOO3, FOO2)(__VA_ARGS__)

// #define EarlyReturnIfNull(retValue, numArgs, ...) if (AnArgumentWasNull(numArgs, __VA_ARGS__)) return retValue;
#define EarlyReturnIfNull2(retValue, a, b) if (AnArgumentWasNull(2, a, b)) return retValue;
#define EarlyReturnIfNull3(retValue, a, b, c) if (AnArgumentWasNull(3, a, b, c)) return retValue;
#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define EarlyReturnIfNull(num, ...) GET_MACRO(__VA_ARGS__, EarlyReturnIfNull3, EarlyReturnIfNull2)(num, __VA_ARGS__)

// const char* name = nullptr;
// const char* material = nullptr;
// EarlyReturnIfNull(eEngineMessage::_QFailure, name, material);

bool AnArgumentWasNull(int n_args, ...); // #TODO Implement an early return with logging null check helper

// #TODO Make these globally accessible at all times, even without logging
#define BREAK __debugbreak();
#define ASSERT(x, msg)   if ((x)) { } else { LOG_CRITICAL("Assert! {0}, {1}, in {2}() in {3}({4})", #x, msg, __FUNCTION__, __FILE__, __LINE__); BREAK }
// #TODO Add asserting : https://www.youtube.com/watch?v=DQCkMnMNFBI&ab_channel=Progrematic

#else
#define BREAK __debugbreak();
#define ASSERT(x, msg)   if ((x)) { } else { LOG_CRITICAL("Assert! {0}, {1}, in {2}() in {3}({4})", #x, msg, __FUNCTION__, __FILE__, __LINE__); BREAK }
#endif
