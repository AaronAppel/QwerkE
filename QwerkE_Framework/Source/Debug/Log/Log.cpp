#include "Log.h"

#include <stdio.h> // fprintf
#include <memory> // std::make_shared
#include <corecrt_wstdio.h> // stderr

#include <spdlog/sinks/stdout_color_sinks.h>

namespace QwerkE {

    bool Log::m_initialized = false;
    std::shared_ptr<spdlog::logger> Log::m_Logger;

    void Log::Initialize()
    {
        if (!m_initialized)
        {
            // The Cherno https://www.youtube.com/watch?v=dZr-53LAlOw&ab_channel=TheCherno
            spdlog::set_pattern("%^[%T] %n: %v%$");
            m_Logger = spdlog::stdout_color_mt("QwerkE");
            m_Logger->set_level(spdlog::level::trace);

            // https://github.com/gabime/spdlog
            // #TODO Look at "Logger with multi sinks - each with a different format and log level" example
            // to setup different kinds of logging for different levels of logs

            // auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            // consoleSink->set_pattern("%^[%T] %n: %v%$");

            // std::vector<spdlog::sink_ptr> sinks{consoleSink};
            // auto logger = std::make_shared<spdlog::logger>(ProjectName, sinks.begin(), sinks.end());
            // logger->set_level(spdlog::level::trace);
            // logger->flush_on(spdlog::level::trace);
            // spdlog::register_logger(logger);

            m_initialized = true;
            LOG_TRACE("Log initialized");
        }
        else
        {
            LOG_ERROR("Log is already initialized!");
        }
    }

    void Log::Shutdown()
    {
        if (m_initialized)
        {
            spdlog::shutdown();
            m_initialized = false;
        }
    }

    void Log::Safe(const char* message...)
    {
        fprintf(stderr, message);
    }
}
