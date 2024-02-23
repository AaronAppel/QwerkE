#pragma once

#include <string>

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Constants.h"

namespace QwerkE {

    namespace Inspector {

        void InspectFieldRecursive(const Mirror::TypeInfo* typeInfo, void* obj, std::string parentName);

        template <class T>
        void InspectFieldRecursive(T& obj, std::string parentName)
        {
            InspectFieldRecursive(Mirror::InfoForType<T>(), &obj, parentName);
        }

        template <class T>
        void InspectObject(T& object, const char* const windowName)
        {
            if (!ImGui::Begin(windowName))
                return;

            // char charArray[200];
            std::string buffer = ""; // charArray;
            buffer.reserve(200);

            ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.5f);
            InspectFieldRecursive(object, buffer);
            ImGui::PopItemWidth();

            ImGui::End();
        }
    }

}
