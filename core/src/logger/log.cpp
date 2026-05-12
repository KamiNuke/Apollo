#include "log.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Apollo
{
    Log::LogRef Log::s_coreSink;
    Log::LogRef Log::s_userSink;

    /*
     * TODO: Make so it saves a new file each run Apollo_1.txt Apollo_2.txt etc...
     */
    void Log::init()
    {
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Apollo.txt", true));

        s_coreSink = std::make_shared<spdlog::logger>("Core", sinks.begin(), sinks.end());
        spdlog::register_logger(s_coreSink);
        s_coreSink->set_level(spdlog::level::trace);
        s_coreSink->flush_on(spdlog::level::trace);

        s_userSink = std::make_shared<spdlog::logger>("Application", sinks.begin(), sinks.end());
        spdlog::register_logger(s_userSink);
        s_userSink->set_level(spdlog::level::trace);
        s_userSink->flush_on(spdlog::level::trace);
    }

    Log::LogRef& Log::getCore()
    {
        return s_coreSink;
    }

    Log::LogRef& Log::getUser()
    {
        return s_userSink;
    }
} // Apollo