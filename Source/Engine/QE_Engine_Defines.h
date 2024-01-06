#pragma once

// NOTE Separate from Engine.h for PCH

#ifdef FrameworkDir
#error FrameworkDir already defined!
#endif

// Override framework defaults
#define ProjectName "QwerkE"
#define _QAdvancedApi
#define FrameworkDir ""
#define AssetsDir "Assets/"
#define ProjectsDir AssetsDir "Projects/"
