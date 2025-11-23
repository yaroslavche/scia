#pragma once

#include <application/port/ILogger.hpp>
#include <domain/enum/LogLevel.hpp>
#include <mutex>

class ConsoleLogger final : public ILogger
{
public:
    explicit ConsoleLogger(LogLevel minLevel = LogLevel::Info);
    ~ConsoleLogger() override = default;

    void log(
        LogLevel level,
        const std::string &msg,
        const std::string &file,
        int line,
        const std::string &func) override;
    void log(LogLevel level, const std::string &msg) override;
    void setMinLevel(LogLevel level) override;
    [[nodiscard]] LogLevel getMinLevel() const override;

private:
    LogLevel m_minLevel;
    std::mutex m_mutex;

    [[nodiscard]] static std::string getCurrentTimestamp();
    [[nodiscard]] static std::string getLevelString(LogLevel level);
    [[nodiscard]] static std::string getLevelColor(LogLevel level);
};
