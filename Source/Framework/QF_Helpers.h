#pragma once

#include <string>

namespace QwerkE {

    namespace Helpers {

        char* StringAppend(const char* a, const char* b);
        char* StringAppend(const char* a, const char* b, const char* c);
        // char* StringAppend(va_list); // FEATURE: String append function that takes a variable number of char*s and appends them in order

        char* DeepCopyString(const char* string);

        char* CombineStrings(const char* a, const char* b);

        std::string Concatenate(const char* a, const char* b);

        bool StrCmp(std::string a, std::string b);
        bool StringCompare(std::string a, std::string b);

        bool StringHasText(std::string content, std::string searchText);

        int StringToInt(const char* string);

        char* ReverseString(char* value);

        uPtr<char[]> NumberAppendOrIncrement(const char* const charArray);

        // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
#include <memory>
#include <stdexcept>

        template<typename ... Args>
        std::string FormatString(const std::string& format, Args ... args)
        {
            const int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
            if (size_s <= 0)
            {
                LOG_ERROR("Error formatting string!");
                return std::string();
            }

            const size_t size = static_cast<size_t>(size_s);
            std::unique_ptr<char[]> buf(new char[size]);
            std::snprintf(buf.get(), size, format.c_str(), args ...);
            return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
        }

    }

}
