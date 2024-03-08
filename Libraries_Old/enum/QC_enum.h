#pragma once

#include "Libraries/Enum/enum.h"

#define ENUM_TO_STR(x) (+x)._to_string()

// #TODO Create a #define to wrap the name. Maybe something like SERIALIZE_ENUM would be better
// #TODO Review enum declarations using enum.h library
#define QC_ENUM(EnumName, EnumType, ...) BETTER_ENUM(EnumName, EnumType, __VA_ARGS__)

// #TODO Examples of how to use BETTER_ENUMS
// ImGui::Button( ENUM_TO_STR(eEnumName::_from_index(i)), ...)
