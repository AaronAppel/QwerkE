#pragma once

// Primitive types
// Integers
#include <stdint.h>         // Comments for mouse over IDE context info
typedef int8_t s8;          // -128 to 127
#define S8_MIN INT8_MIN     // -128
#define S8_MAX INT8_MAX     //  127
#define S8_MAX_DIGITS 3

typedef int16_t s16;        // -32,768 to 32,767
#define S16_MIN INT16_MIN   // -32,768
#define S16_MAX INT16_MAX   //  32,767
#define S16_MAX_DIGITS 5

typedef int32_t s32;        // -2,147,483,648 to 2,147,483,647
#define S32_MIN INT32_MIN   // -2,147,483,648
#define S32_MAX INT32_MAX   //  2,147,483,647
#define S32_MAX_DIGITS 10

typedef int64_t s64;        // -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
#define S64_MIN INT64_MIN   // -9,223,372,036,854,775,808
#define S64_MAX INT64_MAX   //  9,223,372,036,854,775,807
#define S64_MAX_DIGITS 19

typedef uint8_t u8;         //  0 to 255
#define U8_MIN 0            //  0
#define U8_MAX UINT8_MAX    //  255
#define U8_MAX_DIGITS 3

typedef uint16_t u16;       //  0 to 65,535
#define U16_MIN 0           //  0
#define U16_MAX UINT16_MAX  //  65,535
#define U16_MAX_DIGITS 5

typedef uint32_t u32;       //  0 to 4,294,967,295
#define U32_MIN 0           //  0
#define U32_MAX UINT32_MAX  //  4,294,967,295
#define U32_MAX_DIGITS 10

typedef uint64_t u64;       //  0 to 18,446,744,073,709,551,615
#define U64_MIN 0           //  0
#define U64_MAX UINT64_MAX  //  18,446,744,073,709,551,615
#define U64_MAX_DIGITS 20

#include "QC_BitTypes.h"    // #TODO Comments for mouse over IDE context info
using u1 = ubits1;          // size of 1 byte, value range of 0 to 1
using u2 = ubits2;
using u3 = ubits3;
using u4 = ubits4;
using u5 = ubits5;
using u6 = ubits6;
using u7 = ubits7;
using u9 = ubits9;

using s1 = sbits1;
using s2 = sbits2;
using s3 = sbits3;
using s4 = sbits4;
using s5 = sbits5;
using s6 = sbits6;
using s7 = sbits7;
using s9 = sbits9;

// String
#include <string>
typedef std::string string;

// Collections // #TODO Review value of aliasing std:: collections
#include <vector>
template <typename T>
using vector = std::vector<T>;

#include <array>
template <typename T, u64 size>
using array = std::array<T, size>;

#include <unordered_map>
template <typename T, typename U>
using uMap = std::unordered_map<T, U>;

#include <map>
template <typename T, typename U>
using oMap = std::map<T, U>;

#include <unordered_set>
template <typename T, typename U>
using uSet = std::unordered_set<T, U>;

#include <set>
template <typename T, typename U>
using oSet = std::set<T, U>;

#include <stack>
template <typename T>
using stack = std::stack<T>;

#include <queue>
template <typename T>
using queue = std::queue<T>;

// Memory
#include <memory>
template<typename T>
using sPtr = std::shared_ptr<T>;

template<typename T>
using uPtr = std::unique_ptr<T>;

template<typename T>
using wPtr = std::weak_ptr<T>;

// Templating
template<typename... T>
struct TemplateArgumentList { };

// #TODO This file should probably not define anything and just list other files to include in all other files
// #TODO Move GLM type aliasing to another/better file/place
#ifdef _QGLM
#include "Libraries/glm/glm.hpp"

typedef glm::vec2		vec2f;

typedef glm::u16vec2	vec2u16; // #TODO Review aliasing and it's usefulness and clarity

typedef glm::vec3		vec3f;
typedef glm::vec4		vec4f;
typedef glm::mat3		mat3;
typedef glm::mat4		mat4;
typedef glm::quat		quat;
#endif