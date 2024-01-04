#pragma once

#include <map>

// #TODO Reduce caller logic. Maybe create a new method if needed
bool ArgumentExists(unsigned int argc, char** argv, const char* argumentString);
std::map<const char*, const char*> ArgumentKeyValuePairs(unsigned int argc, char** argv);
