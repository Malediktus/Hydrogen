#pragma once

#include "Memory.hpp"
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
} // namespace Hydrogen
