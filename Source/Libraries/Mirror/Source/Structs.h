#pragma once

#include <string>
#include <vector>

#ifdef MIRROR_TESTING
#include <cassert>
#define MIRROR_ASSERT(x) assert(x)
#else
#define MIRROR_ASSERT(x)
#endif

#include "ConstexprCounter.h"
template<int N = 1, int C = reader(0, flag<128>())> // #NOTE 0 reserved for invalid value, up to max value of 128(+1)
int constexpr NextTypeId(int R = writer<C + N>::value) {
	return R;
}

#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

#define MIRROR_FIELD_FLAG_SIZE uint8_t
#define MIRROR_TYPE_CATEGORY_SIZE uint8_t
#define MIRROR_TYPE_SIZE uint16_t
#define MIRROR_FIELD_ID_SIZE std::size_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX

struct Mirror
{
	enum TypeInfoCategories : MIRROR_TYPE_CATEGORY_SIZE
	{
		TypeInfoCategory_Primitive = 0,
		TypeInfoCategory_Class,
		TypeInfoCategory_Collection,
		TypeInfoCategory_Pair,
		TypeInfoCategory_Pointer,
	};

	struct TypeInfo;

	struct Field
	{
		const TypeInfo* typeInfo = nullptr;
		std::string name = "";
		std::size_t offset = 0;

#ifndef MIRROR_TYPE_SIZE_UNUSED
		MIRROR_TYPE_SIZE size = 0;
#endif

#ifndef MIRROR_FIELD_FLAGS_UNUSED
		MIRROR_FIELD_FLAG_SIZE flags = 0;
#endif
	};

	using Func_void_voidPtr_sizet_constvoidPtr_constvoidPtr = void (*)(void*, size_t, const void*, const void*);
	using Func_void_voidPtr = void (*)(void*);
	using Func_voidPtr_constVoidPtr_bool = void* (*)(const void*, bool);
	using Func_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
	using Func_bool_constVoidPtr = bool (*)(const void*);

	struct TypeInfo
	{
		// #TODO Review accessibility of members. Should this be a class, or at least use a private scope for some/all data members?
		// #TODO Review memory usage for each type. There are opportunities for reducing the memory usage for some types, and re-using data member memory using unions

		std::string stringName = "";
		MIRROR_FIELD_ID_SIZE id = 0;

#ifndef MIRROR_TYPE_SIZE_UNUSED
		MIRROR_TYPE_SIZE size = 0;
#endif
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { };
		std::vector<const TypeInfo*> derivedTypes; // #TODO Only required for base classes (with declared derived types)

		const TypeInfo* AbsoluteType() const {
			return pointerDereferencedTypeInfo ? pointerDereferencedTypeInfo : this;
		}

		const TypeInfo* DerivedTypeFromPointer(const void* instanceAddress) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes)
			{
				MIRROR_ASSERT(derivedType->typeDynamicCastFunc && "Null typeDynamicCastFunc!");
				if (derivedType->typeDynamicCastFunc(instanceAddress))
				{
					return derivedType;
				}
			}
			return this;
		}

		const TypeInfo* DerivedTypeFromTypeName(const std::string& typeName) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes)
			{
				if (strcmp(derivedType->stringName.c_str(), typeName.c_str()) == 0)
				{
					return derivedType;
				}
			}
			return this;
		}

		void CollectionAppend(void* collectionAddress, size_t index, void* first, void* second = nullptr) const {
			MIRROR_ASSERT(collectionAddFunc && "Null collectionAddFunc!");
			if (collectionAddFunc)
			{
				collectionAddFunc(collectionAddress, index, first, second);
			}
		}

		void Construct(void* instanceAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null typeConstructorFunc!");
			if (typeConstructorFunc)
			{
				typeConstructorFunc(instanceAddress);
			}
		}

		void ClearCollection(void* collectionAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null collectionClearFunction!");
			if (collectionClearFunction)
			{
				collectionClearFunction(collectionAddress);
			}
		}

		const TypeInfo* collectionTypeInfoFirst = nullptr; // #TODO Only used for collections
		const TypeInfo* collectionTypeInfoSecond = nullptr; // #TODO Only used for collections
		const TypeInfo* superTypeInfo = nullptr; // #TODO Only used for derived types
		const TypeInfo* pointerDereferencedTypeInfo = nullptr; // #TODO Only used for pointers

		Func_void_voidPtr_sizet_constvoidPtr_constvoidPtr collectionAddFunc = nullptr;
		Func_voidPtr_constVoidPtr_bool collectionAddressOfPairObjectFunc = nullptr;
		Func_void_voidPtr collectionClearFunction = nullptr;
		Func_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		Func_void_voidPtr typeConstructorFunc = nullptr; // #TODO Only used for classes
		Func_bool_constVoidPtr typeDynamicCastFunc = nullptr; // #TODO Only used for classes in inheritance hierarchy
	};

	template <class T>
	static const TypeInfo* InfoForType(const T& typeObj);

	template<typename T>
	static const TypeInfo* InfoForType();

	template<typename T>
	static constexpr size_t TypeId();

	// #NOTE Defining type ids through templated functions aims to avoid id collisions with generated class ids
	// #NOTE TypeIds are defined in headers so all compile units have matching type ids for each type
#define MIRROR_TYPE_ID_IMPL(TYPE) \
	template <> constexpr std::size_t Mirror::TypeId<TYPE>() { return NextTypeId(); }
#define MIRROR_TYPE_ID(...) MIRROR_TYPE_ID_IMPL(__VA_ARGS__)

#define MIRROR_TYPE_ID_CLASS(...) MIRROR_TYPE_ID_CLASS_IMPL(__VA_ARGS__)
#define MIRROR_TYPE_ID_CLASS_IMPL(TYPE) \
	template <> \
	static constexpr size_t Mirror::TypeId<TYPE>() { \
		auto result = HashFromString(#TYPE); \
		assert(result > PREDEFINED_ID_MAX); \
		return result; \
	}

	template<typename... T>
	struct MirrorTemplateArgumentList { };
};
