#pragma once

#include <map>

#include "QF_Enums.h"

#include "QC_StringHelpers.h"

/* Define values to be used in other ares of code. */
#define EngineDataFolderPath(fileName) StringAppend("Data/", fileName)

/* Define program arguments */
#define key_ProjectName "-projectName" // "-projectName" Look in projects folder for a project with the same name.
// "-projectFilePath" Absolute or relative path to working directory.
// etc...

// What engine functionality should be user accessible?

namespace QwerkE {

	namespace Engine
	{
		QwerkE::eEngineMessage Run(std::map<const char*, const char*> &args);
		void Stop();

		void NewFrame();
		void PollInput();
		void Update(float deltatime);
		void Draw();
	}

}
