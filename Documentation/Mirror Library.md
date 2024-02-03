
Mirror originally sprouted from the work of :
More info at the bottom of this page...

## What is Mirror? A reflection library/helper
## What does it do? Supplies runtime type information
## Why? For better type knowledge at runtime. Especially useful for type indirect code, such as data serialization

## What does the usage look like in code?
```cpp
template <class T>
void PrintClassFieldNames(const T& classType)
{
	std::cout << "Printing fields for type: " << Mirror::InfoForType(T)->stringName.c_str() << "\n";
	const Mirror::ClassInfo* classTypeInfo = Mirror::InfoForClass<T>(); // Compilation ensures valid pointer
	for (size_t i = 0; i < classTypeInfo->fields.size(); i++)
	{
		const Mirror::Field& field = classTypeInfo->fields[i];
		std::cout << field.name.c_str() << "\n";
	}
}
```

## How do I achieve that?
```cpp
// Serialization example using Mirror //
// 1. Create your type
class GameObject // Structs work the same in Mirror
{
public:
	...
private:
	MIRROR_PRIVATE_MEMBERS // Allows Mirror to access private members of a class/struct
	float movementSpeed = 1.f;
	int livesRemaining = 5;
}
MIRROR_CLASS_START(GameObject) // 2. "Mirror" types using macros
MIRROR_CLASS_MEMBER(movementSpeed) // All Mirror class macros can also be organized in 1 .cpp file
// MIRROR_CLASS_MEMBER(livesRemaining) // livesRemaining left unserializable/ignored by Mirror, for this example
MIRROR_CLASS_END

// 3. Add your new type in the MirrorTypes.h file
enum class MirrorTypes
{
	Settings, 
	...
}

// 4. Ask mirror for class and type info at runtime
template <class T>
void Serialize(T& objectOfTypeT)
{
	std::cout << "Type name is: " << Mirror::InfoForType(objectOfTypeT)->stringName.c_str() << "\n";
	if (const Mirror::ClassInfo* classInfo = Mirror::InfoForClass<T>())
	{
		std::cout << "Class field count: " << classInfo->fields.size() << std::endl;
	}
	...
}

```

Supporting more types, like pointers and collections? Mirror support is easy, but the usage will increase in complexity for most tasks. Here's an example of supporting a pointer type and vector of <u>TYPE</u>...
```cpp
// #TODO Example of GameObject* and std::vector<std::string> from serialization code
```

## Limitations?
Not many, as I think of it now. There certainly used to be as Mirror was in early development.

Mirror used to be limited by multiple instances of a classes, meaning you couldn't have a class that owned multiple instances of 1 class type. That is now supported.

In order to separate types and instances, each type must have a unique ID/key/identifier, but that is already enforced by language in the form of variable names, so since Mirror takes in the unique variable names, Mirror can support most use cases.

## More info:
I created Mirror to solve an old problem I had when first working on my game engine [QwerkE](https://github.com/AaronAppel/QwerkE) in college. I was interested in simplifying the process of serialization and deserializing code objects to data structures using the [cJSON](https://github.com/DaveGamble/cJSON) library. I thought there had to be a better way to tell an object to serialize itself without having to type out each parameter each time I added a new one. Some effort into searching was made, and as I didn't care to import the Boost library, or implement some more convoluted solutions, I gave up trying. Fast forward 4 years later, and I resumed work on my game engine, only to pick up the old problem that I had left behind. This go around, with more experience and internet information accumulation time, I searched and searched until I found something that resembled what I wanted. At first, it didn't work, but it did! And with more massaging I was able to extend the supported types, add templated methods with macros, then recursion for traversing complex and larger scale data trees/code objects.

## Ongoing :
Support for collections and info on them like field.type->collectionType to know **if** the field or type is a collection, and what type the collection is holding. Great for iterating over collections without needing to know they type of the collection, or what type the collection holds.

Problems once I started \#including the serialization header in other files. Can't have the power of templated functions being used everywhere. Either only file can \#include the templated method, or I need to use switch statements to handle explicitly calling a private method with the proper static known or compiled type.