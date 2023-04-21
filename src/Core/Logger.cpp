#include <Hydrogen/Core/Logger.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace Hydrogen;

Logger::Logger(std::string name, LogLevel logLevel, std::string format, bool out, std::string filename) {
    std::vector<spdlog::sink_ptr> logSinks;
    if (!filename.empty())
        logSinks.emplace_back(NewReference<spdlog::sinks::basic_file_sink_mt>(filename, true));
    if (out)
        logSinks.emplace_back(NewReference<spdlog::sinks::stdout_color_sink_mt>());

    for (auto& logSink : logSinks)
        logSink->set_pattern(format);

    m_Logger = NewReference<spdlog::logger>(name, begin(logSinks), end(logSinks));
    m_Logger->set_level((spdlog::level::level_enum) logLevel);
}
