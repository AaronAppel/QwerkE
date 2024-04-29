#pragma once

#include <map>
#include <string>
#include <vector>

#include "MirrorTypes.h"

#define MIRROR_TO_STR(x) #x

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

// #TODO Fix indentation issues from macros

#define MIRROR_MEMBER_FIELDS_DEFAULT 3
#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

// #NOTE Experimental feature
#define MIRROR_EXPOSE_FIELD_FLAGS 1
#define MIRROR_FIELD_FLAG_SIZE uint8_t

namespace QwerkE {

	struct Mirror
	{
		struct TypeInfo;

#ifdef MIRROR_EXPOSE_FIELD_FLAGS
		enum FieldSerializationFlags : MIRROR_FIELD_FLAG_SIZE
		{
			_None				= 0,
			_HideInInspector	= 1 << 0, // Serialize but don't show in editor UI
			_InspectorOnly		= 1 << 1, // Do not serialize the value
			_InspectorViewOnly	= 1 << 1, // Do not serialize the value or allow changes
		};
#endif
		enum TypeInfoCategories : int8_t
		{
			TypeInfoCategory_Invalid = 0,
			TypeInfoCategory_Primitive,
			TypeInfoCategory_Class,
			TypeInfoCategory_Collection,
			TypeInfoCategory_Pointer,
		};

		struct Iterator
		{
			const void* collectionAddress = nullptr;
			const TypeInfo* typeInfo = nullptr;
			size_t index = 0; // #TODO Private
			void* current = nullptr;

			operator bool() const { return current != nullptr && typeInfo; }
			void operator ++() { if (current != nullptr && typeInfo) current = (char*)current + typeInfo->size; }

			//void* ElementAddress()
			//{
			//	if (!typeInfo) return nullptr;
			//
			//	void* a = typeInfo->typeConstructorFunc;
			//}
		};

		struct Field
		{
			const TypeInfo* typeInfo = nullptr;
			std::string name = "";
			size_t offset = 0; // #TODO Shrink sizes to shorts
			size_t size = 0;
#ifdef MIRROR_EXPOSE_FIELD_FLAGS
			uint8_t serializationFlags = FieldSerializationFlags::_None;
#endif
		};

		using Func_void_voidPtr_sizet_voidPtr_voidPtr = void (*)(void*, size_t, void*, void*);
		using Func_void_voidPtr = void (*)(void*);
		using Func_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);

		struct TypeInfo
		{
			// #TODO Look at shrinking types and using unions for exclusive members
			std::string stringName = "";
			MirrorTypes enumType = MirrorTypes::m_Invalid;
			size_t size = 0;
			size_t arrayCount = 0;

			// #TODO Can create a TypeCategory enum type to switch on instead of using if/else
			bool isPrimitive() const { return enumType > MirrorTypes::m_PRIMITIVES_START; } // { return !isClass; } // { return enumType > MirrorTypes::m_PRIMITIVES_START; }
			bool isSubClass() const { return superTypeInfo != nullptr; }
			bool hasSubClass() const { return !derivedTypesMap.empty(); }
			bool isCollection() const { return isPair() || isArray() || isMap() || isVector() || CollectionTypeInfoFirst() != nullptr || CollectionTypeInfoSecond() != nullptr; }

			// bool isPair() const { return false; }
			bool isPair() const { return enumType > MirrorTypes::m_PAIRS_START && enumType < MirrorTypes::m_PAIRS_END; } // #TODO Deprecate enum dependency
			bool isArray() const { return enumType > MirrorTypes::m_ARRAYS_START && enumType < MirrorTypes::m_ARRAYS_END; } // #TODO Deprecate enum dependency
			bool isMap() const { return enumType > MirrorTypes::m_MAPS_START && enumType < MirrorTypes::m_MAPS_END; } // #TODO Deprecate enum dependency
			bool isVector() const { return enumType > MirrorTypes::m_VECTORS_START && enumType < MirrorTypes::m_VECTORS_END; } // #TODO Deprecate enum dependency

			bool newIsCollection() const { return
				enumType > MirrorTypes::m_PAIRS_START && enumType < MirrorTypes::m_PAIRS_END ||
				enumType > MirrorTypes::m_ARRAYS_START && enumType < MirrorTypes::m_ARRAYS_END ||
				enumType > MirrorTypes::m_VECTORS_START && enumType < MirrorTypes::m_VECTORS_END ||
				enumType > MirrorTypes::m_MAPS_START && enumType < MirrorTypes::m_MAPS_END;
			}

			bool newIsClass() const {
				return isClass && !newIsCollection();
			}

			bool isPointer = false; // #TODO Turn to bit flags
			bool isClass = false;
			bool isAbstract = false;

			std::vector<Field> fields = { }; // #TODO Hide/private non-constants
			std::map<uint16_t, const TypeInfo*> derivedTypesMap;
			const TypeInfo* DerivedTypeInfo(uint16_t typeInfoKey) const {
				if (derivedTypesMap.find(typeInfoKey) == derivedTypesMap.end()) return nullptr;
				return derivedTypesMap.at(typeInfoKey);
			}

			static constexpr int8_t collectionInfoArraySize = 2;
			const TypeInfo* collectionTypeInfo[collectionInfoArraySize] = { nullptr, nullptr };
			const TypeInfo* CollectionTypeInfoFirst() const { return collectionTypeInfo[0]; }
			const TypeInfo* CollectionTypeInfoSecond() const { return collectionTypeInfo[1]; }
			// const TypeInfo* CollectionTypeInfoThird() const { return collectionTypeInfo[2]; }

			const TypeInfo* superTypeInfo = nullptr;
			const TypeInfo* pointerDereferencedTypeInfo = nullptr; // #TODO Can union with superTypeInfo to save space

			const TypeInfo* AbsoluteType() const {
				if (pointerDereferencedTypeInfo) return pointerDereferencedTypeInfo;

				unsigned char buf[2];
				char* tptr = new(buf) char;

				return this;
			}

			// #TODO Look at sizing vector ahead of time to avoid allocations + copies
			// using Func_void_voidPtr_sizet = void (*)(void*, size_t);
			// Func_void_voidPtr_sizet collectionResize;

			void CollectionAppend(void* collectionAddress, size_t index, void* first, void* second = nullptr) const { collectionAddFunc(collectionAddress, index, first, second); }

			Func_void_voidPtr_sizet_voidPtr_voidPtr collectionAddFunc = nullptr;
			Func_void_voidPtr typeConstructorFunc = nullptr;
			Func_charPtr_constVoidPtr_sizet typeIterateCurrentFunc = nullptr;

			TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Invalid;
		};

		template <class T>
		static const TypeInfo* InfoForType(const T& typeObj);

		template<typename T>
		static const TypeInfo* InfoForType();

		template <class T>
		static const TypeInfo* GetSuperClassInfo(); // #TODO Return super pointer
	};

}

template <class TYPE>
static const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType(const TYPE& typeObj)
{
	return Mirror::InfoForType<TYPE>();
}

#define MIRROR_ENUM(ENUM_TYPE) MIRROR_PRIMITIVE_TYPE(ENUM_TYPE) // Nothing special for enums currently. Could use std::is_enum_v<ENUM_TYPE>
#define MIRROR_PRIMITIVE_TYPE(TYPE)																								\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>;																	\
	localStaticTypeInfo.pointerDereferencedTypeInfo = nullptr;																	\
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection();									\
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>;																	\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Primitive;												\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_PRIMITIVE_TYPE_STRING(TYPE)																						\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>;																	\
	localStaticTypeInfo.pointerDereferencedTypeInfo = nullptr;																	\
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection();									\
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>;																	\
	localStaticTypeInfo.typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) TYPE; };		\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Primitive;												\
	return &localStaticTypeInfo;																								\
}

// #TODO Look to combine with MIRROR_TYPE, or at least reduce duplication
#define MIRROR_POINTER(TYPE)																									\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
	static_assert(std::is_pointer_v<TYPE>);																						\
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>;																	\
	localStaticTypeInfo.pointerDereferencedTypeInfo = QwerkE::Mirror::InfoForType<std::remove_pointer_t<TYPE>>();				\
	localStaticTypeInfo.isClass = false;																						\
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>;																	\
	localStaticTypeInfo.typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) TYPE; };		\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Pointer;												\
	return &localStaticTypeInfo;																								\
	}

#define MIRROR_CLASS_START(TYPE) MIRROR_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_CLASS_STARTN(TYPE, FIELDCOUNT)																					\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }													\
	const int fieldsCount = FIELDCOUNT;																							\
	localStaticTypeInfo.fields.reserve(fieldsCount);																			\
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>;																	\
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection();									\
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>;																	\
	localStaticTypeInfo.typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) TYPE(); };	\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Class;													\
																																\
	using ClassType = TYPE;																										\
	enum { BASE = __COUNTER__ };

#define MIRROR_ABSTRACT_CLASS_START(TYPE) MIRROR_ABSTRACT_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_ABSTRACT_CLASS_STARTN(TYPE, FIELDCOUNT)																			\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }													\
	const int fieldsCount = FIELDCOUNT;																							\
	localStaticTypeInfo.fields.reserve(fieldsCount);																			\
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>;																	\
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection();									\
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>;																	\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Class;													\
																																\
	using ClassType = TYPE;																										\
	enum { BASE = __COUNTER__ };

#define MIRROR_DEPENDENT_CLASS_START(TYPE, DEPENDENT_MEMEBER_NAME) \
MIRROR_DEPENDENT_CLASS_STARTN(TYPE, DEPENDENT_MEMEBER_NAME, MIRROR_MEMBER_FIELDS_DEFAULT)

#define MIRROR_DEPENDENT_CLASS_STARTN(TYPE, DEPENDENT_MEMEBER_NAME, FIELDCOUNT)													\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() {															\
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo;																		\
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; }								\
	localStaticTypeInfo.enumType = MirrorTypes::TYPE;																			\
	localStaticTypeInfo.stringName = #TYPE;																						\
	localStaticTypeInfo.size = sizeof(TYPE);																					\
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; }													\
	const int fieldsCount = FIELDCOUNT;																							\
	localStaticTypeInfo.fields.reserve(fieldsCount);																			\
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>;																	\
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection();									\
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>;																	\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Class;													\
																																\
	using ClassType = TYPE;																										\
	enum { BASE = __COUNTER__ };

#define MIRROR_CLASS_SUBCLASS(SUBCLASS_TYPE)																					\
	const QwerkE::Mirror::TypeInfo* SUBCLASS_TYPE##Info = QwerkE::Mirror::InfoForType<SUBCLASS_TYPE>();							\
	localStaticTypeInfo.derivedTypesMap[(uint16_t)MirrorTypes::SUBCLASS_TYPE] = SUBCLASS_TYPE##Info;							\
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo;							\

// #TODO See if using the user type as the key works better.
// Need to change derivedTypesMap to an int most likely, and enum type cast everywhere.
#define MIRROR_CLASS_SUBCLASS_USER_TYPE(SUBCLASS_TYPE, USER_TYPE)																\
	const QwerkE::Mirror::TypeInfo* SUBCLASS_TYPE##Info = QwerkE::Mirror::InfoForType<SUBCLASS_TYPE>();							\
	localStaticTypeInfo.derivedTypesMap[(uint16_t)USER_TYPE] = SUBCLASS_TYPE##Info;												\
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo;							\

#define MIRROR_CLASS_MEMBER(MEMBER_NAME)																						\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																		\
	QwerkE::Mirror::Field MEMBER_NAME##field;																					\
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>();								\
	MEMBER_NAME##field.name = #MEMBER_NAME;																						\
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME);																\
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME));															\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);

#ifdef MIRROR_EXPOSE_FIELD_FLAGS
#define MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, FLAGS)																			\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																		\
	QwerkE::Mirror::Field MEMBER_NAME##field;																					\
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>();								\
	MEMBER_NAME##field.name = #MEMBER_NAME;																						\
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME);																\
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME));															\
	MEMBER_NAME##field.serializationFlags = FLAGS;																				\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);
#endif

#define MIRROR_CLASS_MEMBER_TYPE_OVERRIDE(MEMBER_NAME, OVERRIDE_TYPE)															\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																		\
	QwerkE::Mirror::Field MEMBER_NAME##field;																					\
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<OVERRIDE_TYPE>();													\
	MEMBER_NAME##field.name = #MEMBER_NAME;																						\
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME);																\
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME));															\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);

#define MIRROR_CLASS_MEMBER_ARRAY(MEMBER_NAME, TYPE, ARRAY_SIZE)																\
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 };																		\
	QwerkE::Mirror::Field MEMBER_NAME##field;																					\
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);																	\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = QwerkE::Mirror::InfoForType<TYPE*>();								\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME;															\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);									\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].size = sizeof(decltype(ClassType::MEMBER_NAME));								\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].isArray = true;																\
	localStaticTypeInfo.fields[MEMBER_NAME##Index].arraySize = ARRAY_SIZE;

#define MIRROR_CLASS_END(TYPE)																									\
	if (localStaticTypeInfo.fields.size() >= fieldsCount) { bool warning = true; }												\
	return &localStaticTypeInfo;																								\
}																																\
static const QwerkE::Mirror::TypeInfo* TYPE##typeInfo = QwerkE::Mirror::InfoForType<TYPE>();									\
// Call above initializes field class reference(s). Ideally, remove it and find another init method

// Could use std::is_array_v<ARRAY_TYPE>
#define MIRROR_ARRAY(ARRAY_TYPE, COLLECTION_TYPE)																				\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<ARRAY_TYPE>() {														\
	static TypeInfo localStaticTypeInfo;																						\
	localStaticTypeInfo.stringName = #ARRAY_TYPE;																				\
	localStaticTypeInfo.size = sizeof(ARRAY_TYPE);																				\
	localStaticTypeInfo.arrayCount = sizeof(ARRAY_TYPE) / sizeof(COLLECTION_TYPE);												\
	localStaticTypeInfo.enumType = MirrorTypes::ARRAY_TYPE;																		\
	localStaticTypeInfo.collectionTypeInfo[0] = QwerkE::Mirror::InfoForType<COLLECTION_TYPE>();									\
	localStaticTypeInfo.collectionAddFunc =																						\
		[](void* collectionAddress, size_t index, void* elementFirst, void* /*elementSecond*/) {								\
		memcpy((char*)collectionAddress + (sizeof(COLLECTION_TYPE) * index), elementFirst, sizeof(COLLECTION_TYPE));			\
	};																															\
	localStaticTypeInfo.typeIterateCurrentFunc =																				\
		[](const void* collectionAddress, size_t aIndex) -> char* {																\
		static size_t index = 0;																								\
		if (aIndex < index) index = aIndex;																						\
		if (index >= localStaticTypeInfo.arrayCount) { index = 0; return nullptr; }												\
		return (char*)collectionAddress + (sizeof(COLLECTION_TYPE) * index++);													\
	};																															\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Collection;												\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_VECTOR(VECTOR_TYPE, COLLECTION_TYPE)																				\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<VECTOR_TYPE>() {													\
	static TypeInfo localStaticTypeInfo;																						\
	localStaticTypeInfo.stringName = #VECTOR_TYPE;																				\
	localStaticTypeInfo.size = sizeof(VECTOR_TYPE);																				\
	localStaticTypeInfo.enumType = MirrorTypes::VECTOR_TYPE;																	\
	localStaticTypeInfo.collectionTypeInfo[0] = QwerkE::Mirror::InfoForType<COLLECTION_TYPE>();									\
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
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Collection;												\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_MAP(MAP_TYPE, PAIR_TYPE)																							\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<MAP_TYPE>() {														\
	static TypeInfo localStaticTypeInfo;																						\
	localStaticTypeInfo.stringName = #MAP_TYPE;																					\
	localStaticTypeInfo.size = sizeof(MAP_TYPE);																				\
	localStaticTypeInfo.enumType = MirrorTypes::MAP_TYPE;																		\
	localStaticTypeInfo.collectionTypeInfo[0] = QwerkE::Mirror::InfoForType<PAIR_TYPE>();										\
	localStaticTypeInfo.collectionAddFunc =																						\
		[](void* collectionAddress, size_t /*index*/, void* elementFirst, void* /*elementSecond*/) {							\
		((MAP_TYPE*)collectionAddress)->insert( *(PAIR_TYPE*)elementFirst );													\
	};																															\
	localStaticTypeInfo.typeIterateCurrentFunc =																				\
		[](const void* collectionAddress, size_t aIndex) -> char* {																\
		static size_t index = 0;																								\
		MAP_TYPE* map = (MAP_TYPE*)collectionAddress;																			\
		static MAP_TYPE::iterator iterator = map->begin();																		\
		if (aIndex < index) { index = aIndex; iterator = map->begin(); }														\
		if (aIndex >= map->size()) { return nullptr; }																			\
		++index;																												\
		return (char*)&iterator->first;																							\
	};																															\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Collection;												\
	return &localStaticTypeInfo;																								\
}

#define MIRROR_PAIR(PAIR_TYPE, FIRST_TYPE, SECOND_TYPE)																			\
template<>																														\
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<PAIR_TYPE>() {														\
	static TypeInfo localStaticTypeInfo;																						\
	localStaticTypeInfo.stringName = #PAIR_TYPE;																				\
	localStaticTypeInfo.size = sizeof(PAIR_TYPE);																				\
	localStaticTypeInfo.enumType = MirrorTypes::PAIR_TYPE;																		\
	localStaticTypeInfo.collectionTypeInfo[0] = QwerkE::Mirror::InfoForType<FIRST_TYPE>();										\
	localStaticTypeInfo.collectionTypeInfo[1] = QwerkE::Mirror::InfoForType<SECOND_TYPE>();										\
	localStaticTypeInfo.collectionAddFunc =																						\
		[](void* collectionAddress, size_t /*index*/, void* elementFirst, void* elementSecond) {								\
		((PAIR_TYPE*)collectionAddress)->first = *(FIRST_TYPE*)elementFirst;													\
		((PAIR_TYPE*)collectionAddress)->second = *(SECOND_TYPE*)elementSecond;													\
	};																															\
	localStaticTypeInfo.typeConstructorFunc =																					\
		[](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) PAIR_TYPE; };										\
	localStaticTypeInfo.category = TypeInfoCategories::TypeInfoCategory_Collection;												\
	return &localStaticTypeInfo;																								\
}
