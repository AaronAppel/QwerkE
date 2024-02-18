#include "QF_Log.h"

#include <memory> // std::make_shared
#include <corecrt_wstdio.h> // stderr
#include <debugapi.h> // OuputDebugStringA(buffer)

#include "Libraries/spdlog/sinks/stdout_color_sinks.h"

#include "QF_Debug.h"

namespace QwerkE {

    bool Log::m_initialized = false;
    std::shared_ptr<spdlog::logger> Log::m_Logger;

    void Log::Initialize()
    {
        ASSERT(!m_initialized, "Log is already initialized!");
        if (!m_initialized)
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");
            const char* loggerName = "QwerkE";
            m_Logger = spdlog::stdout_color_mt(loggerName);
            m_Logger->set_level(spdlog::level::trace);

            m_initialized = true;
            LOG_TRACE("Logger \"{0}\" initialized", loggerName);
        }
    }

    void Log::Shutdown()
    {
        ASSERT(m_initialized, "Cannot shutdown if uninitialized!");
        spdlog::shutdown();
        m_initialized = false;
    }

    void Log::Print(const char* message)
    {
        printf(message);
    }

    void Log::Console(const char* message)
    {
#if _DEBUG
        OutputDebugStringA(message);
#endif
    }

    void Log::Print(const char* message, const char* arg1)
    {
        printf(message, arg1);
    }

    void Log::Print(const char* message, double arg1)
    {
        printf(message, arg1);
    }

    void Log::Print(const char* message, const char* arg1, const char* arg2)
    {
        printf(message, arg1, arg2);
    }

    void Log::Print(const char* message, double arg1, const char* arg2)
    {
        printf(message, arg1, arg2);
    }
}
