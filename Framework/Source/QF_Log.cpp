#include "QF_Log.h"

#include <cstdarg> // Variadic arguments
#include <memory> // std::make_shared
#include <corecrt_wstdio.h> // stderr
#include <debugapi.h> // OuputDebugStringA(buffer)

#include "Libraries/spdlog/sinks/stdout_color_sinks.h"

// #include "QF_Debug.h"

namespace QwerkE {

    namespace Log {

        bool s_initialized = false;
        std::shared_ptr<spdlog::logger> s_Logger;

        void Initialize()
        {
            // ASSERT(!m_initialized, "Log is already initialized!");
            if (!s_initialized)
            {
                spdlog::set_pattern("%^[%T] %n: %v%$");
                const char* loggerName = "Logger1";
                s_Logger = spdlog::stdout_color_mt(loggerName);
                s_Logger->set_level(spdlog::level::trace);

                s_initialized = true;
                LOG_TRACE("{0} Logger \"{1}\" initialized", __FUNCTION__, loggerName);
            }
        }

        void Shutdown()
        {
            // ASSERT(m_initialized, "Cannot shutdown if uninitialized!");
            spdlog::shutdown();
            s_initialized = false;
        }

        void Print(const char* message)
        {
            printf(message);
        }

        void Console(const char* message)
        {
#if _DEBUG
            OutputDebugStringA(message);
#endif
        }

// From : https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr

        // #TODO Clean up
        std::string string_format(const std::string fmt_str, ...) {
            int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
            std::unique_ptr<char[]> formatted;
            va_list ap;
            while (1) {
                formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
                strcpy(&formatted[0], fmt_str.c_str());
                va_start(ap, fmt_str);
                final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
                va_end(ap);
                if (final_n < 0 || final_n >= n)
                    n += abs(final_n - n + 1);
                else
                    break;
            }
            return std::string(formatted.get());
        }

        void Console(const char* message, const char* argument)
        {
#if _DEBUG
#include <string>
            std::string formattedString = string_format(message, argument);
            OutputDebugStringA(formattedString.data());
#endif
        }

        void Log(eLogLevel logLevel, ...)
        {
            // const char* message; // #TODO Fix logging
        }

        void Log(eLogLevel logLevel, const char* message, ...)
        {
            // #TODO Fix logging
            std::string formattedString = string_format(message);

            switch (logLevel)
            {
            case Trace:
                s_Logger->trace(formattedString.data());
                break;

            case Info:
                s_Logger->info(formattedString.data());
                break;

            case Warn:
                s_Logger->warn(formattedString.data());
                break;

            case Error:
                s_Logger->error(formattedString.data());
                break;

            case Critical:
                s_Logger->critical(formattedString.data());
                break;
            }
        }
    }

}
