#include <chrono>
#include <infrastructure/ConsoleLogger.hpp>
#include <iomanip>
#include <iostream>

ConsoleLogger::ConsoleLogger(const LogLevel minLevel)
    : m_minLevel(minLevel)
{}

std::string ConsoleLogger::getCurrentTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())
                    % 1000;
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string ConsoleLogger::getLevelString(LogLevel level)
{
    switch (level) {
    case LogLevel::Trace:
        return "TRACE";
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO ";
    case LogLevel::Warn:
        return "WARN ";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Fatal:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

std::string ConsoleLogger::getLevelColor(LogLevel level)
{
    // ANSI escape codes:
    // \033[0m  - Reset/End
    // \033[30m - Black \033[31m - Red \033[32m - Green \033[33m - Yellow
    // \033[34m - Blue \033[35m - Magenta \033[36m - Cyan \033[37m - White
    // \033[90m - Bright Black (Gray) \033[91m - Bright Red, etc.
    switch (level) {
    case LogLevel::Trace:
        return "\033[90m"; // Gray
    case LogLevel::Debug:
        return "\033[36m"; // Cyan
    case LogLevel::Info:
        return "\033[32m"; // Green
    case LogLevel::Warn:
        return "\033[33m"; // Yellow
    case LogLevel::Error:
        return "\033[31m"; // Red
    case LogLevel::Fatal:
        return "\033[91m"; // Bright Red
    default:
        return "\033[0m"; // Reset
    }
}

void ConsoleLogger::log(
    const LogLevel level,
    const std::string &msg,
    const std::string &file,
    const int line,
    const std::string &func)
{
    std::lock_guard lock(m_mutex);
    std::cout << getLevelColor(level) << "[" << getCurrentTimestamp() << "] "
              << getLevelString(level) << " [" << file << ":" << line << "][" << func << "] " << msg
              << "\033[0m" << std::endl;
}

void ConsoleLogger::log(const LogLevel level, const std::string &msg)
{
    std::lock_guard lock(m_mutex);
    std::cout << getLevelColor(level) << "[" << getCurrentTimestamp() << "] "
              << getLevelString(level) << " " << msg << "\033[0m" << std::endl;
}

void ConsoleLogger::setMinLevel(const LogLevel level)
{
    m_minLevel = level;
}

LogLevel ConsoleLogger::getMinLevel() const
{
    return m_minLevel;
}
