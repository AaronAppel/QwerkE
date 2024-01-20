
#TODO Write section
QwerkE stores data using the [[JSON]] structure as laid out by the [[cJSON]] library.
The cJSON library assists in formatting data to write into, and read out of JSON text files. Normally JSON files will have the extension .json, but QwerkE uses several alternate file extensions to represent the data that is expected to be inside.

With the help of the [[Reflection Setup]] and [[Type Reflection]] system, the Serialization system is able to more easily read and write data to files. The process is made simpler by the reflection systems' type information to add indirection to type info. As long .

#TODO Write section
Structures as data layouts. If you want to change the way your data is laid out in a file, create or modify structs to represent how data should be formatted by the serialization system.

#TODO Provide examples :
```cpp
    struct Valueset1
    {
        bool boolValue = true;
        int intValue = 10;
    };
    
    struct Valueset2
    {
        const char* stringValue;
        std::vector<std::string> vectorValue;
    };
    
    struct StorageLayout1
    {
        Valueset1 valueset1;
        Valueset2 valueset2;
    };
```