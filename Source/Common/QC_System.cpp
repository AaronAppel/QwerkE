#include "QC_System.h"

#define _QWINDOWS 1

#if defined(_WINDOWS_) || defined(_QWINDOWS)
#include <windows.h>    // For getting user name
#include <Lmcons.h>     // For getting user name
#endif // defined(_WINDOWS_) || defined(_QWINDOWS)

#include "QC_StringHelpers.h"

namespace QwerkE {

	namespace System {

		std::string UserName()
		{
#if defined(_WINDOWS_) || defined(_QWINDOWS)
			TCHAR userName[UNLEN + 1];
			DWORD size = UNLEN + 1;
			GetUserName((TCHAR*)userName, &size);
			return wstrtostr(userName);
#else
#error Current platform unsupported!
#endif // defined(_WINDOWS_) || defined(_QWINDOWS)
			return std::string();
		}

		// https://learn.microsoft.com/en-us/windows/win32/shell/launch
		// ShellExecuteEx();

		// #TODO Explore process launching helpers
		void StartProcess()
		{
#if defined(_WINDOWS_) || defined(_QWINDOWS)
			CreateProcess(
				LPCWSTR(""),				// lpApplicationName
				LPWSTR(""),					// lpCommandLine
				LPSECURITY_ATTRIBUTES(),	// lpProcessAttributes
				LPSECURITY_ATTRIBUTES(),	// lpThreadAttributes
				BOOL(),						// bInheritHandles
				DWORD(),					// dwCreationFlags
				LPVOID(),					// lpEnvironment
				LPCWSTR(),					// lpCurrentDirectory
				LPSTARTUPINFOW(),			// lpStartupInfo
				LPPROCESS_INFORMATION()		// lpProcessInformation
			);
#else
// #error Current platform unsupported!
#endif // defined(_WINDOWS_) || defined(_QWINDOWS)
		}

		// #TODO Expose system sleep/pause helper
		void Sleep()
		{
#if defined(_WINDOWS_) || defined(_QWINDOWS)
			// system("pause/sleep");
#else
// #error Current platform unsupported!
#endif // defined(_WINDOWS_) || defined(_QWINDOWS)
		}

		void Command()
		{
#if defined(_WINDOWS_) || defined(_QWINDOWS)
			system("");
#else
			// #error Current platform unsupported!
#endif // defined(_WINDOWS_) || defined(_QWINDOWS)
		}

	}

}
