#include "QF_Serialize.h"

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MirrorTesting.h"
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

}
