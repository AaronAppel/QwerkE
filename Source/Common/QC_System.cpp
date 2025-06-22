#include "QC_System.h"

#ifdef _WINDOWS_
#include <windows.h>    // For getting user name
#include <Lmcons.h>     // For getting user name
#endif // _WINDOWS_

#include "QC_StringHelpers.h"

namespace QwerkE {

	namespace System {

		std::string UserName()
		{
#ifdef _WINDOWS_
			TCHAR userName[UNLEN + 1];
			DWORD size = UNLEN + 1;
			GetUserName((TCHAR*)userName, &size);

			return wstrtostr(userName);
#else
// #error Current platform unsupported!
#endif // _WINDOWS_
			return std::string();
		}

		// https://learn.microsoft.com/en-us/windows/win32/shell/launch
		// ShellExecuteEx();

		// #TODO Explore process launching helpers
		void StartProcess()
		{
#ifdef _WINDOWS_
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
#endif // _WINDOWS_
		}

		// #TODO Expose system sleep/pause helper
		void Sleep()
		{
#ifdef _WINDOWS_
			// system("pause/sleep");
#else
// #error Current platform unsupported!
#endif // _WINDOWS_
		}

		void Command()
		{
#ifdef _WINDOWS_
			system("");
#else
			// #error Current platform unsupported!
#endif // _WINDOWS_
		}

	}

}
