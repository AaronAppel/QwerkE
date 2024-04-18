#include "QC_System.h"

#include <windows.h>    // For getting user name
#include <Lmcons.h>     // For getting user name

#include "QC_StringHelpers.h"

namespace QwerkE {

	namespace System {

		std::string UserName()
		{
			TCHAR userName[UNLEN + 1];
			DWORD size = UNLEN + 1;
			GetUserName((TCHAR*)userName, &size);

			return wstrtostr(userName);
		}

	}

}
