#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "Assert.hpp"
#include "Memory.hpp"

namespace Hydrogen {
class Logger {
 public:
  enum class LogLevel { Trace = 0, Debug = 1, Info = 2, Warn = 3, Error = 4, Fatal = 5, Disable = 6 };
  Logger(String name, LogLevel logLevel = LogLevel::Info, String format = "%^[%T] %n: %v%$", bool out = true, String filename = "");

  template <typename... Args>
  inline void Trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    m_Logger->trace(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  inline void Debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    m_Logger->debug(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  inline void Info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    m_Logger->info(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  inline void Warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    m_Logger->warn(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  inline void Error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    m_Logger->error(fmt, std::forward<Args>(args)...);
  }
  template <typename... Args>
  inline void Fatal(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    m_Logger->critical(fmt, std::forward<Args>(args)...);
  }

 private:
  ReferencePointer<spdlog::logger> m_Logger;
};

#define HY_LOG_TRACE(...) Hydrogen::SystemLogger::GetLogger()->Trace(__VA_ARGS__);
#define HY_LOG_DEBUG(...) Hydrogen::SystemLogger::GetLogger()->Debug(__VA_ARGS__);
#define HY_LOG_INFO(...) Hydrogen::SystemLogger::GetLogger()->Info(__VA_ARGS__);
#define HY_LOG_WARN(...) Hydrogen::SystemLogger::GetLogger()->Warn(__VA_ARGS__);
#define HY_LOG_ERROR(...) Hydrogen::SystemLogger::GetLogger()->Error(__VA_ARGS__);
#define HY_LOG_FATAL(...) Hydrogen::SystemLogger::GetLogger()->Fatal(__VA_ARGS__);

class SystemLogger {
 public:
  static void Init() {
    s_Logger = NewReferencePointer<Logger>("SYS", Logger::LogLevel::Debug);
    HY_LOG_DEBUG("Initialized system logger!");
  }

  static ReferencePointer<Logger> GetLogger() { return s_Logger; }

 private:
  static ReferencePointer<Logger> s_Logger;
};
}  // namespace Hydrogen
