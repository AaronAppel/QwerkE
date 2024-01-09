#pragma once

#include "Libraries/Enum/enum.h"

#define ENUM_TO_STR(x) (+x)._to_string()

// #TODO Create a #define to wrap the name. Maybe something like SERIALIZE_ENUM would be better
// #TODO Review enum declarations using enum.h library
#define QC_ENUM(Enum, Underlying, ...) BETTER_ENUM(Enum, Underlying, __VA_ARGS__)