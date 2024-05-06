#include "QF_Serialize.h"

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Mirror/Source/MirrorTesting.h"
#include "Libraries/Mirror/Source/MirrorTypes.h"
#endif

namespace QwerkE {

    namespace Serialize {

        void SerializeTest()
        {
            // Serialize with unique values
            TestStruct testStructSerialize;
            auto loc = &testStructSerialize.m_UmapStringInt32;

            testStructSerialize.m_Base.baseX = 90;
            testStructSerialize.m_Derived1.derivedZ = 80;
            testStructSerialize.m_Derived1.baseX = 70;
            if (testStructSerialize.m_Derived1Ptr)
            {
                testStructSerialize.m_Derived1Ptr->baseX = 60;
                testStructSerialize.m_Derived1Ptr->derivedZ = 50;
            }
            for (size_t i = 0; i < 10; i++)
            {
                testStructSerialize.m_FloatArray10[i] = i + (.1 * i);
            }
            for (size_t i = 0; i < 5; i++)
            {
                testStructSerialize.m_CharVector.push_back(66 + i);
            }
            testStructSerialize.m_Bool = !testStructSerialize.m_Bool;
            testStructSerialize.m_U8 += 1;
            testStructSerialize.m_U16 += 1;
            testStructSerialize.m_U32 += 1;
            testStructSerialize.m_U64 += 1;
            testStructSerialize.m_S8 += 1;
            testStructSerialize.m_S16 += 1;
            testStructSerialize.m_S32 += 1;
            testStructSerialize.m_S64 += 1;
            testStructSerialize.m_Float += 1.5;
            testStructSerialize.m_Double += 1.5;
            testStructSerialize.m_String = "test";
            testStructSerialize.m_ConstCharPtr = "test";
            testStructSerialize.m_UmapStringInt32.clear();
            testStructSerialize.m_UmapStringInt32.insert({ "test", 890 });
            auto loc2 = &testStructSerialize.m_UmapStringInt32;

            testStructSerialize.m_Derived1Ptr = new Derived1();
            testStructSerialize.m_Derived2Ptr = new Derived2();

            if (true) testStructSerialize.m_BasePtrDerived = new Derived1();
            else if (true) testStructSerialize.m_BasePtrDerived = new Derived2();
            else testStructSerialize.m_BasePtrDerived = new Base();

            testStructSerialize.m_EditorWindowPtr = new Editor::EditorWindowAssets(GUID());

            Serialize::ToFile(testStructSerialize, "TestStruct");

            // Deserialize
            TestStruct testStructDeserialize;
            Serialize::FromFile("TestStruct", testStructDeserialize);
            testStructDeserialize.m_BasePtrDerived;
        }

    }

    template <typename Super, typename... SubClass>
    void MirrorSubClassUserType(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
    {
        uint16_t enumValue = enumStartOffset;
        ([&]()
            {
                const QwerkE::Mirror::TypeInfo* subclassTypeInfo = QwerkE::Mirror::InfoForType<SubClass>();
                localStaticTypeInfo.derivedTypes.push_back(subclassTypeInfo);
                const_cast<QwerkE::Mirror::TypeInfo*>(subclassTypeInfo)->superTypeInfo = &localStaticTypeInfo;
                const_cast<QwerkE::Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc =
                    [](const void* pointerToInstance) -> bool {
                    SubClass* subClass = (SubClass*)pointerToInstance;
                    return dynamic_cast<SubClass*>(*(Super**)pointerToInstance) != nullptr;
                    };
                ++enumValue;
            }(), ...);
    }

    template<typename Super, typename... T>
    static void MirrorSubClassUserTypes(MirrorTemplateArgumentList<T...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
    {
        MirrorSubClassUserType<Super, T...>(localStaticTypeInfo, enumStartOffset);
    }

    MIRROR_CLASS_START(Derived1)
    MIRROR_CLASS_MEMBER(derivedZ)
    MIRROR_CLASS_END(Derived1)

    MIRROR_CLASS_START(Derived2)
    MIRROR_CLASS_MEMBER(derivedY)
    MIRROR_CLASS_END(Derived2)

    typedef Derived1* m_derived1Ptr;
    MIRROR_POINTER(m_derived1Ptr)

    typedef Derived2* m_derived2Ptr;
    MIRROR_POINTER(m_derived2Ptr)

    MIRROR_CLASS_START(Base)
    MIRROR_CLASS_MEMBER(baseX)
    MirrorSubClassUserTypes<Base>(MirrorTemplateArgumentList<Derived1, Derived2>{}, localStaticTypeInfo, 0);
    MIRROR_CLASS_END(Base)

    typedef Base* m_basePtr;
    MIRROR_POINTER(m_basePtr)

    // Add to MirrorTypes.h "m_arr_float10,"
    using m_arr_float10 = float[10];
    MIRROR_ARRAY(m_arr_float10, float)

    // Add to MirrorTypes.h "m_vec_char,"
    typedef std::vector<char> m_vec_char;
    MIRROR_VECTOR(m_vec_char, char)

    // Add to MirrorTypes.h "m_pair_string_int32,"
    typedef std::pair<std::string, int32_t> m_pair_string_int32;
    MIRROR_PAIR(m_pair_string_int32)

    // Add to MirrorTypes.h "m_umap_string_s32,"
    typedef std::unordered_map<std::string, int32_t> m_umap_string_int32;
    MIRROR_MAP(m_umap_string_int32, m_pair_string_int32)

    // Add to MirrorTypes.h "m_int32Ptr,"
    typedef int32_t* m_int32Ptr;
    MIRROR_POINTER(m_int32Ptr)

}
