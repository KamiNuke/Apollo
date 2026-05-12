#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace Apollo
{
    class Log
    {
    public:
        using LogRef = std::shared_ptr<spdlog::logger>;

        static void init();

        static LogRef& getCore();
        static LogRef& getUser();

    private:
        static LogRef s_coreSink;
        static LogRef s_userSink;
    };
} // Apollo

// MACROS TO USE
#define APOLLO_LOGGER_INFO(...) ::Apollo::Log::getCore()->info(__VA_ARGS__)
#define APOLLO_LOGGER_TRACE(...) ::Apollo::Log::getCore()->trace(__VA_ARGS__)
#define APOLLO_LOGGER_WARN(...) ::Apollo::Log::getCore()->warn(__VA_ARGS__)
#define APOLLO_LOGGER_CRITICAL(...) ::Apollo::Log::getCore()->critical(__VA_ARGS__)
#define APOLLO_LOGGER_ERROR(...) ::Apollo::Log::getCore()->error(__VA_ARGS__)

#define LOGGER_INFO(...) ::Apollo::Log::getUser()->info(__VA_ARGS__)
#define LOGGER_TRACE(...) ::Apollo::Log::getUser()->trace(__VA_ARGS__)
#define LOGGER_WARN(...) ::Apollo::Log::getUser()->warn(__VA_ARGS__)
#define LOGGER_CRITICAL(...) ::Apollo::Log::getUser()->critical(__VA_ARGS__)
#define LOGGER_ERROR(...) ::Apollo::Log::getUser()->error(__VA_ARGS__)