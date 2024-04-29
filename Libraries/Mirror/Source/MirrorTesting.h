#pragma once

// #NOTE Using 1 header for convenience but can only #include this file once to avoid compile/linker errors

#if 1 // Enable when testing
#include <unordered_map>
#include <string>
#include <vector>

#include "Mirror.h"

namespace QwerkE {

    // #TODO Add inheritance structure

    struct Base
    {
        int baseX = 1;
    };

    struct Derived : public Base
    {
        float derivedY = 2.f;
    };

    // #TODO
    // - Add pointer requiring deserialize allocation
    // - Derived type de/serialize
    // - Dependent constructor argument(s)
    struct TestStruct
    {
        TestStruct()
        {
            // m_DerivedPtr = new Derived();
        }

        ~TestStruct()
        {
            if (m_DerivedPtr)
            {
                delete m_DerivedPtr;
            }
        }

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

        std::string m_String = "null";
        const char* m_ConstCharPtr = "null";

        float m_FloatArray10[10] = { 0.1f };
        std::vector<char> m_CharVector = { 'A' };
        std::unordered_map<std::string, int32_t> m_UmapStringInt32 = { { "Str", 123 }};

        int32_t* m_Int32Ptr = nullptr;
        // #TODO More complicated pointers
        Base m_Base;
        Derived m_Derived;
        Derived* m_DerivedPtr = nullptr;
    };

}
#endif
