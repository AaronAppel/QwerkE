#pragma once

// #NOTE Using 1 header for convenience but can only #include this file once to avoid compile/linker errors

#if 1 // Enable when testing
#include <unordered_map>
#include <string>
#include <vector>

#include "Mirror.h"

namespace QwerkE {

    // #TODO Add inheritance structure
    struct TestStruct
    {
        bool m_Bool = true;
        uint8_t m_U8 = 1;
        uint16_t m_U16 = 2;
        uint32_t m_U32 = 3;
        uint64_t m_U64 = 4;
        int8_t m_S8 = 5;
        int16_t m_S16 = 6;
        int32_t m_S32 = 7;
        int64_t m_S64 = 8;
        float m_Float = 9.00001f;
        double m_Double = 10.000000001;

        std::string m_String = Constants::gc_DefaultStringValue;
        const char* m_ConstCharPtr = Constants::gc_DefaultStringValue;

        float m_FloatArray10[10] = { 0.1f };
        std::vector<char> m_CharVector = { 'A' };
        std::unordered_map<std::string, int32_t> m_UmapStringInt32 = { { "Str", 123 }};

        int32_t* m_Int32Ptr = nullptr;
        // #TODO More complicated pointers
    };

    // Add to MirrorTypes.h "m_arr_float10,"
    using m_arr_float10 = float[10];
    MIRROR_ARRAY(m_arr_float10, float)

    // Add to MirrorTypes.h "m_vec_char,"
    typedef std::vector<char> m_vec_char;
    MIRROR_VECTOR(m_vec_char, char)

    // Add to MirrorTypes.h "m_pair_string_int32,"
    typedef std::pair<std::string, int32_t> m_pair_string_int32;
    MIRROR_PAIR(m_pair_string_int32, std::string, int32_t);

    // Add to MirrorTypes.h "m_umap_string_s32,"
    typedef std::unordered_map<std::string, int32_t> m_umap_string_int32;
    MIRROR_MAP(m_umap_string_int32, m_pair_string_int32)

    // Add to MirrorTypes.h "m_int32Ptr,"
    typedef int32_t* m_int32Ptr;
    MIRROR_POINTER(m_int32Ptr)

    MIRROR_CLASS_START(TestStruct)
    MIRROR_CLASS_MEMBER(m_Bool)
    MIRROR_CLASS_MEMBER(m_U8)
    MIRROR_CLASS_MEMBER(m_U16)
    MIRROR_CLASS_MEMBER(m_U32)
    MIRROR_CLASS_MEMBER(m_U64)
    MIRROR_CLASS_MEMBER(m_S8)
    MIRROR_CLASS_MEMBER(m_S16)
    MIRROR_CLASS_MEMBER(m_S32)
    MIRROR_CLASS_MEMBER(m_S64)
    MIRROR_CLASS_MEMBER(m_Float)
    MIRROR_CLASS_MEMBER(m_Double)
    MIRROR_CLASS_MEMBER(m_String)
    MIRROR_CLASS_MEMBER(m_ConstCharPtr)
    MIRROR_CLASS_MEMBER(m_FloatArray10)
    MIRROR_CLASS_MEMBER(m_CharVector)
    MIRROR_CLASS_MEMBER(m_UmapStringInt32)
    MIRROR_CLASS_MEMBER(m_Int32Ptr)
    MIRROR_CLASS_END(TestStruct);

}
#endif
