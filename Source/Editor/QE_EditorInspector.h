#pragma once

#include <string>

#include "Libraries/Mirror/Source/Mirror.h"

namespace QwerkE {

    namespace Inspector {

        bool InspectFieldRecursive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);

        template <class T>
        bool InspectFieldRecursive(T& obj, std::string& parentName)
        {
            return InspectFieldRecursive(Mirror::InfoForType<T>(), &obj, parentName);
        }

        template <class T>
        bool InspectObject(T& object)
        {
            std::string nameBuffer = "";
            nameBuffer.reserve(200);

            ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.5f);
            const bool valueChanged = InspectFieldRecursive(object, nameBuffer);
            ImGui::PopItemWidth();

            return valueChanged;
        }

    }

}
