
## Naming
The current standard for naming enums in QwerkE is to use a lower case 'e' as a prefix. The type Enum would be named eEnum using this prefix, as an example. That may change in the future but is currently enforceable.

## Required Values
It seems best to include a null and a max value in all enums for serialization purposes. Even if the enum doesn't need to be serializable, it is still nice to know when the value is invalid and use max to loop through a types/enum values list.
For the given enum eEnum, the following example is a standard default declaration :
```cpp
enum class eEnum
{
    _Null = 0,
    _Max
};
```

#TODO Value naming. Should the type name appear again? If the eEnum:: accessor is not used then the value's name can be confused with other values like NULL. Should underscores be standard, or required?

## Enum Decorator
#TODO With the enum.h library, it is possible and simple to perform some reflection on enum types. It would be nice to be able to declare enums in the standard fashion, instead of using the BETTER_ENUM() \#define formatting, but still get the functionality of better-enums.