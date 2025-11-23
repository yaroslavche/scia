#include <cassert>
#include <chrono>
#include <domain/enum/LogLevel.hpp>
#include <infrastructure/FileLogger.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <utility>

namespace fs = std::filesystem;

FileLogger::FileLogger(
    std::string filename,
    const LogLevel minLevel,
    const size_t maxFileSizeKB,
    const size_t maxBackupFiles)
    : m_filename(std::move(filename))
    , m_minLevel(minLevel)
    , m_maxFileSizeKB(maxFileSizeKB)
    , m_maxBackupFiles(maxBackupFiles)
    , m_stopThread(false)
{
    openLogFile();
    m_workerThread = std::thread(&FileLogger::loggingWorker, this);
}

FileLogger::~FileLogger()
{
    {
        std::unique_lock lock(m_mutex);
        m_stopThread = true;
        m_cv.notify_one();
    }
    if (m_workerThread.joinable()) {
        m_workerThread.join();
    }
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void FileLogger::log(
    LogLevel level,
    const std::string &msg,
    const std::string &file,
    int line,
    const std::string &func)
{
    {
        std::string ts = getCurrentTimestamp();
        std::lock_guard lock(m_mutex);
        m_logQueue.emplace_back(level, msg, file, line, func, ts);
    }
    m_cv.notify_one();
}

void FileLogger::log(LogLevel level, const std::string &msg)
{
    {
        std::string ts = getCurrentTimestamp();
        std::lock_guard lock(m_mutex);
        m_logQueue.emplace_back(level, msg, ts);
    }
    m_cv.notify_one();
}

void FileLogger::setMinLevel(const LogLevel level)
{
    m_minLevel = level;
}

LogLevel FileLogger::getMinLevel() const
{
    return m_minLevel;
}

void FileLogger::openLogFile()
{
    if (m_logFile.is_open()) {
        m_logFile.close();
    }

    const fs::path logPath = m_filename;

    if (const fs::path logDir = logPath.parent_path(); !logDir.empty() && !fs::exists(logDir)) {
        try {
            fs::create_directories(logDir);
        } catch (const fs::filesystem_error &e) {
            std::cerr << "ERROR: FileLogger: Failed to create log directory " << logDir << ": "
                      << e.what() << std::endl;
            return;
        }
    }

    m_logFile.open(m_filename, std::ios::app);
    if (!m_logFile.is_open()) {
        std::cerr << "ERROR: FileLogger: Failed to open log file: " << m_filename << std::endl;
    }
}

void FileLogger::rotateLogs()
{
    if (m_logFile.is_open()) {
        m_logFile.close();
    }

    const fs::path currentPath = m_filename;

    if (const fs::path oldestBackup = currentPath.string() + "."
                                      + std::to_string(m_maxBackupFiles + 1);
        fs::exists(oldestBackup)) {
        try {
            fs::remove(oldestBackup);
        } catch (const fs::filesystem_error &e) {
            std::cerr << "WARN: FileLogger: Failed to remove oldest backup " << oldestBackup << ": "
                      << e.what() << std::endl;
        }
    }

    for (size_t i = m_maxBackupFiles; i > 0; --i) {
        fs::path src = currentPath.string() + "." + std::to_string(i);
        fs::path dest = currentPath.string() + "." + std::to_string(i + 1);

        if (fs::exists(src)) {
            try {
                fs::rename(src, dest);
            } catch (const fs::filesystem_error &e) {
                std::cerr << "WARN: FileLogger: Failed to rename " << src << " to " << dest << ": "
                          << e.what() << std::endl;
            }
        }
    }

    if (fs::exists(currentPath)) {
        try {
            fs::rename(currentPath, currentPath.string() + ".1");
        } catch (const fs::filesystem_error &e) {
            std::cerr << "ERROR: FileLogger: Failed to rename current log file " << currentPath
                      << " to " << currentPath.string() + ".1" << ": " << e.what() << std::endl;
        }
    }

    openLogFile();
}

std::string FileLogger::getCurrentTimestamp()
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

std::string FileLogger::getLevelString(const LogLevel level)
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

void FileLogger::loggingWorker()
{
    while (true) {
        LogEntryData entry;
        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_logQueue.empty() || m_stopThread; });

            if (m_stopThread && m_logQueue.empty()) {
                break;
            }

            entry = m_logQueue.front();
            m_logQueue.pop_front();
        }

        if (m_logFile.is_open()) {
            if (static_cast<size_t>(m_logFile.tellp()) > m_maxFileSizeKB * 1024ULL) {
                rotateLogs();
            }

            m_logFile << "[" << entry.timestamp << "] " << getLevelString(entry.level) << " ";
            if (entry.hasSource) {
                m_logFile << "[" << entry.file << ":" << entry.line << "][" << entry.func << "] ";
            }
            m_logFile << entry.message << std::endl;
            m_logFile.flush();
        } else {
            std::cerr << "ERROR: FileLogger: Log file not open, message lost: " << entry.message
                      << std::endl;
        }
    }
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}
