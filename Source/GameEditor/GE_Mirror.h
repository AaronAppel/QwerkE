#pragma once

#include "Libraries/Mirror/Source/MIR_Mirror.h"

#include "GE_ScriptGameEntity.h"

// Specialize TypeId to support additional types
// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

// #NOTE Game Ids start at +3000
MIRROR_TYPE_ID(3000, QwerkE::ScriptGameEntity)
