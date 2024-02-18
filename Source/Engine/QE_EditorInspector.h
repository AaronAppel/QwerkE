#pragma once

#include <string>

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Constants.h"

namespace QwerkE {

    namespace Inspector {

        void InspectFieldRecursive(const Mirror::ClassInfo* classInfo, void* obj, std::string parentName);

    }

}
