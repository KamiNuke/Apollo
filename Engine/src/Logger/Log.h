#pragma once
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Apollo
{
    class Log
    {
    public:
        using LogRef = std::shared_ptr<spdlog::logger>;

        static void Init();

        static LogRef& GetCore();
        static LogRef& GetUser();

    private:
        static LogRef s_coreSink;
        static LogRef s_userSink;
    };
} // Apollo

// ENGINE ONLY MACROS
#define APOLLO_LOGGER_INFO(...) ::Apollo::Log::GetCore()->info(__VA_ARGS__)
#define APOLLO_LOGGER_TRACE(...) ::Apollo::Log::GetCore()->trace(__VA_ARGS__)
#define APOLLO_LOGGER_WARN(...) ::Apollo::Log::GetCore()->warn(__VA_ARGS__)
#define APOLLO_LOGGER_CRITICAL(...) ::Apollo::Log::GetCore()->critical(__VA_ARGS__)
#define APOLLO_LOGGER_ERROR(...) ::Apollo::Log::GetCore()->error(__VA_ARGS__)

// APPLICATION MACROS
#define LOGGER_INFO(...) ::Apollo::Log::GetUser()->info(__VA_ARGS__)
#define LOGGER_TRACE(...) ::Apollo::Log::GetUser()->trace(__VA_ARGS__)
#define LOGGER_WARN(...) ::Apollo::Log::GetUser()->warn(__VA_ARGS__)
#define LOGGER_CRITICAL(...) ::Apollo::Log::GetUser()->critical(__VA_ARGS__)
#define LOGGER_ERROR(...) ::Apollo::Log::GetUser()->error(__VA_ARGS__)