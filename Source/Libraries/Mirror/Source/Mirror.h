#pragma once

#include <typeinfo> // For typeid(x)

#include "Structs.h"
#include "TypeDeduction.h"

#include "TypeIds.h"

#define MIRROR_TO_STR(x) #x

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

// #TODO Fix indentation issues from macros

template <typename T>
constexpr Mirror::TypeInfoCategories GetCategory() {
	// #TODO Review using std::is_compound, std::is_reference

	// #TODO Review if possible special cases exist, and proper ordering
	if (std::is_same_v<T, std::string>) return Mirror::TypeInfoCategory_Primitive;
	if (std::is_same_v<T, const char*>) return Mirror::TypeInfoCategory_Primitive;
	// if (!std::is_pointer_v<T> && !std::is_class_v<T>) return Mirror::TypeInfoCategory_Primitive; // #TODO Replace above checks
	if (std::is_enum_v<T>) return Mirror::TypeInfoCategory_Primitive; // #REVIEW Needed or useful?

	if (std::is_array_v<T>) return Mirror::TypeInfoCategory_Collection;
	if (std::is_pointer_v<T>) return Mirror::TypeInfoCategory_Pointer;

	if (is_stl_pair<T>::value) return Mirror::TypeInfoCategory_Pair;
	if (is_stl_container<T>::value) return Mirror::TypeInfoCategory_Collection;
	if (std::is_class_v<T>) return Mirror::TypeInfoCategory_Class;
	else return Mirror::TypeInfoCategory_Primitive;
}

template <class TYPE>
static const Mirror::TypeInfo* Mirror::InfoForType(const TYPE& typeObj) {
	return Mirror::InfoForType<TYPE>();
}

template<typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(std::is_class_v<T> || std::is_same_v<T, std::string>);

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
}

template<typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_vector_impl::is_stl_vector<T>::type());

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>(); // #TODO Same as Map
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->push_back(*(T::value_type*)elementFirst); // #TODO Similar to Map
		};
	mutableTypeInfo->collectionClearFunction = [](void* collectionAddress) {
		((T*)collectionAddress)->clear(); // #TODO Same as Map
		};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0;
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
	static_assert(is_stl_map<T>::value);

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>(); // #TODO Same as Vector
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->insert(*(T::value_type*)elementFirst); // #TODO Similar to Vector
		};
	mutableTypeInfo->collectionClearFunction = [](void* collectionAddress) {
		((T*)collectionAddress)->clear(); // #TODO Same as Vector
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
	static_assert(is_stl_pair<T>::value);

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::first_type>();
	mutableTypeInfo->collectionTypeInfoSecond = Mirror::InfoForType<T::second_type>();

	mutableTypeInfo->collectionAddFunc = [](void* pairObjAddress, size_t /*index*/, const void* elementFirst, const void* elementSecond) {
		T* pair = (T*)pairObjAddress;
		memcpy((void*)&pair->first, elementFirst, sizeof(T::first_type));
		memcpy((void*)&pair->second, elementSecond, sizeof(T::second_type));
		};
	mutableTypeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
	mutableTypeInfo->collectionAddressOfPairObjectFunc = [](const void* pairObjAddress, bool isFirst) -> void* {
		T* pair = (T*)pairObjAddress;
		if (isFirst) return (void*)&pair->first;
		return (void*)&pair->second;
		};
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
			if (index >= sizeof(ArrayElementType) / sizeof(ArrayElementType)) { index = 0; return nullptr; }
			return (char*)collectionAddress + (sizeof(ArrayElementType) * index++);
			};
	}
	else if (is_stl_pair<T>::value)
	{
		SetCollectionLambdasPair<T>(mutableTypeInfo, is_stl_pair_impl::is_stl_pair<T>::type());
	}
}

template<typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_container<T>::value);
	// #TODO Look into std::remove_const_t<T>;

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	typedef typename std::remove_all_extents<T>::type CollectionElementTypeFirst;
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<CollectionElementTypeFirst>();

	// #TODO Look at how to support all types without manual implementations
	SetCollectionLambdasVector<T>(mutableTypeInfo, is_stl_vector_impl::is_stl_vector<T>::type());
	SetCollectionLambdasMap<T>(mutableTypeInfo, is_stl_map_impl::is_stl_map<T>::type());
}

#define PREDEFINED_ID_MAX 128
constexpr std::size_t HashFromString(const char* type_name) { // #TODO Improve naive pseudo-unique output implementation
	std::size_t result = PREDEFINED_ID_MAX + 1;
	const char* charPtr = type_name;
	while (*charPtr != '\0')
	{
		result += *charPtr;
		charPtr += 1;
	}
	return result;
}

// #NOTE Can't be a templated function because specialization will be automatically generated. That seems good,
// but if the auto-generated/default specialization isn't correct, any other attempts at specialization for the same type may be
// seen/compiled too late and the incorrect version is used, or a multiply defined error may occur
#define MIRROR_COMMON(TYPE)																													\
template<>																																	\
static const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() {																				\
	static_assert(sizeof(TYPE) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");											\
	static Mirror::TypeInfo localStaticTypeInfo;																							\
																																			\
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; }															\
																																			\
	localStaticTypeInfo.category = GetCategory<TYPE>();																						\
	localStaticTypeInfo.stringName = #TYPE;																									\
	localStaticTypeInfo.id = Mirror::TypeId<TYPE>();																						\
																																			\
	localStaticTypeInfo.size = sizeof(TYPE);																								\

// #NOTE Using __VA_ARGS__ to handle macro calls with commas like MIRROR_INFO_FOR_TYPE(std::map<int, bool>)
#define MIRROR_TYPE(...) MIRROR_TYPE_IMPL(__VA_ARGS__)
#define MIRROR_TYPE_IMPL(TYPE)																												\
MIRROR_COMMON(TYPE)																															\
																																			\
	switch (localStaticTypeInfo.category)																									\
	{																																		\
	case TypeInfoCategory_Collection:																										\
	case TypeInfoCategory_Pair:																												\
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

#define MIRROR_MEMBER_FIELDS_DEFAULT 3

#define MIRROR_CLASS(TYPE)																													\
MIRROR_COMMON(TYPE)																															\
																																			\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }																\
	const int fieldsCount = MIRROR_MEMBER_FIELDS_DEFAULT;																					\
	localStaticTypeInfo.fields.reserve(fieldsCount);																						\
																																			\
	using ClassType = TYPE;																													\
	enum { BASE = __COUNTER__ };

#define MIRROR_CLASS_MEMBER(MEMBER_NAME)  MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, 0)
#define MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, FLAGS)																						\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																					\
	Mirror::Field MEMBER_NAME##field;																										\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);																				\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>();						\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME;																		\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);												\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].flags = FLAGS;

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
	return &localStaticTypeInfo;																											\
}
