Reference file : QF_ScriptCamera.h

1. Create a new class with the prefix "Scriptable" that publicly inherits from Scriptable
Example script of type "Example" :
``` cpp QF_ScriptableExample.h
#pragma once  

namespace QwerkE {  
  
    class ScriptableCamera : public Scriptable
    {  
        // Implied ScriptableExample() = default;
        // Implied ScriptableExample(const ScriptableExample& other) = default;
	
	private:
        bool m_IsEnabled = true;
		// ... other unique members
    };

}
```
2. Implement base pure virtual method(s)
3. Include new header file in QF_Scripting.h list of script header includes
4. Add type to ComponentScriptsList
