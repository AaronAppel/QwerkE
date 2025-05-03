#pragma once

#include <string>

#include "Libraries/Mirror/Source/MIR_Mirror.h"

namespace QwerkE {

    namespace Inspector {

        // #TODO Template so caller doesn't need to provide <T>
        bool InspectType(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);

        template <class T>
        bool InspectType(T& obj, std::string& parentName)
        {
            return InspectType(Mirror::InfoForType<T>(), &obj, parentName);
        }

        bool InspectType(const Mirror::TypeInfo* childTypeInfo, void* childInstance);

        template <class T>
        bool InspectType(T& obj)
        {
            return InspectType(Mirror::InfoForType<T>(), &obj);
        }

        template <class T>
        bool InspectObject(T& object)
        {
            std::string nameBuffer = "";
            nameBuffer.reserve(200);

            ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.5f);
            const bool valueChanged = InspectType(object, nameBuffer);
            ImGui::PopItemWidth();

            return valueChanged;
        }

    }

}
