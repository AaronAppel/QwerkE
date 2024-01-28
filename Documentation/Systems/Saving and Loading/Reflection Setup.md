#TODO Write nice setup documentation for new serialization system

```cpp title:QF_ReflectionTypes.h
// Add type names here to support reflection/serialization (case sensitive)
enum class TypeName {

	int8_t,
	int16_t,
	int32_t,

	Int, // Primitives need upper casing, and a matching typedef to work
	Bool,
	Char,
	Float,
	Double,

    eKeys,
	...
}
```

```cpp title:QF_ReflectionTypes.cpp
namespace QwerkE
{
    namespace Reflection // Type defines
    {
        DEFINE_TYPE(int8_t) // Primitives
        ...
        DEFINE_TYPE(eKeys) // User defined types
	}
	
	namespace Reflection // Classes and fields
	{
        BEGIN_ATTRIBUTES_FOR(Controls) // Structs
        DEFINE_MEMBER(Camera_MoveForward)
        ...
        END_ATTRIBUTES
		
		BEGIN_ATTRIBUTES_FOR(UserData) // Classes
		DEFINE_MEMBER(controls)
		END_ATTRIBUTES
	}
}
```

#TODO Note the need for "typedef bool Bool;", and what issue solves as a work around. Also note the types are defined in a .cpp to avoid injecting using/typedefs into other files through a header.

#TODO Support private members for serialization. Could use friend class approach :
https://www.geeksforgeeks.org/friend-class-function-cpp/
## Limitations
The macros for generating the serialization type code cannot see protected or private level fields (currently). This means only public fields, i.e. structs are supported for serialization, as of now.