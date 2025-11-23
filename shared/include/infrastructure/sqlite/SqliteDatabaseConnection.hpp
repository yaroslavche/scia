#pragma once

#include <application/port/IDatabaseConnection.hpp>
#include <sqlite3.h>

class SqliteDatabaseConnection final : public IDatabaseConnection
{
public:
    explicit SqliteDatabaseConnection(std::string dbPath = {});
    ~SqliteDatabaseConnection() override;
    void open(const std::string &connectionString) override;
    void close() override;
    [[nodiscard]] bool isOpen() const override { return m_db != nullptr; }
    void *getNativeHandle() override { return m_db; }
    [[nodiscard]] DatabaseType getType() const override { return DatabaseType::SQLite; }

private:
    sqlite3 *m_db = nullptr;
    std::string m_dbPath;
};
