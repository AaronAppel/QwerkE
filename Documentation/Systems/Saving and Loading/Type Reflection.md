QwerkE [[Reflection Setup]]
## Blog Post : # Static Reflection and Serialization in C++
- [x]  https://technicallydeclined.com/static-reflection-and-serialization-in-c/
 Daniel Barrera https://github.com/danbar0 
Findings :
My initial attempts ended in compile errors until I eventually found the editor's GitHub repo for the blog post.
There were issues with the repo so I needed to pull in [rapidjson](https://github.com/Tencent/rapidjson) plus remove the google test code. After that I still had to play around with the example and really massage it to get past the ugly linker errors I was getting, but that could have been my fault to start. After that, I did get the example code working and it was pretty cool to see. Definitely the outcome I have been looking for and I will look to play with this in QwerkE in the future.

Enum for behaviour on type using if or switch statements. Allows for looping over fields and handling each type in a switch statement.

Requirements + Usage:
For any type that you want to have reflection supported for you must use the following macro calls:
```cpp
struct Thing
{
	int member;
};
DEFINE_TYPE(Thing)

BEGIN_ATTRIBUTES_FOR(Thing)
DEFINE_MEMBER(member);
END_ATTRIBUTES

```

Note that private members are inaccessible to the macro, so that is one limitation, unless I am missing something.
DEFINE_TYPE() and BEGIN_ATTRIBUTES_FOR() order dependency.
Current supported info : Type name, type size, type enum name, value name, value type, value offset

Results :
The proposed logic solves a current issue I have QwerkE relating to saving and loading scene object and other data using json serialization. I have to manually handle reading and writing each object's value into and out of the json structure. With the new serialization changes I should be and to abstract the value name and type from each object so any fields marked serializable will be added automatically once the system is in place. This should also allow for reading and writing logic to come closer together, and ideally reduce code size and redundancy.

Take Away :
After all of my searching it is nice to find a solution, albeit imperfect but I can work with that. I am glad to have found it and appreciate the work of Daniel Barrera. Next, I am hoping to learn the underlying logic and modify the source code to better suit my current needs, and future needs.

## Ponder reflection library :
- [ ] https://billyquith.github.io/ponder/
- [ ] https://github.com/billyquith/ponder
Does Ponder support serializing private members?
## Long Stack Overflow Post
- [ ] https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application