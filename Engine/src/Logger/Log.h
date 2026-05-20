#pragma once
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "Core/Base.h"

namespace Apollo
{
    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger();
        static Ref<spdlog::logger>& GetUserLogger();

    private:
        static Ref<spdlog::logger> s_coreSink;
        static Ref<spdlog::logger> s_userSink;
    };
} // Apollo

// ENGINE ONLY MACROS
#define APOLLO_LOGGER_INFO(...) ::Apollo::Log::GetCoreLogger()->info(__VA_ARGS__)
#define APOLLO_LOGGER_TRACE(...) ::Apollo::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define APOLLO_LOGGER_WARN(...) ::Apollo::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define APOLLO_LOGGER_CRITICAL(...) ::Apollo::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define APOLLO_LOGGER_ERROR(...) ::Apollo::Log::GetCoreLogger()->error(__VA_ARGS__)

// APPLICATION MACROS
#define LOGGER_INFO(...) ::Apollo::Log::GetUserLogger()->info(__VA_ARGS__)
#define LOGGER_TRACE(...) ::Apollo::Log::GetUserLogger()->trace(__VA_ARGS__)
#define LOGGER_WARN(...) ::Apollo::Log::GetUserLogger()->warn(__VA_ARGS__)
#define LOGGER_CRITICAL(...) ::Apollo::Log::GetUserLogger()->critical(__VA_ARGS__)
#define LOGGER_ERROR(...) ::Apollo::Log::GetUserLogger()->error(__VA_ARGS__)