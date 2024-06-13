#include "Mirror.h"

// Void
// #TODO Clean up void specific (avoid sizeof()) implementation
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void>() {
    static Mirror::TypeInfo localStaticTypeInfo; if (!localStaticTypeInfo.stringName.empty()) {
        return &localStaticTypeInfo;
    }
    localStaticTypeInfo.category = GetCategory<void>();
    localStaticTypeInfo.id = Mirror::TypeId<void>(); localStaticTypeInfo.stringName = typeid(void).name();
    SetConstructionLambda<void>(&localStaticTypeInfo, std::is_same<void, std::string>::type());
    return &localStaticTypeInfo;
}
// MIRROR_TYPE(void)
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void*>() {
    static_assert(sizeof(void*) <= 0xffffui16, "Size is larger than member can hold!"); static Mirror::TypeInfo localStaticTypeInfo; if (!localStaticTypeInfo.stringName.empty()) {
        return &localStaticTypeInfo;
    } localStaticTypeInfo.category = GetCategory<void*>(); localStaticTypeInfo.stringName = "void*"; localStaticTypeInfo.id = Mirror::TypeId<void*>(); localStaticTypeInfo.size = sizeof(void*); switch (localStaticTypeInfo.category) {
    case TypeInfoCategory_Collection: case TypeInfoCategory_Pair: SetCollectionLambdas<void*>(&localStaticTypeInfo, is_stl_container_impl::is_stl_container<void*>::type()); case TypeInfoCategory_Class: SetConstructionLambda<void*>(&localStaticTypeInfo, std::is_class<void*>::type()); break; case TypeInfoCategory_Pointer: localStaticTypeInfo.pointerDereferencedTypeInfo = Mirror::InfoForType<std::remove_pointer_t<void*>>(); break; case TypeInfoCategory_Primitive: SetConstructionLambda<void*>(&localStaticTypeInfo, std::is_same<void*, std::string>::type()); break;
    } return &localStaticTypeInfo;
}

// Mutable
MIRROR_TYPE(signed char)
MIRROR_TYPE(signed short)
MIRROR_TYPE(signed int)
MIRROR_TYPE(signed long long)
MIRROR_TYPE(unsigned char)
MIRROR_TYPE(unsigned short)
MIRROR_TYPE(unsigned int)
MIRROR_TYPE(unsigned long long)
MIRROR_TYPE(float)
MIRROR_TYPE(double)
MIRROR_TYPE(long double)
MIRROR_TYPE(char)
MIRROR_TYPE(bool)

// Const
MIRROR_TYPE(const signed char)
MIRROR_TYPE(const signed short)
MIRROR_TYPE(const signed int)
MIRROR_TYPE(const signed long long)
MIRROR_TYPE(const unsigned char)
MIRROR_TYPE(const unsigned short)
MIRROR_TYPE(const unsigned int)
MIRROR_TYPE(const unsigned long long)
MIRROR_TYPE(const float)
MIRROR_TYPE(const double)
MIRROR_TYPE(const long double)
MIRROR_TYPE(const char)
MIRROR_TYPE(const bool)

// Mutable pointers
MIRROR_TYPE(signed char*)
MIRROR_TYPE(signed short*)
MIRROR_TYPE(signed int*)
MIRROR_TYPE(signed long long*)
MIRROR_TYPE(unsigned char*)
MIRROR_TYPE(unsigned short*)
MIRROR_TYPE(unsigned int*)
MIRROR_TYPE(unsigned long long*)
MIRROR_TYPE(float*)
MIRROR_TYPE(double*)
MIRROR_TYPE(long double*)
MIRROR_TYPE(char*)
MIRROR_TYPE(bool*)

// Const pointers
MIRROR_TYPE(const signed char*)
MIRROR_TYPE(const signed short*)
MIRROR_TYPE(const signed int*)
MIRROR_TYPE(const signed long long*)
MIRROR_TYPE(const unsigned char*)
MIRROR_TYPE(const unsigned short*)
MIRROR_TYPE(const unsigned int*)
MIRROR_TYPE(const unsigned long long*)
MIRROR_TYPE(const float*)
MIRROR_TYPE(const double*)
MIRROR_TYPE(const long double*)
MIRROR_TYPE(const char*)
MIRROR_TYPE(const bool*)

// String
#include <string>
MIRROR_TYPE(std::string)
MIRROR_TYPE(std::string*)
MIRROR_TYPE(const std::string)
MIRROR_TYPE(const std::string*)
