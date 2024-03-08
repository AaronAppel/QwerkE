#pragma once

#include <memory> // std::shared_ptr

// #include "QF_Defines.h"

#pragma warning( disable : 26437 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26498 )
#pragma warning( disable : 6387 )
#pragma warning( disable : 4996 )
#include "Libraries/spdlog/spdlog.h"
#pragma warning( default : 26437 )
#pragma warning( default : 26451 )
#pragma warning( default : 26495 )
#pragma warning( default : 26498 )
#pragma warning( default : 6387 )
// #pragma warning( default : 4996 ) // #TODO Fix 4996 error "error C4996: 'fmt::v9::detail::arg_mapper<Context>::map': was declared deprecated" in ShaderFactory.cpp
// Maybe move spdlog include and references into .cpp so it's only compiled once in there

namespace QwerkE {

    namespace Log {

        void Initialize();
        void Shutdown();

        void Console(const char* message);
        void Console(const char* message, const char* argument);

        struct Logger {
            static std::shared_ptr<spdlog::logger> s_Logger;
        };

    }

}

#if _QDebug
#define LOG_TRACE(...)      QwerkE::Log::Logger::s_Logger->trace(__VA_ARGS__);
#define LOG_INFO(...)       QwerkE::Log::Logger::s_Logger->info(__VA_ARGS__);
#define LOG_WARN(...)       QwerkE::Log::Logger::s_Logger->warn(__VA_ARGS__);
#define LOG_ERROR(...)      QwerkE::Log::Logger::s_Logger->error(__VA_ARGS__);
#define LOG_CRITICAL(...)   QwerkE::Log::Logger::s_Logger->critical(__VA_ARGS__);

#define LOG_INFO_ONCE(...) { static bool s_logged = false; if (!s_logged) { s_logged = true; QwerkE::Log::s_Logger->info(__VA_ARGS__); } }

// #TODO Back trace message : https://github.com/gabime/spdlog?tab=readme-ov-file#backtrace-support

// Defined MSVC macros https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170
// #define LOG_WARN_PLUS(...)  QwerkE::Log::Warn(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

#else
#define LOG_TRACE(...)      (void)0
#define LOG_INFO(...)       (void)0
#define LOG_WARN(...)       (void)0
#define LOG_ERROR(...)      (void)0
#define LOG_CRITICAL(...)   (void)0
#endif
