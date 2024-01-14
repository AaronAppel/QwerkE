#include "QF_Log.h"

#include <stdio.h> // fprintf
#include <memory> // std::make_shared
#include <corecrt_wstdio.h> // stderr

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
}
