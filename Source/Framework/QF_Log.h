#pragma once

#include <memory> // std::shared_ptr

#include "QF_Defines.h"

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

    class Log final
    {
    public:
        static void Initialize();
        static void Shutdown();

        static std::shared_ptr<spdlog::logger> m_Logger; // #TODO Private

        static void Console(const char* message);

        static void Print(const char* message);
        static void Print(const char* message, const char* arg1);
        static void Print(const char* message, double arg1);
        static void Print(const char* message, const char* arg1, const char* arg2);
        static void Print(const char* message, double arg1, const char* arg2);

    private:
        Log() = default;

        static bool m_initialized;
    };

}

// #ifdef _QDebug
#if _DEBUG
// #define LOG_TRACE(...)    ::QwerkE::LOG_TRACE(__VA_ARGS__) // TODO: Contain spdlog code inside Log.cpp only?

// #TODO Could add logic to check if the log system isn't initialized yet, and use a printf call instead
// #TODO Add more info to log calls like __FUNCTION__, __LINE__, __FILE__, and more
#define LOG_SAFE(msg, arg1) QwerkE::Log::Print(msg, arg1);
#define LOG_TRACE(...)      QwerkE::Log::m_Logger->trace(__VA_ARGS__);
#define LOG_INFO(...)       QwerkE::Log::m_Logger->info(__VA_ARGS__);
#define LOG_WARN(...)       QwerkE::Log::m_Logger->warn(__VA_ARGS__);
#define LOG_ERROR(...)      QwerkE::Log::m_Logger->error(__VA_ARGS__);
// #define LOG_ERROR_EXT(__FUNCTION__, ...)      QwerkE::Log::m_Logger->error(__VA_ARGS__);
#define LOG_CRITICAL(...)   QwerkE::Log::m_Logger->critical(__VA_ARGS__);
// #TODO Handle repeated (every frame) logs with some "no spam" function or argument like a bool with a default/optional argument

#else
#define LOG_TRACE(...)      (void)0
#define LOG_INFO(...)       (void)0
#define LOG_WARN(...)       (void)0
#define LOG_ERROR(...)      (void)0
#define LOG_CRITICAL(...)   (void)0
#endif
