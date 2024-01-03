#pragma once

#include <string>

const char* StringAppend(const char* a, const char* b);
const char* StringAppend(const char* a, const char* b, const char* c);
// const char* StringAppend(va_list); // FEATURE: String append function that takes a variable number of char*s and appends them in order

char* DeepCopyString(const char* string);

const char* CombineStrings(const char* a, const char* b);
const char* CombineStrings(std::string a, std::string b);

std::string DispStrCombine(const char* a, const char* b);

bool StrCmp(std::string a, std::string b);
bool StringCompare(std::string a, std::string b);

bool SearchForString(std::string line, std::string word);

int StringToInt(const char* string);

const char* ReverseString(std::string value);
const char* ReverseString(const char* value);
