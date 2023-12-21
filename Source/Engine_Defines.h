#ifndef _Engine_Defines_H_
#define _Engine_Defines_H_

// NOTE Separate from Engine.h for PCH

#ifdef FrameworkDir
#error FrameworkDir already defined!
#endif

// Override framework defaults
#define ProjectName "QwerkE"
#define _QAdvancedApi
#define FrameworkDir "QwerkE_Framework/"
#define AssetsDir "Assets/"
#define ProjectsDir AssetsDir "Projects/"

#endif // _Engine_Defines_H_
