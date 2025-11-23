#pragma once

#include <domain/enum/LogLevel.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

class LogMessageStream
{
public:
    explicit LogMessageStream(const bool active)
        : m_active(active)
    {
        if (m_active) {
            m_stream = std::make_unique<std::ostringstream>();
        }
    }

    template<typename T>
    LogMessageStream &operator<<(const T &value)
    {
        if (m_active && m_stream) {
            *m_stream << value;
        }
        return *this;
    }

    LogMessageStream &operator<<(std::ostream &(*manip)(std::ostream &) )
    {
        if (m_active && m_stream) {
            *m_stream << manip;
        }
        return *this;
    }

    [[nodiscard]] std::string str() const
    {
        if (m_active && m_stream) {
            return m_stream->str();
        }
        return "";
    }

private:
    bool m_active;
    std::unique_ptr<std::ostringstream> m_stream;
};

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void log(
        LogLevel level,
        const std::string &msg,
        const std::string &file,
        int line,
        const std::string &func)
        = 0;

    virtual void log(LogLevel level, const std::string &msg) = 0;

    virtual void setMinLevel(LogLevel level) = 0;
    [[nodiscard]] virtual LogLevel getMinLevel() const = 0;

    class LogEntryWithSource
    {
    public:
        LogEntryWithSource(
            ILogger *logger, const LogLevel level, std::string file, const int line, std::string func)
            : m_logger(logger)
            , m_level(level)
            , m_file(std::move(file))
            , m_line(line)
            , m_func(std::move(func))
            , m_isActive(logger && level >= logger->getMinLevel())
            , m_messageStream(m_isActive)
        {}

        ~LogEntryWithSource()
        {
            if (m_isActive) {
                m_logger->log(m_level, m_messageStream.str(), m_file, m_line, m_func);
            }
        }

        template<typename T>
        LogEntryWithSource &operator<<(const T &value)
        {
            m_messageStream << value;
            return *this;
        }

        LogEntryWithSource &operator<<(std::ostream &(*manip)(std::ostream &) )
        {
            m_messageStream << manip;
            return *this;
        }

        LogEntryWithSource &operator<<(const std::string &message)
        {
            m_messageStream << message;
            return *this;
        }

        LogEntryWithSource &operator<<(const char *message)
        {
            m_messageStream << message;
            return *this;
        }

    private:
        ILogger *m_logger;
        LogLevel m_level;
        std::string m_file;
        int m_line;
        std::string m_func;
        bool m_isActive;
        LogMessageStream m_messageStream;
    };

    class LogEntrySimple
    {
    public:
        LogEntrySimple(ILogger *logger, const LogLevel level)
            : m_logger(logger)
            , m_level(level)
            , m_isActive(logger && level >= logger->getMinLevel())
            , m_messageStream(m_isActive)
        {}

        ~LogEntrySimple()
        {
            if (m_isActive) {
                m_logger->log(m_level, m_messageStream.str());
            }
        }

        template<typename T>
        LogEntrySimple &operator<<(const T &value)
        {
            m_messageStream << value;
            return *this;
        }

        LogEntrySimple &operator<<(std::ostream &(*manip)(std::ostream &) )
        {
            m_messageStream << manip;
            return *this;
        }

        LogEntrySimple &operator<<(const std::string &message)
        {
            m_messageStream << message;
            return *this;
        }

        LogEntrySimple &operator<<(const char *message)
        {
            m_messageStream << message;
            return *this;
        }

    private:
        ILogger *m_logger;
        LogLevel m_level;
        bool m_isActive;
        LogMessageStream m_messageStream;
    };

    [[nodiscard]] LogEntryWithSource traceWithSource(
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        return LogEntryWithSource{const_cast<ILogger *>(this), LogLevel::Trace, file, line, func};
    }

    [[nodiscard]] LogEntryWithSource debugWithSource(
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        return LogEntryWithSource{const_cast<ILogger *>(this), LogLevel::Debug, file, line, func};
    }

    [[nodiscard]] LogEntryWithSource infoWithSource(
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        return LogEntryWithSource{const_cast<ILogger *>(this), LogLevel::Info, file, line, func};
    }

    [[nodiscard]] LogEntryWithSource warnWithSource(
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        return LogEntryWithSource{const_cast<ILogger *>(this), LogLevel::Warn, file, line, func};
    }

    [[nodiscard]] LogEntryWithSource errorWithSource(
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        return LogEntryWithSource{const_cast<ILogger *>(this), LogLevel::Error, file, line, func};
    }

    [[nodiscard]] LogEntryWithSource fatalWithSource(
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        return LogEntryWithSource{const_cast<ILogger *>(this), LogLevel::Fatal, file, line, func};
    }

    [[nodiscard]] LogEntrySimple trace() const
    {
        return LogEntrySimple{const_cast<ILogger *>(this), LogLevel::Trace};
    }
    [[nodiscard]] LogEntrySimple debug() const
    {
        return LogEntrySimple{const_cast<ILogger *>(this), LogLevel::Debug};
    }
    [[nodiscard]] LogEntrySimple info() const
    {
        return LogEntrySimple{const_cast<ILogger *>(this), LogLevel::Info};
    }
    [[nodiscard]] LogEntrySimple warn() const
    {
        return LogEntrySimple{const_cast<ILogger *>(this), LogLevel::Warn};
    }
    [[nodiscard]] LogEntrySimple error() const
    {
        return LogEntrySimple{const_cast<ILogger *>(this), LogLevel::Error};
    }
    [[nodiscard]] LogEntrySimple fatal() const
    {
        return LogEntrySimple{const_cast<ILogger *>(this), LogLevel::Fatal};
    }

    void trace(const std::string &msg) const { trace() << msg; }
    void debug(const std::string &msg) const { debug() << msg; }
    void info(const std::string &msg) const { info() << msg; }
    void warn(const std::string &msg) const { warn() << msg; }
    void error(const std::string &msg) const { error() << msg; }
    void fatal(const std::string &msg) const { fatal() << msg; }

    void traceWithSource(
        const std::string &msg,
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        traceWithSource(file, line, func) << msg;
    }

    void debugWithSource(
        const std::string &msg,
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        debugWithSource(file, line, func) << msg;
    }

    void infoWithSource(
        const std::string &msg,
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        infoWithSource(file, line, func) << msg;
    }

    void warnWithSource(
        const std::string &msg,
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        warnWithSource(file, line, func) << msg;
    }

    void errorWithSource(
        const std::string &msg,
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        errorWithSource(file, line, func) << msg;
    }

    void fatalWithSource(
        const std::string &msg,
        const std::string &file = __builtin_FILE(),
        const int line = __builtin_LINE(),
        const std::string &func = __builtin_FUNCTION()) const
    {
        fatalWithSource(file, line, func) << msg;
    }
};
