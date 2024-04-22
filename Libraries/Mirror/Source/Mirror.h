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

		struct Field
		{
			const TypeInfo* typeInfo = nullptr;
			std::string name = "";
			size_t offset = 0;
			size_t size = 0;
#ifdef MIRROR_EXPOSE_FIELD_FLAGS
			uint8_t serializationFlags = FieldSerializationFlags::_None;
#endif
		};

		struct TypeInfo
		{
			std::string stringName = "";
			MirrorTypes enumType = MirrorTypes::m_Invalid;
			size_t size = 0;

			bool isPrimitive() const { return enumType > MirrorTypes::m_PRIMITIVES_START; } // { return !isClass; } // { return enumType > MirrorTypes::m_PRIMITIVES_START; }
			bool isSubClass() const { return superTypeInfo != nullptr; }
			bool hasSubClass() const { return !derivedTypesMap.empty(); }
			bool isCollection() const { return isPair() || isArray() || isMap() || isVector() || CollectionTypeInfoFirst() != nullptr || CollectionTypeInfoSecond() != nullptr; }

			bool isPair() const { return false; }
			// bool isPair() const { return enumType > MirrorTypes::m_PAIRS_START && enumType < MirrorTypes::m_PAIRS_END; } // #TODO Deprecate enum dependency
			bool isArray() const { return enumType > MirrorTypes::m_ARRAYS_START && enumType < MirrorTypes::m_ARRAYS_END; } // #TODO Deprecate enum dependency
			bool isMap() const { return enumType > MirrorTypes::m_MAPS_START && enumType < MirrorTypes::m_MAPS_END; } // #TODO Deprecate enum dependency
			bool isVector() const { return false; }
			// bool isVector() const { return enumType > MirrorTypes::m_VECTORS_START && enumType < MirrorTypes::m_VECTORS_END; } // #TODO Deprecate enum dependency

			bool newIsCollection() const { return
				enumType > MirrorTypes::m_PAIRS_START && enumType < MirrorTypes::m_PAIRS_END ||
				enumType > MirrorTypes::m_ARRAYS_START && enumType < MirrorTypes::m_ARRAYS_END ||
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
				return this;
			}
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

#define MIRROR_ENUM(ENUM_TYPE) MIRROR_TYPE(ENUM_TYPE) // Nothing special for enums currently
#define MIRROR_TYPE(TYPE) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() { \
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo; \
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>; \
	localStaticTypeInfo.pointerDereferencedTypeInfo = nullptr; \
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection(); \
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>; \
	return &localStaticTypeInfo; \
}

// #TODO Look to combine with MIRROR_TYPE, or at least reduce duplication
#define MIRROR_POINTER(TYPE) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() { \
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo; \
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>; \
	localStaticTypeInfo.pointerDereferencedTypeInfo = QwerkE::Mirror::InfoForType<std::remove_pointer_t<TYPE>>(); \
	localStaticTypeInfo.isClass = false; \
	localStaticTypeInfo.isAbstract = false; \
	return &localStaticTypeInfo; \
	}

#define MIRROR_CLASS_START(TYPE) MIRROR_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_CLASS_STARTN(TYPE, FIELDCOUNT) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() { \
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo; \
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; } \
	const int fieldsCount = FIELDCOUNT; \
	localStaticTypeInfo.fields.reserve(fieldsCount); \
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>; \
	localStaticTypeInfo.isClass = std::is_class_v<TYPE> && !localStaticTypeInfo.isCollection(); \
	localStaticTypeInfo.isAbstract = std::is_abstract_v<TYPE>; \
 \
	using ClassType = TYPE; \
	enum { BASE = __COUNTER__ };

#define MIRROR_CLASS_SUBCLASS(SUBCLASS_TYPE) \
	const QwerkE::Mirror::TypeInfo* SUBCLASS_TYPE##Info = QwerkE::Mirror::InfoForType<SUBCLASS_TYPE>(); \
	localStaticTypeInfo.derivedTypesMap[MirrorTypes::SUBCLASS_TYPE] = SUBCLASS_TYPE##Info; \
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo; \

// #TODO See if using the user type as the key works better. Need to change derivedTypesMap to an int most likely, and enum type cast everywhere.
#define MIRROR_CLASS_SUBCLASS_USER_TYPE(SUBCLASS_TYPE, USER_TYPE) \
	const QwerkE::Mirror::TypeInfo* SUBCLASS_TYPE##Info = QwerkE::Mirror::InfoForType<SUBCLASS_TYPE>(); \
	localStaticTypeInfo.derivedTypesMap[(uint16_t)USER_TYPE] = SUBCLASS_TYPE##Info; \
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo; \

#define MIRROR_CLASS_MEMBER(MEMBER_NAME) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	QwerkE::Mirror::Field MEMBER_NAME##field; \
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>(); \
	MEMBER_NAME##field.name = #MEMBER_NAME; \
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME); \
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME)); \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);

#ifdef MIRROR_EXPOSE_FIELD_FLAGS
#define MIRROR_CLASS_MEMBER_FLAGS(MEMBER_NAME, FLAGS) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	QwerkE::Mirror::Field MEMBER_NAME##field; \
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>(); \
	MEMBER_NAME##field.name = #MEMBER_NAME; \
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME); \
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME)); \
	MEMBER_NAME##field.serializationFlags = FLAGS; \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);
#endif

#define MIRROR_CLASS_MEMBER_TYPE_OVERRIDE(MEMBER_NAME, OVERRIDE_TYPE) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	QwerkE::Mirror::Field MEMBER_NAME##field; \
	MEMBER_NAME##field.typeInfo = QwerkE::Mirror::InfoForType<OVERRIDE_TYPE>(); \
	MEMBER_NAME##field.name = #MEMBER_NAME; \
	MEMBER_NAME##field.offset = offsetof(ClassType, MEMBER_NAME); \
	MEMBER_NAME##field.size = sizeof(decltype(ClassType::MEMBER_NAME)); \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field);

#define MIRROR_CLASS_MEMBER_ARRAY(MEMBER_NAME, TYPE, ARRAY_SIZE) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	QwerkE::Mirror::Field MEMBER_NAME##field; \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = QwerkE::Mirror::InfoForType<TYPE*>(); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME; \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].size = sizeof(decltype(ClassType::MEMBER_NAME)); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].isArray = true; \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].arraySize = ARRAY_SIZE;

#define MIRROR_CLASS_END(TYPE) \
	if (localStaticTypeInfo.fields.size() >= fieldsCount) { bool warning = true; } \
	return &localStaticTypeInfo; \
} \
static const QwerkE::Mirror::TypeInfo* TYPE##typeInfo = QwerkE::Mirror::InfoForType<TYPE>(); \
// Call above initializes field class reference(s). Ideally, remove it and find another init method

#define MIRROR_ARRAY(ARRAY_TYPE, COLLECTION_TYPE) MIRROR_MAP(ARRAY_TYPE, COLLECTION_TYPE)
#define MIRROR_VECTOR(VECTOR_TYPE, COLLECTION_TYPE) MIRROR_MAP(VECTOR_TYPE, COLLECTION_TYPE)
#define MIRROR_MAP(MAP_TYPE, COLLECTION_TYPE) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<MAP_TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #MAP_TYPE; \
	localStaticTypeInfo.size = sizeof(MAP_TYPE); \
	localStaticTypeInfo.enumType = MirrorTypes::MAP_TYPE; \
	localStaticTypeInfo.collectionTypeInfo[0] = QwerkE::Mirror::InfoForType<COLLECTION_TYPE>(); \
	return &localStaticTypeInfo; \
}

#define MIRROR_PAIR(PAIR_TYPE, FIRST_TYPE, SECOND_TYPE) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<PAIR_TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #PAIR_TYPE; \
	localStaticTypeInfo.size = sizeof(PAIR_TYPE); \
	localStaticTypeInfo.enumType = MirrorTypes::PAIR_TYPE; \
	localStaticTypeInfo.collectionTypeInfo[0] = QwerkE::Mirror::InfoForType<FIRST_TYPE>(); \
	localStaticTypeInfo.collectionTypeInfo[1] = QwerkE::Mirror::InfoForType<SECOND_TYPE>(); \
	return &localStaticTypeInfo; \
}
