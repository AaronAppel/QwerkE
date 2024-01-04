#pragma once

#include <map>

// #TODO Move more logic into functions so calling
bool ArgumentExists(unsigned int argc, char** argv, const char* argumentString);
std::map<const char*, const char*> ArgumentKeyValuePairs(unsigned int argc, char** argv);
