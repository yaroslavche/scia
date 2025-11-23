#pragma once

#include <infrastructure/sqlite/SqliteRepositoryBase.hpp>
#include <stdexcept>
#include <string>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>

class SqliteInitializerRepository final : public SqliteRepositoryBase
{
public:
    using SqliteRepositoryBase::SqliteRepositoryBase;

    void createMetaSchema() const
    {
        exec(
            "CREATE TABLE IF NOT EXISTS migration_meta (id INTEGER PRIMARY KEY, name TEXT, "
            "applied_at TEXT)");
        exec(
            "CREATE TABLE IF NOT EXISTS fixture_meta (id INTEGER PRIMARY KEY, name TEXT, "
            "applied_at TEXT)");
    }

    bool isMigrationApplied(const std::string &name)
    {
        const std::string sql = "SELECT COUNT(1) AS count FROM migration_meta WHERE name = ?;";
        const auto mapper = [](const RowReader &r) { return r.getInt("count"); };
        return findOne<int>(sql, mapper, name).value_or(0) > 0;
    }

    void markMigrationApplied(const std::string &name)
    {
        executeInsert(
            "INSERT INTO migration_meta (name, applied_at) VALUES (?, datetime('now'));", name);
    }

    bool isFixtureApplied(const std::string &name)
    {
        const std::string sql = "SELECT COUNT(1) AS count FROM fixture_meta WHERE name = ?;";
        const auto mapper = [](const RowReader &r) { return r.getInt("count"); };
        return findOne<int>(sql, mapper, name).value_or(0) > 0;
    }

    void markFixtureApplied(const std::string &name)
    {
        executeInsert(
            "INSERT INTO fixture_meta (name, applied_at) VALUES (?, datetime('now'));", name);
    }

    void applyMigrations(const QStringList &migrationFiles)
    {
        for (const auto &path : migrationFiles) {
            const QString name = QFileInfo(path).fileName();
            if (isMigrationApplied(name.toStdString()))
                continue;

            execResource(path);
            markMigrationApplied(name.toStdString());
        }
    }

    void applyFixtures(const QStringList &fixtureFiles)
    {
        for (const auto &path : fixtureFiles) {
            const QString name = QFileInfo(path).fileName();
            if (isFixtureApplied(name.toStdString()))
                continue;

            execResource(path);
            markFixtureApplied(name.toStdString());
        }
    }

    void dropAllTables()
    {
        const std::string sql
            = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
        const auto mapper = [](const RowReader &r) { return r.getText("name"); };
        for (const std::vector<std::string> tables = find<std::string>(sql, mapper);
             const auto &tableName : tables) {
            const std::string dropSql = "DROP TABLE IF EXISTS " + tableName + ";";
            exec(dropSql);
        }
    }

private:
    void execResource(const QString &resourcePath) const
    {
        QFile file(resourcePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            throw std::runtime_error("Cannot open resource file: " + resourcePath.toStdString());

        QTextStream in(&file);
        const QString sql = in.readAll();
        exec(sql.toStdString());
    }
};
