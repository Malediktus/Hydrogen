#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Hydrogen/Core/Logger.hpp>

using namespace Hydrogen;

Logger::Logger(String name, LogLevel logLevel, String format, bool out,
               String filename) {
  DynamicArray<spdlog::sink_ptr> logSinks;
  if (!filename.empty())
    logSinks.emplace_back(
        NewReferencePointer<spdlog::sinks::basic_file_sink_mt>(filename, true));
  if (out)
    logSinks.emplace_back(
        NewReferencePointer<spdlog::sinks::stdout_color_sink_mt>());

  for (auto& logSink : logSinks) logSink->set_pattern(format);

  m_Logger =
      NewReferencePointer<spdlog::logger>(name, begin(logSinks), end(logSinks));
  m_Logger->set_level((spdlog::level::level_enum)logLevel);
}

ReferencePointer<Logger> SystemLogger::s_Logger;
