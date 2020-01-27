#ifndef _Engine_Defines_H_
#define _Engine_Defines_H_

#define Qwerk_Engine

#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/Headers/QwerkE_Directory_Defines.h"

/* Define program arguments */
#define key_ProjectName "-projectName" // "-projectName" Look in projects folder for a project with the same name.
// "-projectFilePath" Absolute or relative to working directory.
// etc...

/* Define values to be used in other ares of code.*/
#define EngineDataFolderPath(a) StringAppend("../Shared_Engine/Data/", a)

#define EngineConfigFile "EngineConfig.json"

#endif // !_Engine_Defines_H_
