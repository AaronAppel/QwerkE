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

namespace QwerkE
{

	struct Mirror
	{
		struct TypeInfo;

		struct Field
		{
			const TypeInfo* typeInfo = nullptr;
			std::string name = "";
			size_t offset = 0;
		};

		struct TypeInfo
		{
			std::string stringName = "";
			MirrorTypes enumType = MirrorTypes::m_Invalid;
			size_t size = 0;

			bool isPrimitive() const { return enumType > MirrorTypes::m_PRIMITIVES_START; }
			bool isSubClass() const { return superTypeInfo != nullptr; }
			bool hasSubClass() const { return !derivedTypesMap.empty(); }
			bool isCollection() const { return collectionTypeInfo != nullptr; }
			bool isPointer = false;

			std::vector<Field> fields = { }; // #TODO Hide/private non-constants
			std::map<MirrorTypes, const TypeInfo*> derivedTypesMap;

			const TypeInfo* collectionTypeInfo = nullptr;
			const TypeInfo* superTypeInfo = nullptr;
		};

		template <class T>
		static const TypeInfo* InfoForType(const T& typeObj);

		template<typename T>
		static const TypeInfo* InfoForType();

		template <class T>
		static const TypeInfo* GetParentClassInfo();
	};

}

template <class TYPE>
static const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType(const TYPE& typeObj)
{
	return Mirror::InfoForType<TYPE>();
}

#define MIRROR_TYPE(TYPE) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() { \
	static QwerkE::Mirror::TypeInfo localStaticTypeInfo; \
	if (localStaticTypeInfo.enumType != MirrorTypes::m_Invalid) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.isPointer = std::is_pointer_v<TYPE>; \
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
 \
	using ClassType = TYPE; \
	enum { BASE = __COUNTER__ };

#define MIRROR_CLASS_SUBCLASS(SUBCLASS_TYPE) \
	const QwerkE::Mirror::TypeInfo* SUBCLASS_TYPE##Info = QwerkE::Mirror::InfoForType<SUBCLASS_TYPE>(); \
	localStaticTypeInfo.derivedTypesMap[MirrorTypes::SUBCLASS_TYPE] = SUBCLASS_TYPE##Info; \
	const_cast<QwerkE::Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo; \

#define MIRROR_CLASS_MEMBER(MEMBER_NAME) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	QwerkE::Mirror::Field MEMBER_NAME##field; \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = QwerkE::Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>(); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME; \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);

#define MIRROR_CLASS_END(TYPE) \
	if (localStaticTypeInfo.fields.size() >= fieldsCount) { bool warning = true; } \
	return &localStaticTypeInfo; \
} \
static const QwerkE::Mirror::TypeInfo* TYPE##typeInfo = QwerkE::Mirror::InfoForType<TYPE>(); \
// Call above initializes field class reference(s). Ideally, remove it and find another init method

// #NOTE Experimental collection type macro (currently unused)
#define MIRROR_COLLECTION(TYPE, COLLECTIONTYPE) \
template<> \
const QwerkE::Mirror::TypeInfo* QwerkE::Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	localStaticTypeInfo.collectionTypeInfo = QwerkE::Mirror::InfoForType<COLLECTIONTYPE>(); \
	return &localStaticTypeInfo; \
}
