#pragma once

// Reference : Timmy_A
// https://stackoverflow.com/questions/3386861/converting-a-variable-name-to-a-string-in-c

#define VARNAME_TO_STR(x) #x
#define TYPENAME_TO_STR(x) #x

// #define VARIABLE_TO_STRING(Variable) #Variable // #TODO Lowest variable name. Ex: object.value = "value"
#define FULL_VARIABLE_TO_STRING(Variable) (void(Variable),#Variable) // Full variable name. Ex: object.value = "object.value"

#define FUNCTION_TO_STRING(Function) (void(&Function),#Function)

#define METHOD_TO_STRING(ClassName,Method) (void(&ClassName::Method),#Method)

#define TYPE_TO_STRING(TypeInfo) (void(sizeof(TypeInfo)),#TypeInfo)

// #define STRINGIFY_VARIABLE # // #TODO Look into stringify reflection logic for variables, functions, types, etc

#include <string>
#include <array>

#include "QF_ReflectionTypes.h"

// #TODO Might be worthwhile creating a reflection string type that might be lighter/better suited than std::string

// Original reflection helpers source : https://github.com/danbar0/StaticJsonReflection/tree/master
// Modified to work with QwerkE

// #TODO Find a way to get a nice typeof() method.
// This one relies on the type being known at compile time, which won't work in all use cases (indirect serialization for example).
#define typeof(T) Reflection::GetClass<T>()

namespace QwerkE
{
	namespace Reflection
	{

#define MAX_NUMBER_OF_FIELDS 10

		// #TODO Rename Type, Field, and Class types to be more specific to reflection.
		// Also consider placing code in this file in a Reflection:: Namespace
		struct TypeInfo {
			std::string stringName;
			ReflectionType enumType;
			size_t size;
		};

		struct Field {
			const TypeInfo* type;
			std::string name;
			size_t offset;
		};

		struct ClassInfo {
			std::array<Field, MAX_NUMBER_OF_FIELDS> fields;
		};

		template<typename T>
		const TypeInfo* GetTypeInfo();

		template<typename T>
		const ClassInfo* GetClass(); // #TODO Look at naming GetClassInfo if can get around naming collision with windows library
	}
}

#define DEFINE_TYPE(TYPE) \
template<> \
const TypeInfo* GetTypeInfo<TYPE>() { \
	static TypeInfo type; \
	type.stringName = #TYPE; \
	type.size = sizeof(TYPE); \
	type.enumType = ReflectionType::TYPE; \
	return &type; \
}\

#define BEGIN_ATTRIBUTES_FOR(CLASS)  \
template<> \
const ClassInfo* GetClass<CLASS>() { \
	using ClassType = CLASS; \
	static ClassInfo localClass; \
	enum { BASE = __COUNTER__ }; \

#define DEFINE_MEMBER(MEMBER_NAME)  \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1}; \
	localClass.fields[MEMBER_NAME##Index].type = GetTypeInfo<decltype(ClassType::MEMBER_NAME)>();\
	localClass.fields[MEMBER_NAME##Index].name = { #MEMBER_NAME };  \
	localClass.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);\

#define END_ATTRIBUTES \
return &localClass; \
}\
