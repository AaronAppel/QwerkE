Reference file : QE_EditorWindowAssets.h

1. Create a new class with the prefix "EditorWindow" that publicly inherits from EditorWindow
Example script of type "Example" :
``` cpp QF_EditorWindowExample.h
#pragma once  

namespace QwerkE {  
  
    class EditorWindowExample : public EditorWindow
    {  
        // #TODO

	private:
        bool m_IsEnabled = true;
		// ... other unique members
    };

}
```
2. Implement base pure virtual method(s)
3. Include new header file in QF_EditorWindowHelpers.h list of editor window header includes
4. Add type to EditorWindowsList
