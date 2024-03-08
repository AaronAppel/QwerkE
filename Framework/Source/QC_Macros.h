#pragma once

// #TODO Find a way to get ARGSN or NARGS macro to work, or at least macro overloading
#define QC_VARIABLE_ARGS_COUNT(...)  sizeof(__VA_ARGS__)/sizeof(int)