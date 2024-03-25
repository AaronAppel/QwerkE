#include "Mirror.h"

#include <string>

// Primitive type defines
typedef int8_t m_int8_t; // Typedef to allow primitive type support in the MirrorTypes enum
typedef int16_t m_int16_t;
typedef int32_t m_int32_t;
typedef int64_t m_int64_t;
typedef uint8_t m_uint8_t;
typedef uint16_t m_uint16_t;
typedef uint32_t m_uint32_t;
typedef uint64_t m_uint64_t;

MIRROR_TYPE(m_int8_t)
MIRROR_TYPE(m_int16_t)
MIRROR_TYPE(m_int32_t)
MIRROR_TYPE(m_int64_t)
MIRROR_TYPE(m_uint8_t)
MIRROR_TYPE(m_uint16_t)
MIRROR_TYPE(m_uint32_t)
MIRROR_TYPE(m_uint64_t)

typedef bool m_bool;
typedef char m_char;

MIRROR_TYPE(m_bool)
MIRROR_TYPE(m_char)

typedef float m_float;
typedef double m_double;

MIRROR_TYPE(m_float)
MIRROR_TYPE(m_double)

typedef char* m_charPtr;
typedef std::string m_string;
typedef const char* m_constCharPtr;

MIRROR_TYPE(m_charPtr)
MIRROR_TYPE(m_string)
MIRROR_TYPE(m_constCharPtr)