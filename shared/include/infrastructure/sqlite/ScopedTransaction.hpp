#pragma once

#include <application/port/IDatabaseConnection.hpp>
#include <sqlite3.h>
#include <stdexcept>

class ScopedTransaction
{
public:
    explicit ScopedTransaction(IDatabaseConnection *connection)
        : m_databaseConnection(connection)
        , m_active(true)
    {
        if (!m_databaseConnection)
            throw std::invalid_argument("ScopedTransaction: DB is null");

        if (sqlite3_exec(db(), "BEGIN", nullptr, nullptr, nullptr) != SQLITE_OK)
            throw std::runtime_error("Failed to begin transaction");
    }

    void commit()
    {
        if (m_active && sqlite3_exec(db(), "COMMIT", nullptr, nullptr, nullptr) == SQLITE_OK) {
            m_active = false;
        } else if (m_active) {
            throw std::runtime_error("Failed to commit transaction");
        }
    }

    void rollback()
    {
        if (m_active && sqlite3_exec(db(), "ROLLBACK", nullptr, nullptr, nullptr) == SQLITE_OK) {
            m_active = false;
        } else if (m_active) {
            throw std::runtime_error("Failed to rollback transaction");
        }
    }

    ~ScopedTransaction()
    {
        if (m_active)
            sqlite3_exec(db(), "ROLLBACK", nullptr, nullptr, nullptr);
    }

private:
    IDatabaseConnection *m_databaseConnection;
    bool m_active;

    [[nodiscard]] sqlite3 *db() const
    {
        return static_cast<sqlite3 *>(m_databaseConnection->getNativeHandle());
    }
};
