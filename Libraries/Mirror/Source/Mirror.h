#pragma once

#include <cassert>
#include <string>

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <type_traits>

#include "MirrorTypes.h"

#define MIRROR_TO_STR(x) #x

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

// #TODO Fix indentation issues from macros

#define MIRROR_MEMBER_FIELDS_DEFAULT 3
#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

#define MIRROR_FIELD_FLAG_SIZE uint8_t // #NOTE Experimental feature
#define MIRROR_TYPE_INFO_CATEGORY_SIZE uint8_t
#define MIRROR_TYPE_SIZE uint16_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX

namespace QwerkE {

	struct Mirror
	{
		struct TypeInfo;

		enum FieldSerializationFlags : MIRROR_FIELD_FLAG_SIZE
		{
			_None				= 0,
			_HideInInspector	= 1 << 0, // Serialize but don't show in editor UI
			_InspectorOnly		= 1 << 1, // Do not serialize the value
			_InspectorViewOnly	= 1 << 1, // Do not serialize the value or allow changes
		};

		enum TypeInfoCategories : MIRROR_TYPE_INFO_CATEGORY_SIZE
		{
			TypeInfoCategory_Primitive = 0,
			TypeInfoCategory_Class,
			TypeInfoCategory_Collection,
			TypeInfoCategory_Pair,
			TypeInfoCategory_Pointer,
		};

		struct Field
		{
			const TypeInfo* typeInfo = nullptr;
			std::string name = "";
			size_t offset = 0; // #TODO Shrink size (uint16_t) and add static assert
			MIRROR_TYPE_SIZE size = 0;
			MIRROR_FIELD_FLAG_SIZE serializationFlags = FieldSerializationFlags::_None;
		};

		struct TypeInfo
		{
			// #TODO Look at shrinking types and using unions for exclusive members
			// #TODO Hide/private non-constants

			std::string stringName = "";
			MirrorTypes enumType = MirrorTypes::m_Invalid;
			MIRROR_TYPE_SIZE size = 0;
			TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

			std::vector<Field> fields = { };
			std::vector<const TypeInfo*> derivedTypes;

			const TypeInfo* collectionTypeInfoFirst = nullptr;
			const TypeInfo* collectionTypeInfoSecond = nullptr;
			const TypeInfo* superTypeInfo = nullptr;
			const TypeInfo* pointerDereferencedTypeInfo = nullptr;

			const TypeInfo* AbsoluteType() const { return pointerDereferencedTypeInfo ? pointerDereferencedTypeInfo : this; }
			const TypeInfo* DerivedTypeFromPointer(const void* instanceAddress) const
			{
				if (derivedTypes.empty()) return this;
				for (const auto& derivedType : derivedTypes)
				{
					assert(derivedType->typeDynamicCastFunc, "Null typeDynamicCastFunc!");
					if (derivedType->typeDynamicCastFunc(instanceAddress))
					{
						return derivedType;
					}
				}
				return this;
			}
			const TypeInfo* DerivedTypeFromTypeName(const std::string& typeName) const
			{
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

			void CollectionAppend(void* collectionAddress, size_t index, void* first, void* second = nullptr) const
			{
				assert(collectionAddFunc, "Null collectionAddFunc!");
				collectionAddFunc(collectionAddress, index, first, second);
			}

			void Construct(void* instanceAddress) const
			{
				if (typeConstructorFunc)
				{
					typeConstructorFunc(instanceAddress);
				}
			}

			using Func_void_voidPtr_sizet_voidPtr_voidPtr = void (*)(void*, size_t, void*, void*);
			using Func_void_voidPtr = void (*)(void*);
			using Func_voidPtr_constVoidPtr_bool = void* (*)(const void*, bool);
			using Func_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
			using Func_bool_constVoidPtr = bool (*)(const void*);
			// using Func_void_voidPtr_voidPtr = void (*)(void*, void*);

			Func_void_voidPtr_sizet_voidPtr_voidPtr collectionAddFunc = nullptr;
			Func_voidPtr_constVoidPtr_bool collectionFirstSecondFunc = nullptr;
			Func_charPtr_constVoidPtr_sizet typeIterateCurrentFunc = nullptr;
			Func_bool_constVoidPtr typeDynamicCastFunc = nullptr;

			Func_void_voidPtr typeConstructorFunc = nullptr; // #TODO Look to hold only 1 construction lambda
		};

		template <class T>
		static const TypeInfo* InfoForType(const T& typeObj);

		template<typename T>
		static const TypeInfo* InfoForType();
	};

	template<typename... T>
	struct MirrorTemplateArgumentList { };

	template <typename Super, typename... SubClass>
	void todo_MirrorSubClassUserType(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
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
	static void todo_MirrorSubClassUserTypes(MirrorTemplateArgumentList<T...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
	{
		MirrorSubClassUserType<Super, T...>(localStaticTypeInfo, enumStartOffset);
	}

}

// Start STL type deduction
// https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859
// Specialize a type for all of the STL containers.
namespace is_stl_container_impl {
	template <typename T>       struct is_stl_container :std::false_type {};
	template <typename T, std::size_t N> struct is_stl_container<std::array    <T, N>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::vector            <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::deque             <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::list              <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::forward_list      <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::set               <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::multiset          <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::map               <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::multimap          <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::stack             <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::queue             <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>> :std::true_type {};
}

namespace is_stl_pair_impl {
	template <typename T>       struct is_stl_pair :std::false_type {};
	template <typename... Args> struct is_stl_pair<std::pair <Args...>> :std::true_type {};
}

// Type trait to utilize the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
	static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};
template <typename T> struct is_stl_pair {
	static constexpr bool const value = is_stl_pair_impl::is_stl_pair<std::decay_t<T>>::value;
};
// End STL type deduction

template <class TYPE>
static const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType(const TYPE& typeObj)
{
	return Mirror::InfoForType<TYPE>();
}

template <typename T>
constexpr QwerkE::Mirror::TypeInfoCategories SetCategory()
{
	// #TODO Review special cases
	if (std::is_same_v<T, std::string>) return QwerkE::Mirror::TypeInfoCategory_Primitive;
	if (std::is_same_v<T, const char*>) return QwerkE::Mirror::TypeInfoCategory_Primitive;
	if (std::is_enum_v<T>) return QwerkE::Mirror::TypeInfoCategory_Primitive; // #REVIEW Extra

	if (std::is_array_v<T>) return QwerkE::Mirror::TypeInfoCategory_Collection;
	if (std::is_pointer_v<T>) return QwerkE::Mirror::TypeInfoCategory_Pointer;
	if (is_stl_pair<T>::value) return QwerkE::Mirror::TypeInfoCategory_Pair;
	if (is_stl_container<T>::value) return QwerkE::Mirror::TypeInfoCategory_Collection;
	if (std::is_class_v<T>) return QwerkE::Mirror::TypeInfoCategory_Class;
	else return QwerkE::Mirror::TypeInfoCategory_Primitive;
}

#define MIRROR_GENERIC(TYPE)																									\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static_assert(sizeof(TYPE) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");								\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.category = SetCategory<TYPE>();																			\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
	using ClassType = TYPE;	/* For use by other macros */

#define MIRROR_TYPE(TYPE)																										\
	MIRROR_GENERIC(TYPE)																										\
	return &localStaticTypeInfo;																								\
}

// #TODO QC_ENUM() is actually a class so category needs overriding
#define MIRROR_ENUM(ENUM_TYPE)																									\
	MIRROR_GENERIC(ENUM_TYPE)																									\
	localStaticTypeInfo.category = QwerkE::Mirror::TypeInfoCategory_Primitive;													\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_POINTER(TYPE)																									\
	MIRROR_GENERIC(TYPE)																										\
	static_assert(std::is_pointer_v<TYPE>);																						\
	localStaticTypeInfo.pointerDereferencedTypeInfo = QwerkE::Mirror::InfoForType<std::remove_pointer_t<TYPE>>();				\
	return &localStaticTypeInfo;																								\
}

// #TODO Keep logic here. Check if this class needs it's own member to construct
#define MIRROR_CONSTRUCT_USING_MEMBER(MEMBER_NAME)																				\
	localStaticTypeInfo.typeConstructorFunc = [](void* instanceAddress) {														\
		using MemberType = decltype(ClassType::MEMBER_NAME);																	\
		char* memberAddress = (char*)instanceAddress + offsetof(ClassType, MEMBER_NAME);										\
		new(instanceAddress) ClassType(*(MemberType*)memberAddress);															\
	};

#define MIRROR_CLASS_START(TYPE) MIRROR_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_CLASS_STARTN(TYPE, FIELDCOUNT)																					\
	MIRROR_GENERIC(TYPE)																										\
	static_assert(std::is_class_v<TYPE> && !std::is_abstract_v<TYPE>);															\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }													\
	const int fieldsCount = FIELDCOUNT;																							\
	localStaticTypeInfo.fields.reserve(fieldsCount);																			\
	localStaticTypeInfo.typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) TYPE(); };	\
	enum { BASE = __COUNTER__ };

#define MIRROR_ABSTRACT_CLASS_START(TYPE) MIRROR_ABSTRACT_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_ABSTRACT_CLASS_STARTN(TYPE, FIELDCOUNT)																			\
	MIRROR_GENERIC(TYPE)																										\
	static_assert(std::is_class_v<TYPE> && std::is_abstract_v<TYPE>);															\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }													\
	const int fieldsCount = FIELDCOUNT;																							\
	localStaticTypeInfo.fields.reserve(fieldsCount);																			\
	enum { BASE = __COUNTER__ };

#define MIRROR_DEPENDENT_CLASS_START(TYPE, DEPENDENT_MEMEBER_NAME) \
MIRROR_DEPENDENT_CLASS_STARTN(TYPE, DEPENDENT_MEMEBER_NAME, MIRROR_MEMBER_FIELDS_DEFAULT)

#define MIRROR_DEPENDENT_CLASS_STARTN(TYPE, DEPENDENT_MEMEBER_NAME, FIELDCOUNT)													\
	MIRROR_GENERIC(TYPE)																										\
	static_assert(std::is_class_v<TYPE> && !std::is_abstract_v<TYPE>);															\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }													\
	const int fieldsCount = FIELDCOUNT;																							\
	localStaticTypeInfo.fields.reserve(fieldsCount);																			\
	enum { BASE = __COUNTER__ };

#define MIRROR_SUBCLASS(SUBCLASS_TYPE)																					\
	const QwerkE::Mirror::TypeInfo* SUBCLASS_TYPE##Info = QwerkE::Mirror::InfoForType<SUBCLASS_TYPE>();							\
	localStaticTypeInfo.derivedTypes.push_back(SUBCLASS_TYPE##Info);															\
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo;							\
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->typeDynamicCastFunc =											\
	[](const void* pointerToInstance) -> bool {																					\
		return dynamic_cast<SUBCLASS_TYPE*>(*(ClassType**)pointerToInstance) != nullptr;										\
	};																															\

#define MIRROR_CLASS_MEMBER(MEMBER_NAME)																						\
	MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, 0)

#define MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, FLAGS)																			\
	static_assert(sizeof(decltype(ClassType::MEMBER_NAME)) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");	\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																		\
	QwerkE::Mirror::Field MEMBER_NAME##field;																					\
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>();								\
	MEMBER_NAME##field.name = #MEMBER_NAME;																						\
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME);																\
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME));															\
	MEMBER_NAME##field.serializationFlags = FLAGS;																				\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);

// #TODO Deprecate. Currently used for m_imvec4_array
#define MIRROR_CLASS_MEMBER_TYPE_OVERRIDE(MEMBER_NAME, OVERRIDE_TYPE)															\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																		\
	QwerkE::Mirror::Field MEMBER_NAME##field;																					\
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<OVERRIDE_TYPE>();													\
	MEMBER_NAME##field.name = #MEMBER_NAME;																						\
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME);																\
	static_assert(sizeof(decltype(ClassType::MEMBER_NAME)) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");	\
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME));															\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);

#define MIRROR_CLASS_END(TYPE)																									\
	if (localStaticTypeInfo.fields.size() >= fieldsCount) { bool warning = true; }												\
	return &localStaticTypeInfo;																								\
}																																\
static const QwerkE::Mirror::TypeInfo* TYPE##typeInfo = QwerkE::Mirror::InfoForType<TYPE>();									\
// Instance above used for intialization. Ideally, remove it and find better alternative

// Could use std::is_array_v<ARRAY_TYPE>
#define MIRROR_ARRAY(ARRAY_TYPE, COLLECTION_TYPE)																				\
	MIRROR_GENERIC(ARRAY_TYPE)																									\
	static_assert(std::is_array_v<ARRAY_TYPE>);																					\
	localStaticTypeInfo.collectionTypeInfoFirst = QwerkE::Mirror::InfoForType<COLLECTION_TYPE>();								\
	localStaticTypeInfo.collectionAddFunc =																						\
		[](void* collectionAddress, size_t index, void* elementFirst, void* /*elementSecond*/) {								\
		memcpy((char*)collectionAddress + (sizeof(COLLECTION_TYPE) * index), elementFirst, sizeof(COLLECTION_TYPE));			\
	};																															\
	localStaticTypeInfo.typeIterateCurrentFunc =																				\
		[](const void* collectionAddress, size_t aIndex) -> char* {																\
		static size_t index = 0;																								\
		if (aIndex < index) index = aIndex;																						\
		if (index >= sizeof(ARRAY_TYPE) / sizeof(COLLECTION_TYPE)) { index = 0; return nullptr; }								\
		return (char*)collectionAddress + (sizeof(COLLECTION_TYPE) * index++);													\
	};																															\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_VECTOR(VECTOR_TYPE, COLLECTION_TYPE)																				\
	MIRROR_GENERIC(VECTOR_TYPE)																									\
	static_assert(is_stl_container<VECTOR_TYPE>::value);																		\
	localStaticTypeInfo.collectionTypeInfoFirst = QwerkE::Mirror::InfoForType<COLLECTION_TYPE>();								\
	localStaticTypeInfo.collectionAddFunc =																						\
	[](void* collectionAddress, size_t /*index*/, void* elementFirst, void* /*elementSecond*/) {								\
		((VECTOR_TYPE*)collectionAddress)->push_back(*(COLLECTION_TYPE*)elementFirst);											\
	};																															\
	localStaticTypeInfo.typeIterateCurrentFunc =																				\
		[](const void* collectionAddress, size_t aIndex) -> char* {																\
		static size_t index = 0;																								\
		VECTOR_TYPE* vec = ((VECTOR_TYPE*)collectionAddress);																	\
		if (aIndex < index) index = aIndex;																						\
		if (index >= vec->size()) { index = 0; return nullptr; }																\
		return (char*)vec->data() + (sizeof(COLLECTION_TYPE) * index++);														\
	};																															\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_MAP(MAP_TYPE, PAIR_TYPE)																							\
	MIRROR_GENERIC(MAP_TYPE)																									\
	static_assert(is_stl_container<MAP_TYPE>::value);																			\
	localStaticTypeInfo.collectionTypeInfoFirst = QwerkE::Mirror::InfoForType<PAIR_TYPE>();										\
	localStaticTypeInfo.collectionAddFunc =																						\
		[](void* collectionAddress, size_t /*index*/, void* elementFirst, void* /*elementSecond*/) {							\
		((MAP_TYPE*)collectionAddress)->insert( *(PAIR_TYPE*)elementFirst );													\
	};																															\
	localStaticTypeInfo.typeIterateCurrentFunc =																				\
		[](const void* collectionAddress, size_t aIndex) -> char* {																\
		static size_t index = 0;																								\
		MAP_TYPE* map = (MAP_TYPE*)collectionAddress;																			\
		static MAP_TYPE::iterator iterator = map->begin();																		\
		if (aIndex < index || map->end() == iterator)																			\
		{																														\
			index = aIndex;																										\
			iterator = map->begin();																							\
		}																														\
		if (index >= map->size()) { ++index; return nullptr; }																	\
		++index;																												\
		auto result = (char*)&iterator->first;																					\
		++iterator;																												\
		return result;																											\
	};																															\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_PAIR(PAIR_TYPE)																									\
	MIRROR_GENERIC(PAIR_TYPE)																									\
	static_assert(is_stl_pair<PAIR_TYPE>::value);																				\
	localStaticTypeInfo.collectionTypeInfoFirst = QwerkE::Mirror::InfoForType<PAIR_TYPE::first_type>();							\
	localStaticTypeInfo.collectionTypeInfoSecond = QwerkE::Mirror::InfoForType<PAIR_TYPE::second_type>();						\
	localStaticTypeInfo.collectionAddFunc =																						\
		[](void* collectionAddress, size_t /*index*/, void* elementFirst, void* elementSecond) {								\
		localStaticTypeInfo.collectionTypeInfoFirst->Construct(&((PAIR_TYPE*)collectionAddress)->first);						\
		((PAIR_TYPE*)collectionAddress)->first = *(PAIR_TYPE::first_type*)elementFirst;											\
		localStaticTypeInfo.collectionTypeInfoSecond->Construct(&((PAIR_TYPE*)collectionAddress)->second);						\
		((PAIR_TYPE*)collectionAddress)->second = *(PAIR_TYPE::second_type*)elementSecond;										\
	};																															\
	localStaticTypeInfo.typeConstructorFunc =																					\
		[](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) PAIR_TYPE; };										\
	localStaticTypeInfo.collectionFirstSecondFunc =																				\
		[](const void* pairObjAddress, bool isFirst) -> void* {																	\
		if (isFirst) return &(((PAIR_TYPE*)pairObjAddress)->first);																\
		return &(((PAIR_TYPE*)pairObjAddress)->second);																			\
	};																															\
	return &localStaticTypeInfo;																								\
}
