#pragma once

#include <string>

char* StringAppend(const char* a, const char* b);
char* StringAppend(const char* a, const char* b, const char* c);
// char* StringAppend(va_list); // FEATURE: String append function that takes a variable number of char*s and appends them in order

char* DeepCopyString(const char* string);

char* CombineStrings(const char* a, const char* b);
char* CombineStrings(std::string a, std::string b);

std::string DispStrCombine(const char* a, const char* b);

bool StrCmp(std::string a, std::string b);
bool StringCompare(std::string a, std::string b);

bool SearchForString(std::string line, std::string word);

int StringToInt(const char* string);

char* ReverseString(char* value);

char* NumberAppendOrIncrement(const char* const charArray);

// std::vector<int> NumberSplitDigits(int number); // Number splitter helper, in some other file

// #TODO FormatString(const char* string) // For formatting, inserting, or replacing, variable argument strings
