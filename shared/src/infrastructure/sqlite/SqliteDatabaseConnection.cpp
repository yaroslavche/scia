#include <infrastructure/sqlite/SqliteDatabaseConnection.hpp>

SqliteDatabaseConnection::SqliteDatabaseConnection(std::string dbPath)
    : m_dbPath(std::move(dbPath))
{
    m_db = nullptr;
}

SqliteDatabaseConnection::~SqliteDatabaseConnection()
{
    close();
}

void SqliteDatabaseConnection::open(const std::string &connectionString)
{
    if (m_db) {
        close();
    }

    const std::string path_to_open = connectionString.empty() ? m_dbPath : connectionString;

    if (const int rc = sqlite3_open(path_to_open.c_str(), &m_db); rc != SQLITE_OK) {
        if (m_db) {
            sqlite3_close(m_db);
            m_db = nullptr;
        }
    }
}

void SqliteDatabaseConnection::close()
{
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}
