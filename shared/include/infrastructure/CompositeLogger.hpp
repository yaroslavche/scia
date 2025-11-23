#pragma once

#include <application/port/ILogger.hpp>
#include <functional>
#include <mutex>
#include <vector>

enum class LogTarget : unsigned int {
    None = 0,
    Console = 1 << 0,
    File = 1 << 1,
    Database = 1 << 2,
    All = Console | File | Database
};

inline LogTarget operator|(LogTarget a, LogTarget b)
{
    return static_cast<LogTarget>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline LogTarget operator&(LogTarget a, LogTarget b)
{
    return static_cast<LogTarget>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

class CompositeLogger final : public ILogger
{
public:
    ~CompositeLogger() override = default;

    void addLogger(ILogger *logger, const LogTarget type)
    {
        if (logger) {
            std::lock_guard lock(m_mutex);
            m_loggers.push_back({*logger, type});
        }
    }

    void log(
        const LogLevel level,
        LogTarget targets,
        const std::string &msg,
        const std::string &file,
        const int line,
        const std::string &func) const
    {
        std::lock_guard lock(m_mutex);
        for (const auto &[loggerRef, target] : m_loggers) {
            if (static_cast<unsigned int>(target) & static_cast<unsigned int>(targets)) {
                if (level >= loggerRef.get().getMinLevel()) {
                    loggerRef.get().log(level, msg, file, line, func);
                }
            }
        }
    }

    void log(const LogLevel level, LogTarget targets, const std::string &msg) const
    {
        std::lock_guard lock(m_mutex);
        for (const auto &[loggerRef, target] : m_loggers) {
            if (static_cast<unsigned int>(target) & static_cast<unsigned int>(targets)) {
                if (level >= loggerRef.get().getMinLevel()) {
                    loggerRef.get().log(level, msg);
                }
            }
        }
    }

    void log(
        const LogLevel level,
        const std::string &msg,
        const std::string &file,
        const int line,
        const std::string &func) override
    {
        log(level, LogTarget::All, msg, file, line, func);
    }

    void log(const LogLevel level, const std::string &msg) override
    {
        log(level, LogTarget::All, msg);
    }

    void setMinLevel(const LogLevel level) override
    {
        std::lock_guard lock(m_mutex);
        for (const auto &[loggerRef, target] : m_loggers) {
            loggerRef.get().setMinLevel(level);
        }
    }

    [[nodiscard]] LogLevel getMinLevel() const override { return LogLevel::Trace; }

private:
    struct LoggerEntry
    {
        std::reference_wrapper<ILogger> loggerRef;
        LogTarget target;
    };

    std::vector<LoggerEntry> m_loggers;
    mutable std::mutex m_mutex;
};
