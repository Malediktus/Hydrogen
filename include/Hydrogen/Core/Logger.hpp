#pragma once

#include "Memory.hpp"
#include "Assert.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <string>

namespace Hydrogen {
class Logger {
public:
    enum class LogLevel { Trace = 0, Debug = 1, Info = 2, Warn = 3, Error = 4, Fatal = 5, Disable = 6 };
    // TODO: Default arguments
    Logger(std::string name, LogLevel logLevel = LogLevel::Info, std::string format = "%^[%T] %n: %v%$", bool out = true, std::string filename = "");

    template <typename... Args> inline void Trace(Args&&... args) {
        m_Logger->trace(std::forward<Args>(args)...);
    }
    template <typename... Args> inline void Debug(Args&&... args) {
        m_Logger->debug(std::forward<Args>(args)...);
    }
    template <typename... Args> inline void Info(Args&&... args) {
        m_Logger->info(std::forward<Args>(args)...);
    }
    template <typename... Args> inline void Warn(Args&&... args) {
        m_Logger->warn(std::forward<Args>(args)...);
    }
    template <typename... Args> inline void Error(Args&&... args) {
        m_Logger->error(std::forward<Args>(args)...);
    }
    template <typename... Args> inline void Fatal(Args&&... args) {
        m_Logger->critical(std::forward<Args>(args)...);
    }

private:
    Reference<spdlog::logger> m_Logger;
};

class SystemLogger {
public:
    static void Init() {
        s_Logger = NewReference<Logger>("SYS", Logger::LogLevel::Debug);
    }

    static Reference<Logger> GetLogger() {
        return s_Logger;
    }

private:
    static Reference<Logger> s_Logger;
};

#define HY_LOG_TRACE(...) Hydrogen::SystemLogger::GetLogger()->Trace(__VA_ARGS__);
#define HY_LOG_DEBUG(...) Hydrogen::SystemLogger::GetLogger()->Debug(__VA_ARGS__);
#define HY_LOG_INFO(...) Hydrogen::SystemLogger::GetLogger()->Info(__VA_ARGS__);
#define HY_LOG_WARN(...) Hydrogen::SystemLogger::GetLogger()->Warn(__VA_ARGS__);
#define HY_LOG_ERROR(...) Hydrogen::SystemLogger::GetLogger()->Error(__VA_ARGS__);
#define HY_LOG_FATAL(...) Hydrogen::SystemLogger::GetLogger()->Fatal(__VA_ARGS__);
} // namespace Hydrogen
