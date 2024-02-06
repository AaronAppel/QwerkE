This is most likely caused by a circular \#include dependency error.
I encountered it while working on the Serialization logic in QF_Serialization.h.
The templated functions in the header are used in files that I \#include in QF_Serialization.cpp, and therefore the templated function are defined multiple times.