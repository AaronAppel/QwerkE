#pragma once

// See README.md for documentation

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

#include "MIR_Structs.h"
#include "MIR_TypeDeduction.h"
#include "MIR_TypeIds.h"

template <typename T>
constexpr Mirror::TypeInfoCategories GetCategory() {

	static_assert(!std::is_reference_v<T>, "Reference(s) currently unsupported");
	static_assert(!std::is_function_v<T> || !std::is_function_v<std::remove_pointer_t<T>>,  "Function object(s) and function pointer(s) currently unsupported");

	if (std::is_enum_v<T>) return Mirror::TypeInfoCategory_Primitive; // #NOTE Should come before class in case of enum class

	// #TODO Review if (std::is_array_v<T>) return Mirror::TypeInfoCategory_Collection;
	if (std::is_array_v<T>) return Mirror::TypeInfoCategory_Collection;
	if (std::is_pointer_v<T>) return Mirror::TypeInfoCategory_Pointer;

	if (is_stl_container<T>::value) return Mirror::TypeInfoCategory_Collection;
	if (std::is_class_v<T>) return Mirror::TypeInfoCategory_Class; // #NOTE Should come after container/collection check as stl containers are classes

	MIRROR_ASSERT(false && "Unsupported type found!");

	return Mirror::TypeInfoCategory_Primitive;
}

template <class TYPE>
static const Mirror::TypeInfo* Mirror::InfoForType(const TYPE& typeObj) {
	return Mirror::InfoForType<TYPE>();
}

template<typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(std::is_class_v<T> || std::is_same_v<T, std::string>, "Type T is not a class!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
}

template<typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_vector_impl::is_stl_vector<T>::type(), "Type T is not a vector!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->emplace_back(*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0; // #TODO Support iterating backwards over a collection, and random access
		T* vector = ((T*)collectionAddress);
		if (aIndex < index) index = aIndex;
		if (index >= vector->size()) { index = 0; return nullptr; }
		return (char*)vector->data() + (sizeof(T::value_type) * index++);
	};
}

template<typename T>
static void SetCollectionLambdasMap(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasMap(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_map<T>::value, "Type T is not a map!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->insert(*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0;
		T* map = (T*)collectionAddress;
		static T::iterator iterator = map->begin();
		if (aIndex < index || map->end() == iterator)
		{
			index = aIndex;
			iterator = map->begin();
		}
		if (index >= map->size()) { ++index; return nullptr; }
		++index;
		auto result = (char*)&iterator->first;
		++iterator;
		return result;
	};
}

template<typename T>
static void SetCollectionLambdasPair(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasPair(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_pair<T>::value, "Type T is not a pair!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::first_type>();
	mutableTypeInfo->collectionTypeInfoSecond = Mirror::InfoForType<T::second_type>();

	mutableTypeInfo->collectionAddFunc = [](void* pairObjAddress, size_t /*index*/, const void* elementFirst, const void* elementSecond) {
		T* pair = (T*)pairObjAddress;
		memcpy((void*)&pair->first, elementFirst, sizeof(T::first_type));
		memcpy((void*)&pair->second, elementSecond, sizeof(T::second_type));
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* pairObjAddress, size_t aIndex) -> char* {
		T* pair = (T*)pairObjAddress;
		if (0 == aIndex) return (char*)&pair->first;
		else if (1 == aIndex) return (char*)&pair->second;
		else return nullptr;
	};
	mutableTypeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
}

template<typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::false_type) {
	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	if (std::is_array_v<T>)
	{
		typedef typename std::remove_all_extents<T>::type ArrayElementType;
		mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<ArrayElementType>();
		mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t index, const void* elementFirst, const void* /*elementSecond*/) {
			memcpy((char*)collectionAddress + (sizeof(ArrayElementType) * index), elementFirst, sizeof(ArrayElementType));
		};
		mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
			static size_t index = 0;
			if (aIndex < index) index = aIndex;
			if (index >= sizeof(T) / sizeof(ArrayElementType)) { index = 0; return nullptr; }
			return (char*)collectionAddress + (sizeof(ArrayElementType) * index++);
		};
	}
}

template<typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_container<T>::value, "Type T is not a collection!");
	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	typedef typename std::remove_all_extents<T>::type CollectionElementTypeFirst;
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<CollectionElementTypeFirst>();

	// #TODO Review how std::array is handled
	// SetCollectionLambdasArray<T>(mutableTypeInfo, is_stl_vector_impl::is_stl_array<T>::type());
	SetCollectionLambdasVector<T>(mutableTypeInfo, is_stl_vector_impl::is_stl_vector<T>::type());
	SetCollectionLambdasMap<T>(mutableTypeInfo, is_stl_map_impl::is_stl_map<T>::type());
	SetCollectionLambdasPair<T>(mutableTypeInfo, is_stl_pair_impl::is_stl_pair<T>::type());
}

// #NOTE Required to avoid sizeof(void) C2070 compiler error
#define MIRROR_TYPE_NON_VOID(TYPE)																											\
static_assert(sizeof(TYPE) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");												\
localStaticTypeInfo.size = sizeof(TYPE);

// #NOTE Must be a macro to avoid default specialization issues causing "multiply defined..." errors dependent on order of compilation.
#define MIRROR_TYPE_COMMON(TYPE)																											\
template<>																																	\
static const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() {																				\
	static Mirror::TypeInfo localStaticTypeInfo;																							\
																																			\
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; }															\
																																			\
	localStaticTypeInfo.category = GetCategory<TYPE>();																						\
	localStaticTypeInfo.stringName = #TYPE;																									\
	localStaticTypeInfo.id = Mirror::IdForType<TYPE>();																						\

// #NOTE Using __VA_ARGS__ to handle macro calls with comma(s) ',' like MIRROR_INFO_FOR_TYPE(std::map<int, bool>)
// #NOTE Below switch falthrough compiler warning 26819 cannot be handled within this macro
#define MIRROR_TYPE(...) MIRROR_TYPE_IMPL(__VA_ARGS__)
#define MIRROR_TYPE_IMPL(TYPE)																												\
MIRROR_TYPE_COMMON(TYPE)																													\
MIRROR_TYPE_NON_VOID(TYPE)																													\
																																			\
	switch (localStaticTypeInfo.category)																									\
	{																																		\
	case TypeInfoCategory_Collection: /* #NOTE Intentional case fall through as a collection is also a class */								\
		SetCollectionLambdas<TYPE>(&localStaticTypeInfo, is_stl_container_impl::is_stl_container<TYPE>::type());							\
	case TypeInfoCategory_Class:																											\
		SetConstructionLambda<TYPE>(&localStaticTypeInfo, std::is_class<TYPE>::type());														\
		break;																																\
																																			\
	case TypeInfoCategory_Pointer:																											\
		localStaticTypeInfo.pointerDereferencedTypeInfo = Mirror::InfoForType<std::remove_pointer_t<TYPE>>();								\
		break;																																\
																																			\
	case TypeInfoCategory_Primitive:																										\
		SetConstructionLambda<TYPE>(&localStaticTypeInfo, std::is_same<TYPE, std::string>::type());											\
		break;																																\
	}																																		\
																																			\
	return &localStaticTypeInfo;																											\
}

// #NOTE Avoids sizeof(void) C2070 compile error
#define MIRROR_TYPE_VOID(TYPE)																												\
MIRROR_TYPE_COMMON(TYPE)																													\
	return &localStaticTypeInfo;																											\
}

#ifndef MIRROR_MEMBER_FIELDS_COUNT_DEFAULT
#define MIRROR_MEMBER_FIELDS_COUNT_DEFAULT 3
#endif

#define MIRROR_CLASS(TYPE)																													\
MIRROR_TYPE_COMMON(TYPE)																													\
MIRROR_TYPE_NON_VOID(TYPE)																													\
																																			\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }																\
	const int fieldsCount = MIRROR_MEMBER_FIELDS_COUNT_DEFAULT;																				\
	localStaticTypeInfo.fields.reserve(fieldsCount);																						\
																																			\
	using ClassType = TYPE;																													\
	enum { BASE = __COUNTER__ };

#ifndef MIRROR_OMIT_FLAGS
#define MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, FLAGS) MIRROR_CLASS_MEMBER_IMPL(MEMBER_NAME, FLAGS)
#define MIRROR_CLASS_MEMBER_FLAGS_IMPL(MEMBER_NAME, FLAGS)																					\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].flags = FLAGS;
#else
#define MIRROR_CLASS_MEMBER_FLAGS_IMPL(MEMBER_NAME, FLAGS)
#endif // !MIRROR_OMIT_FLAGS

// #TODO Consider user exposed macros, etc, to have shorter or nicer (MirClass(), MirType(), MirId(), etc) names?
#define MIRROR_CLASS_MEMBER(MEMBER_NAME)  MIRROR_CLASS_MEMBER_IMPL(MEMBER_NAME, 0)
#define MIRROR_CLASS_MEMBER_IMPL(MEMBER_NAME, FLAGS)																						\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																					\
	Mirror::Field MEMBER_NAME##field;																										\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);																				\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>();						\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME;																		\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);												\
	MIRROR_CLASS_MEMBER_FLAGS_IMPL(MEMBER_NAME, FLAGS)

#define MIRROR_CONSTRUCT_USING_MEMBER(MEMBER_NAME)																							\
	localStaticTypeInfo.typeConstructorFunc = [](void* instanceAddress) {																	\
		using MemberType = decltype(ClassType::MEMBER_NAME);																				\
		char* memberAddress = (char*)instanceAddress + offsetof(ClassType, MEMBER_NAME);													\
		new(instanceAddress) ClassType(*(MemberType*)memberAddress);																		\
	};

#define MIRROR_CLASS_SUBCLASS(SUBCLASS_TYPE)																								\
	const Mirror::TypeInfo* SUBCLASS_TYPE##Info = Mirror::InfoForType<SUBCLASS_TYPE>();														\
	localStaticTypeInfo.derivedTypes.push_back(SUBCLASS_TYPE##Info);																		\
	const_cast<Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo;												\

#define MIRROR_CLASS_END																													\
	/* #TODO Sanity checks: if (collection) { assert(collectionTypeInfoFirst) }, etc  */													\
	return &localStaticTypeInfo;																											\
}
