#include "QF_System.h"

#include <cstdlib> // for system() command calls

#include "QF_Files.h"

namespace QwerkE {

	namespace System {

        void Sleep()
        {
            // #include <thread>
            // #include <chrono>
            // std::this_thread::sleep_for(milliseconds);
        }

        void YieldTime()
        {
#ifdef _WINDOWS_
            YieldProcessor();
#else
// #error Platform unsupported!
#endif // _WINDOWS_
        }

		void Command(const std::string& a_Command)
		{
#ifdef _WINDOWS_
            system(a_Command.c_str());

            // #TODO Stalls QwerkE and doesn't execute command as expected
            return;

			// #TODO Review safety and performance
			// #TODO Review return value

            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si)); // Initialize STARTUPINFO structure
            si.cb = sizeof(si); // Set the size of the structure
            ZeroMemory(&pi, sizeof(pi)); // Initialize PROCESS_INFORMATION structure

            const LPCWSTR applicationName = L"C:\\Windows\\System32\\cmd.exe";
            LPWSTR arguments = LPWSTR(a_Command.c_str());

            std::wstring ws;
            ws.assign(a_Command.begin(), a_Command.end());
            // get temporary LPWSTR (not really safe)
            LPWSTR pwst = &ws[0];

            // Create the new process
            auto result = !CreateProcess(
                applicationName,    // lpApplicationName (full path to executable)
                pwst,               // lpCommandLine (command-line arguments, if any)
                NULL,               // lpProcessAttributes (default security descriptor)
                NULL,               // lpThreadAttributes (default security descriptor)
                FALSE,              // bInheritHandles (do not inherit handles)
                0,                  // dwCreationFlags (default creation flags)
                NULL,               // lpEnvironment (use parent's environment)
                NULL,               // lpCurrentDirectory (use parent's current directory)
                &si,                // lpStartupInfo (pointer to STARTUPINFO structure)
                &pi                 // lpProcessInformation (pointer to PROCESS_INFORMATION structure)
            );

            if (0 != result)
            {
                // printf("CreateProcess failed (%d).\n", GetLastError());
            }

            // Wait until child process exits.
            // WaitForSingleObject(pi.hProcess, INFINITE);

            // Close process and thread handles.
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            // printf("Process created and terminated successfully.\n");

            // #TODO Look at proper or more up to date and safer method to start and wait on a new process
            // https://learn.microsoft.com/en-us/windows/win32/procthread/creating-processes
            /*
            #include <windows.h>
            #include <stdio.h>

            int main() {
                STARTUPINFO si;
                PROCESS_INFORMATION pi;

                ZeroMemory(&si, sizeof(si)); // Initialize STARTUPINFO structure
                si.cb = sizeof(si); // Set the size of the structure
                ZeroMemory(&pi, sizeof(pi)); // Initialize PROCESS_INFORMATION structure

                // Specify the application to launch
                // For example, launching Notepad
                LPCWSTR applicationName = L"C:\\Windows\\System32\\notepad.exe";

                // Create the new process
                if (!CreateProcess(
                    applicationName, // lpApplicationName (full path to executable)
                    NULL,            // lpCommandLine (command-line arguments, if any)
                    NULL,            // lpProcessAttributes (default security descriptor)
                    NULL,            // lpThreadAttributes (default security descriptor)
                    FALSE,           // bInheritHandles (do not inherit handles)
                    0,               // dwCreationFlags (default creation flags)
                    NULL,            // lpEnvironment (use parent's environment)
                    NULL,            // lpCurrentDirectory (use parent's current directory)
                    &si,             // lpStartupInfo (pointer to STARTUPINFO structure)
                    &pi              // lpProcessInformation (pointer to PROCESS_INFORMATION structure)
                )) {
                    printf("CreateProcess failed (%d).\n", GetLastError());
                    return 1;
                }

                // Wait until child process exits.
                WaitForSingleObject(pi.hProcess, INFINITE);

                // Close process and thread handles.
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);

                printf("Process created and terminated successfully.\n");

                return 0;
            }

            Explanation:
            STARTUPINFO si; and PROCESS_INFORMATION pi;:
            These structures are used to provide information about the new process and receive information about the created process, respectively.
            ZeroMemory and si.cb = sizeof(si);:
            It is crucial to initialize the STARTUPINFO structure and set its cb member to its size.
            LPCWSTR applicationName = L"C:\\Windows\\System32\\notepad.exe";:
            This defines the full path to the executable you want to launch. Using L prefix makes it a wide-character string, suitable for CreateProcessW (the Unicode version of CreateProcess).
            CreateProcess(...):
            This is the core function call.
            lpApplicationName: Specifies the executable to run.
            lpCommandLine: Can be used to pass command-line arguments to the new process. In this example, it's NULL as Notepad doesn't require arguments.
            The other parameters control security, handle inheritance, creation flags, environment, and current directory.
            WaitForSingleObject(pi.hProcess, INFINITE);:
            This function waits until the created process terminates. INFINITE means it will wait indefinitely.
            CloseHandle(pi.hProcess); and CloseHandle(pi.hThread);:
            It is vital to close the handles to the process and its primary thread when they are no longer needed to prevent resource leaks.
            Error Handling:
            GetLastError() is used to retrieve extended error information if CreateProcess fails.
            */
#endif // _WINDOWS_
		}

        void StartProcess(const string& a_PathToProcess)
        {
            // #TODO Error handling and update to non-deprecated API
            if (Files::Exists(a_PathToProcess.c_str()))
            {
                WinExec(a_PathToProcess.c_str(), 1);
            }
        }

	}

}
