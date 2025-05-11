#pragma once

#include <string>
#include <vector>

// #NOTE Functionality macros. See Macros section in README.md for more info.
// #define MIRROR_TESTING
// #define MIRROR_TYPE_SIZE_UNUSED
// #define MIRROR_FIELD_FLAGS_UNUSED

#include "MIR_ConstexprCounter.h"

#ifdef MIRROR_TESTING
#include <cassert>
#define MIRROR_ASSERT(x) assert(x)
#else
#define MIRROR_ASSERT(x)
#endif

#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

#define MIRROR_FIELD_FLAG_SIZE uint8_t
#define MIRROR_FIELD_FLAG_SIZE_MAX UINT8_MAX
#define MIRROR_FIELD_ID_SIZE uint16_t
#define MIRROR_FIELD_ID_SIZE_MAX UINT16_MAX
#define MIRROR_TYPE_SIZE uint16_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX
#define MIRROR_TYPE_CATEGORY_SIZE uint8_t
#define MIRROR_TYPE_CATEGORY_SIZE_MAX UINT8_MAX

struct Mirror
{
	enum TypeInfoCategories : uint8_t
	{
		TypeInfoCategory_Primitive = 0, // #TODO Review maintaining order in Mirror.h and everywhere else (class after collection)
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

	struct TypeInfo
	{
		std::string stringName = "";
		MIRROR_FIELD_ID_SIZE id = 0;

#ifndef MIRROR_TYPE_SIZE_UNUSED
		MIRROR_TYPE_SIZE size = 0;
#endif
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { };
		std::vector<const TypeInfo*> derivedTypes;

		const TypeInfo* collectionTypeInfoFirst = nullptr;
		const TypeInfo* collectionTypeInfoSecond = nullptr;
		const TypeInfo* superTypeInfo;
		const TypeInfo* pointerDereferencedTypeInfo;

		using FuncPtr_void_voidPtr_sizet_constvoidPtr_constvoidPtr = void (*)(void*, size_t, const void*, const void*);
		using FuncPtr_void_voidPtr = void (*)(void*);
		using FuncPtr_voidPtr_constVoidPtr_bool = void* (*)(const void*, bool);
		using FuncPtr_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
		using FuncPtr_bool_constVoidPtr = bool (*)(const void*);

		FuncPtr_void_voidPtr_sizet_constvoidPtr_constvoidPtr collectionAddFunc = nullptr;
		FuncPtr_voidPtr_constVoidPtr_bool collectionAddressOfPairObjectFunc = nullptr;
		FuncPtr_void_voidPtr collectionClearFunction = nullptr;
		FuncPtr_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		FuncPtr_void_voidPtr typeConstructorFunc = nullptr;
		FuncPtr_bool_constVoidPtr typeDynamicCastFunc = nullptr;

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
	};

	template <class T>
	static const TypeInfo* InfoForType(const T& typeObj);

	template<typename T>
	static const TypeInfo* InfoForType();

	template<typename T>
	static constexpr MIRROR_FIELD_ID_SIZE TypeId();

#if defined(MIRROR_NONCONFORMING) && defined(MIRROR_GENERATE_TYPE_IDS)
	template<int N = 1, int C = ConstexprCounter::reader(0, ConstexprCounter::flag<128>())> // #NOTE 0 reserved for invalid value, up to max value of 128(+1)
	static int constexpr NextTypeId(int R = ConstexprCounter::writer<C + N>::value) { return R; }

#define MIRROR_TYPE_ID_IMPL(TYPE) \
	template <> constexpr MIRROR_FIELD_ID_SIZE Mirror::TypeId<TYPE>() { return NextTypeId(); }
#define MIRROR_TYPE_ID(...) MIRROR_TYPE_ID_IMPL(__VA_ARGS__)

#else
#define MIRROR_TYPE_ID_IMPL(ID, TYPE) \
	template <> constexpr MIRROR_FIELD_ID_SIZE Mirror::TypeId<TYPE>() { return ID; }
#define MIRROR_TYPE_ID(ID, ...) MIRROR_TYPE_ID_IMPL(ID, __VA_ARGS__)

#endif

	template<typename... T>
	struct MirrorTemplateArgumentList { };
};
