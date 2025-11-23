#pragma once

#include <application/port/ILogger.hpp>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

struct LogEntryData
{
    LogLevel level;
    std::string message;
    std::string file;
    int line;
    std::string func;
    bool hasSource;
    std::string timestamp;

    LogEntryData()
        : level(LogLevel::Info)
        , line(0)
        , hasSource(false)
    {}

    LogEntryData(
        const LogLevel lvl,
        std::string msg,
        std::string f,
        const int l,
        std::string fn,
        std::string ts)
        : level(lvl)
        , message(std::move(msg))
        , file(std::move(f))
        , line(l)
        , func(std::move(fn))
        , hasSource(true)
        , timestamp(std::move(ts))
    {}

    LogEntryData(const LogLevel lvl, std::string msg, std::string ts)
        : level(lvl)
        , message(std::move(msg))
        , line(0)
        , hasSource(false)
        , timestamp(std::move(ts))
    {}
};

class FileLogger final : public ILogger
{
public:
    explicit FileLogger(
        std::string filename,
        LogLevel minLevel = LogLevel::Info,
        size_t maxFileSizeKB = 5 * 1024,
        size_t maxBackupFiles = 5);
    ~FileLogger() override;

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
    std::string m_filename;
    LogLevel m_minLevel;
    size_t m_maxFileSizeKB;
    size_t m_maxBackupFiles;
    std::ofstream m_logFile;
    std::mutex m_mutex;

    std::deque<LogEntryData> m_logQueue;
    std::thread m_workerThread;
    std::condition_variable m_cv;
    bool m_stopThread;

    void rotateLogs();
    void openLogFile();
    [[nodiscard]] static std::string getCurrentTimestamp();
    [[nodiscard]] static std::string getLevelString(LogLevel level);

    void loggingWorker();
};
