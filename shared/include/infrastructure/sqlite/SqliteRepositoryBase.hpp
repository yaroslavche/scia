#pragma once

#include <application/port/IDatabaseConnection.hpp>
#include <application/port/ILogger.hpp>
#include <chrono>
#include <functional>
#include <infrastructure/sqlite/RowReader.hpp>
#include <iomanip>
#include <optional>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>

class SqliteRepositoryBase
{
public:
    explicit SqliteRepositoryBase(ILogger *logger, IDatabaseConnection *databaseConnection)
        : m_logger(logger)
        , m_databaseConnection(databaseConnection)
    {
        if (!m_databaseConnection || m_databaseConnection->getType() != DatabaseType::SQLite) {
            throw std::invalid_argument(
                "Provided IDatabaseConnection is not a SQLite connection or is null.");
        }
    }

    [[nodiscard]] IDatabaseConnection *getDatabaseConnection() const
    {
        return m_databaseConnection;
    }
    [[nodiscard]] sqlite3 *db() const
    {
        return static_cast<sqlite3 *>(m_databaseConnection->getNativeHandle());
    }

protected:
    [[nodiscard]] sqlite3_stmt *prepare(const std::string &sql) const
    {
        sqlite3_stmt *stmt = nullptr;
        if (const int rc = sqlite3_prepare_v2(db(), sql.c_str(), -1, &stmt, nullptr);
            rc != SQLITE_OK) {
            throw std::runtime_error("SQLite prepare failed: " + getLastError());
        }
        return stmt;
    }

    void exec(const std::string &sql) const
    {
        char *errMsg = nullptr;
        if (const int rc = sqlite3_exec(db(), sql.c_str(), nullptr, nullptr, &errMsg);
            rc != SQLITE_OK) {
            const std::string msg = errMsg ? errMsg : "Unknown SQLite error";
            sqlite3_free(errMsg);
            throw std::runtime_error("SQLite exec failed: " + msg);
        }
    }

    void step(sqlite3_stmt *stmt) const
    {
        if (const int rc = sqlite3_step(stmt); rc != SQLITE_DONE && rc != SQLITE_ROW) {
            throw std::runtime_error("SQLite step failed: " + getLastError());
        }
    }

    static void finalize(sqlite3_stmt *stmt) { sqlite3_finalize(stmt); }

    [[nodiscard]] std::string getLastError() const { return sqlite3_errmsg(db()); }

    template<typename T, typename Mapper, typename... Args>
    std::vector<T> find(const std::string &sql, Mapper mapper, Args &&...args)
    {
        m_logger->trace() << sql;
        sqlite3_stmt *stmt = prepare(sql);
        bindAll(stmt, std::forward<Args>(args)...);

        std::vector<T> result;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            RowReader reader(stmt);
            result.push_back(mapper(reader));
        }

        finalize(stmt);
        m_logger->trace() << result.size() << " rows found";
        return result;
    }

    template<typename T, typename Mapper, typename... Args>
    std::optional<T> findOne(const std::string &sql, Mapper mapper, Args &&...args)
    {
        sqlite3_stmt *stmt = prepare(sql);
        bindAll(stmt, std::forward<Args>(args)...);

        std::optional<T> result;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            RowReader reader(stmt);
            result = mapper(reader);
        }

        finalize(stmt);
        return result;
    }

    template<typename T, typename Mapper, typename... Args>
    auto stream(const std::string &sql, Mapper mapper, Args &&...args)
        -> std::function<std::optional<T>()>
    {
        sqlite3_stmt *stmt = prepare(sql);
        bindAll(stmt, std::forward<Args>(args)...);

        return [stmt, mapper]() mutable -> std::optional<T> {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                RowReader reader(stmt);
                return mapper(reader);
            }
            finalize(stmt);
            return std::nullopt;
        };
    }

    template<typename... Args>
    auto executeInsert(const std::string &sql, Args &&...args) -> std::optional<int64_t>
    {
        executeInternal(sql, std::forward<Args>(args)...);
        return sqlite3_last_insert_rowid(db());
    }

    template<typename... Args>
    auto executeUpdate(const std::string &sql, Args &&...args) -> std::optional<int64_t>
    {
        executeInternal(sql, std::forward<Args>(args)...);
        return sqlite3_changes(db());
    }

    template<typename... Args>
    auto executeDelete(const std::string &sql, Args &&...args) -> std::optional<int64_t>
    {
        executeInternal(sql, std::forward<Args>(args)...);
        return sqlite3_changes(db());
    }

private:
    ILogger *m_logger = nullptr;
    IDatabaseConnection *m_databaseConnection = nullptr;

    template<typename... Args>
    void executeInternal(const std::string &sql, Args &&...args)
    {
        auto stmt = prepare(sql);
        bindAll(stmt, std::forward<Args>(args)...);
        step(stmt);
        finalize(stmt);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, const int value)
    {
        sqlite3_bind_int(stmt, index, value);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, const long long value)
    {
        sqlite3_bind_int64(stmt, index, value);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, const std::string &value)
    {
        sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, const std::vector<uint8_t> &value)
    {
        sqlite3_bind_blob(stmt, index, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, std::nullptr_t)
    {
        sqlite3_bind_null(stmt, index);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, const bool value)
    {
        sqlite3_bind_int(stmt, index, value);
    }

    static void bindParam(sqlite3_stmt *stmt, const int index, const double value)
    {
        sqlite3_bind_double(stmt, index, value);
    }

    template<typename T>
    static void bindParam(sqlite3_stmt *stmt, int index, const std::optional<T> &opt)
    {
        if (opt.has_value()) {
            bindParam(stmt, index, *opt);
        } else {
            sqlite3_bind_null(stmt, index);
        }
    }

    static void bindParam(
        sqlite3_stmt *stmt, const int index, const std::chrono::system_clock::time_point &value)
    {
        const std::string dtString = timePointToSqliteString(value);
        bindParam(stmt, index, dtString);
    }

    template<typename T>
    static void bindParam([[maybe_unused]] sqlite3_stmt *stmt, [[maybe_unused]] int index, T &&)
    {
        static_assert(sizeof(T) == 0, "Unsupported bindParam type");
    }

    template<typename... Args>
    void bindAll(sqlite3_stmt *stmt, Args &&...args) const
    {
        int index = 1;
        (bindParam(stmt, index++, std::forward<Args>(args)), ...);
    }

    static std::string timePointToSqliteString(const std::chrono::system_clock::time_point &tp)
    {
        const auto tt = std::chrono::system_clock::to_time_t(tp);
        const std::tm tm = *std::gmtime(&tt);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};
