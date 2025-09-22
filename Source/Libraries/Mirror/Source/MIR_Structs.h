#pragma once

#include <stdint.h>
#include <string>
#include <vector>

// #NOTE Functionality macros. See Macros section in README.md for more info.
// #define MIR_TESTING
// #define MIR_OMIT_ALT_API
// #define MIR_OMIT_FLAGS

#ifdef MIR_TESTING
#include <cassert>
#define MIR_ASSERT(x) assert(x)
#else
#define MIR_ASSERT(x)
#endif

#define MIR_FRIEND friend struct Mirror;

#define MIR_FIELD_FLAG_TYPE uint8_t
#define MIR_FIELD_FLAG_MAX UINT8_MAX

// #TODO Allow user override but specify default values
#ifndef MIR_TYPE_ID_TYPE
#define MIR_TYPE_ID_TYPE uint16_t
#endif // !MIR_TYPE_ID_TYPE

#define MIR_TYPE_ID_MAX UINT16_MAX
#define MIR_TYPE_SIZE_TYPE uint16_t
#define MIR_TYPE_SIZE_MAX UINT16_MAX
#define MIR_TYPE_CATEGORY_TYPE uint8_t
#define MIR_TYPE_CATEGORY_SIZE_MAX UINT8_MAX

#ifndef MIR_OMIT_ALT_API // #NOTE Optional, alternative API
struct Mirror;
using Mir = Mirror;
#endif // !#define MIR_OMIT_ALT_API

struct Mirror
{
	struct TypeInfo;

#ifndef MIR_OMIT_ALT_API // #NOTE Optional, alternative API
	using Id = MIR_TYPE_ID_TYPE;
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
	static constexpr auto GetId(T& typeObj) -> MIR_TYPE_ID_TYPE {
		return IdForType<T>();
	}
	template <typename T>
	static constexpr auto GetId() -> MIR_TYPE_ID_TYPE {
		return IdForType<T>();
	}
#endif // !#define MIR_OMIT_ALT_API

	enum TypeInfoCategories : MIR_TYPE_CATEGORY_TYPE
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
		MIR_TYPE_SIZE_TYPE size = 0;
#ifndef MIR_OMIT_FLAGS
		MIR_FIELD_FLAG_TYPE flags = 0;
#endif // !MIR_OMIT_FLAGS
	};

	struct TypeInfo
	{
		TypeInfo() : superTypeInfo(nullptr), typeDynamicCastFunc(nullptr) { }

		std::string stringName = ""; // #TODO Could reduce overhead/class size using const char*
		MIR_TYPE_ID_TYPE id = 0;
		MIR_TYPE_SIZE_TYPE size = 0;
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { };

		// #TODO Support tuples and handle collection logic type-agnosticly using a vector of const TypeInfo*s?

		using FuncPtr_void_voidPtr_sizet_constvoidPtr = void (*)(void*, size_t, const void*);
		using FuncPtr_void_voidPtr = void (*)(void*);
		using FuncPtr_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
		using FuncPtr_bool_constVoidPtr = bool (*)(const void*);

		// #TODO No need for dynamic array if known at compile time. Move as much logic as possible to compile time.
		// There is benefit to avoiding type reflection if never referenced but may not be practically worthwhile.

		std::vector<const TypeInfo*> collectionTypeInfos; // #TODO Review using an array or std::array
		std::vector<const TypeInfo*> derivedTypes; // #TODO Review using an array or std::array

		// #TODO Review anonymous union structs for aliasing "opposing" members
		union
		{
			struct // Collection or pointer specific
			{
				const TypeInfo* pointerDereferencedTypeInfo; // #NOTE Pointer specific
				// #TODO Type won't play nice std::vector<const TypeInfo*> collectionTypeInfos; // #TODO Review using an array or std::array

				FuncPtr_void_voidPtr_sizet_constvoidPtr collectionAddFunc;
				// FuncPtr_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc2;
			};
			struct // Class specific
			{
				const TypeInfo* superTypeInfo;
				FuncPtr_bool_constVoidPtr typeDynamicCastFunc;
				// #TODO Type won't play nice std::vector<const TypeInfo*> derivedTypes;
			};
		};

		FuncPtr_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		FuncPtr_void_voidPtr typeConstructorFunc = nullptr;

		// #TODO Review helper methods to deprecate
		const TypeInfo* AbsoluteType() const {
			return pointerDereferencedTypeInfo ? pointerDereferencedTypeInfo : this;
		}

		const TypeInfo* DerivedTypeFromPointer(const void* instanceAddress) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes) {
				MIR_ASSERT(derivedType->typeDynamicCastFunc && "Null typeDynamicCastFunc!");
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

		void CollectionAppend(void* collectionAddress, size_t index, void* first) const {
			MIR_ASSERT(collectionAddFunc && "Null collectionAddFunc!");
			if (collectionAddFunc) {
				collectionAddFunc(collectionAddress, index, first);
			}
		}

		void Construct(void* instanceAddress) const {
			MIR_ASSERT(collectionAddFunc && "Null typeConstructorFunc!");
			if (typeConstructorFunc) {
				typeConstructorFunc(instanceAddress);
			}
		}
	};

	template <typename T>
	static const TypeInfo* InfoForType(T& typeObj); // #NOTE Making arg const, reference, etc, requires additional reflecting of T as const, reference, etc

	template <typename T>
	static const TypeInfo* InfoForType();

	template <typename T> // #NOTE Fails when using arrays due to [] brace ordering, so define here for all types
	static constexpr MIR_TYPE_ID_TYPE IdForType(T& typeObj) { return IdForType<T>(); } // #NOTE Making arg const, reference, etc, requires additional reflecting of T as const, reference, etc

	template <typename T>
	static constexpr MIR_TYPE_ID_TYPE IdForType();

#define VA_ARGS_STRING(...) #__VA_ARGS__
#define VA_ARGS(...) __VA_ARGS__
#define STRIP_PARENTHESES(X) X

#define TYPE_WRAP_STRING(TYPE) STRIP_PARENTHESES( VA_ARGS_STRING TYPE )
#define TYPE_WRAP(TYPE) STRIP_PARENTHESES(VA_ARGS TYPE)

#if _MSC_VER && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL) // Compile option disabled: /Zc:preprocessor
#define MIR_TYPE_ID_IMPL(ID, TYPE)													\
	template <> constexpr MIR_TYPE_ID_TYPE Mirror::IdForType<TYPE>() { return ID; }

#define MIR_TYPE_ID(ID, ...) MIR_TYPE_ID_IMPL(ID, __VA_ARGS__)

#else // Compile option enabled: /Zc:preprocessor

// #TODO IdForType not working in Examples solution, but does in QwerkE as _MSVC_TRADITIONAL is not defined.
// Using GetId() instead as a work around. Maybe better to use long term, but still need to review why Examples can't use IdForType(obj).

#define MIR_TYPE_ID_IMPL(ID, TYPE) \
	template <> constexpr MIR_TYPE_ID_TYPE Mirror::IdForType<TYPE_WRAP(TYPE)>() { return ID; }

#define MIR_TYPE_ID(ID, ...) MIR_TYPE_ID_IMPL(ID, (__VA_ARGS__))

#endif // _MSC_VER && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)

	template<typename... T>
	struct TypesList { };

};
