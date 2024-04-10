#pragma once

#include <string>

#include "Libraries/Mirror/Source/Mirror.h"

namespace QwerkE {

    namespace Inspector {

        bool InspectFieldRecursive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);

        template <class T>
        bool InspectFieldRecursive(T& obj, std::string parentName)
        {
            return InspectFieldRecursive(Mirror::InfoForType<T>(), &obj, parentName);
        }

        template <class T>
        bool InspectObject(T& object, const char* const windowName)
        {
            // #TODO Look at using "##Title" for ImGui titles to hide the auto name on right hand side.
            // Can use a separate text field for field names to look better.

            if (!ImGui::Begin(windowName))
                return false; // #TODO ImGui::End() needed?

            // char charArray[200];
            std::string buffer = ""; // charArray;
            buffer.reserve(200);

            ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.5f);
            const bool valueChanged = InspectFieldRecursive(object, buffer);
            ImGui::PopItemWidth();

            ImGui::End();

            return valueChanged;
        }
    }

}
