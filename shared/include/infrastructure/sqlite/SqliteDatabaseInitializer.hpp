#pragma once

#include <application/port/IDatabaseInitializer.hpp>
#include <infrastructure/sqlite/SqliteDatabaseConnection.hpp>
#include <infrastructure/sqlite/SqliteInitializerRepository.hpp>
#include <memory>
#include <QStringList>

class SqliteDatabaseInitializer final : public IDatabaseInitializer
{
public:
    explicit SqliteDatabaseInitializer(
        ILogger *logger, QString dbPath, QStringList migrations = {}, QStringList fixtures = {})
        : m_logger(logger)
        , m_dbPath(std::move(dbPath))
        , m_migrations(std::move(migrations))
        , m_fixtures(std::move(fixtures))
    {}

    std::unique_ptr<IDatabaseConnection> init() override
    {
        auto connection = std::make_unique<SqliteDatabaseConnection>(m_dbPath.toStdString());
        connection->open(m_dbPath.toStdString());

        if (!connection->isOpen())
            throw std::runtime_error("Failed to open database: " + m_dbPath.toStdString());

        SqliteInitializerRepository repo(m_logger, connection.get());

        // for developing purposes only
        // repo.dropAllTables();
        repo.createMetaSchema();

        if (!m_migrations.isEmpty())
            repo.applyMigrations(m_migrations);

        if (!m_fixtures.isEmpty())
            repo.applyFixtures(m_fixtures);

        return connection;
    }

private:
    ILogger *m_logger;
    QString m_dbPath;
    QStringList m_migrations;
    QStringList m_fixtures;
};
