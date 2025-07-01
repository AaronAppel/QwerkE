#pragma once

#include <stdint.h>
#include <string>
#include <vector>

// #NOTE Functionality macros. See Macros section in README.md for more info.
// #define MIRROR_TESTING
// #define MIRROR_OMIT_ALT_API
// #define MIRROR_OMIT_FLAGS

#ifdef MIRROR_TESTING
#include <cassert>
#define MIRROR_ASSERT(x) assert(x)
#else
#define MIRROR_ASSERT(x)
#endif

#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

#define MIRROR_FIELD_FLAG_TYPE uint8_t
#define MIRROR_FIELD_FLAG_MAX UINT8_MAX
#define MIRROR_TYPE_ID_TYPE uint16_t
#define MIRROR_TYPE_ID_MAX UINT16_MAX
#define MIRROR_TYPE_SIZE_TYPE uint16_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX
#define MIRROR_TYPE_CATEGORY_TYPE uint8_t
#define MIRROR_TYPE_CATEGORY_SIZE_MAX UINT8_MAX

#ifndef MIRROR_OMIT_ALT_API // #NOTE Optional, alternative API
struct Mirror;
using Mir = Mirror;
#endif // !#define MIRROR_OMIT_ALT_API

struct Mirror
{
	struct TypeInfo;

#ifndef MIRROR_OMIT_ALT_API // #NOTE Optional, alternative API
	using Id = MIRROR_TYPE_ID_TYPE;
	using Info = TypeInfo;
	template <typename T>
	static auto GetInfo(T& typeObj) -> const TypeInfo* {
		return InfoForType(typeObj);
	}
	template <typename T>
	static auto GetInfo() -> const TypeInfo* {
		return InfoForType<T>();
	}
	template <typename T>
	static constexpr auto GetId(T& typeObj) -> MIRROR_TYPE_ID_TYPE {
		return IdForType<T>();
	}
	template <typename T>
	static constexpr auto GetId() -> MIRROR_TYPE_ID_TYPE {
		return IdForType<T>();
	}
#endif // !#define MIRROR_OMIT_ALT_API

	enum TypeInfoCategories : MIRROR_TYPE_CATEGORY_TYPE
	{
		TypeInfoCategory_Primitive = 0, // #TODO Review maintaining order in Mirror.h and everywhere else (class after collection)
		TypeInfoCategory_Class,
		TypeInfoCategory_Collection,
		TypeInfoCategory_Pointer
	};

	struct Field
	{
		const TypeInfo* typeInfo = nullptr;

		std::string name = "";
		std::size_t offset = 0;
		MIRROR_TYPE_SIZE_TYPE size = 0;
#ifndef MIRROR_OMIT_FLAGS
		MIRROR_FIELD_FLAG_TYPE flags = 0;
#endif // !MIRROR_OMIT_FLAGS
	};

	struct TypeInfo
	{
		std::string stringName = "";
		MIRROR_TYPE_ID_TYPE id = 0;
		MIRROR_TYPE_SIZE_TYPE size = 0;
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { };
		std::vector<const TypeInfo*> derivedTypes;

		const TypeInfo* collectionTypeInfoFirst = nullptr;
		const TypeInfo* collectionTypeInfoSecond = nullptr;

		std::vector<const TypeInfo*> collectionTypeInfos; // #TODO Review using an array or std::array

		// #TODO Support tuples and handle collection logic type-agnosticly using a vector of const TypeInfo*s?
		const TypeInfo* superTypeInfo;
		const TypeInfo* pointerDereferencedTypeInfo;

		using FuncPtr_void_voidPtr_sizet_constvoidPtr_constvoidPtr = void (*)(void*, size_t, const void*, const void*);
		using FuncPtr_void_voidPtr = void (*)(void*);
		using FuncPtr_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
		using FuncPtr_bool_constVoidPtr = bool (*)(const void*);

		FuncPtr_void_voidPtr_sizet_constvoidPtr_constvoidPtr collectionAddFunc = nullptr;
		FuncPtr_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		using FuncPtr_constVectorSizetPtr = const std::vector<size_t>* (*)();
		FuncPtr_constVectorSizetPtr collectionOffsetsVecFunc = nullptr;

		FuncPtr_void_voidPtr typeConstructorFunc = nullptr;
		FuncPtr_bool_constVoidPtr typeDynamicCastFunc = nullptr;

		const TypeInfo* AbsoluteType() const {
			return pointerDereferencedTypeInfo ? pointerDereferencedTypeInfo : this;
		}

		const TypeInfo* DerivedTypeFromPointer(const void* instanceAddress) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes) {
				MIRROR_ASSERT(derivedType->typeDynamicCastFunc && "Null typeDynamicCastFunc!");
				if (derivedType->typeDynamicCastFunc(instanceAddress)) {
					return derivedType;
				}
			}
			return this;
		}

		const TypeInfo* DerivedTypeFromTypeName(const std::string& typeName) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes) {
				if (strcmp(derivedType->stringName.c_str(), typeName.c_str()) == 0) {
					return derivedType;
				}
			}
			return this;
		}

		void CollectionAppend(void* collectionAddress, size_t index, void* first, void* second = nullptr) const {
			MIRROR_ASSERT(collectionAddFunc && "Null collectionAddFunc!");
			if (collectionAddFunc) {
				collectionAddFunc(collectionAddress, index, first, second);
			}
		}

		void Construct(void* instanceAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null typeConstructorFunc!");
			if (typeConstructorFunc) {
				typeConstructorFunc(instanceAddress);
			}
		}
	};

	template <typename T>
	static const TypeInfo* InfoForType(T& typeObj); // #NOTE Making arg const, reference, etc, requires additional reflecting of T as const, reference, etc

	template <typename T>
	static const TypeInfo* InfoForType();

	template <typename T>
	static constexpr MIRROR_TYPE_ID_TYPE IdForType(T& typeObj); // #NOTE Making arg const, reference, etc, requires additional reflecting of T as const, reference, etc

	template <typename T>
	static constexpr MIRROR_TYPE_ID_TYPE IdForType();

#define VA_ARGS_STRING(...) #__VA_ARGS__
#define VA_ARGS(...) __VA_ARGS__
#define STRIP_PARENTHESES(X) X

#define TYPE_WRAP_STRING(TYPE) STRIP_PARENTHESES( VA_ARGS_STRING TYPE )
#define TYPE_WRAP(TYPE) STRIP_PARENTHESES(VA_ARGS TYPE)

#if _MSC_VER && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL) // Compile option disabled: /Zc:preprocessor
#define MIRROR_TYPE_ID_IMPL(ID, TYPE) \
	template <> constexpr MIRROR_TYPE_ID_TYPE Mirror::IdForType<TYPE>() { return ID; }

#define MIRROR_TYPE_ID(ID, ...) MIRROR_TYPE_ID_IMPL(ID, __VA_ARGS__)

#else // Compile option enabled: /Zc:preprocessor
#define MIRROR_TYPE_ID_IMPL(ID, TYPE) \
	template <> constexpr MIRROR_TYPE_ID_TYPE Mirror::IdForType<TYPE_WRAP(TYPE)>() { return ID; }

#define MIRROR_TYPE_ID(ID, ...) MIRROR_TYPE_ID_IMPL(ID, (__VA_ARGS__))

#endif // _MSC_VER && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)

	template<typename... T>
	struct TypesList { };

};
