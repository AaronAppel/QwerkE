Reference file : QF_ComponentInfo.h

1. Create a new struct with the prefix "Component".
Example component of type "Example" :
``` cpp QF_ComponentExample.h
#pragma once  

namespace QwerkE {  
  
    struct ComponentExample
    {  
        // Implied ComponentExample() = default;
        // Implied ComponentExample(const ComponentExample& other) = default;

        bool m_IsEnabled = true;
		// ... other unique members
    };

}
```
2. Include new header file in QF_ComponentHelpers.h list of component header includes
3. Add type to EntityComponentsList

Notes on entity components:
- Data is public so struct is used to avoid unnecessary getters and setters
- No base class inheritance, so common API or members between component types
- Must have an empty constructor implementation (or default) for de/serialization purposes
- No enum type for serialization or other purposes