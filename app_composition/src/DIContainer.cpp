#include <DIContainer.hpp>
#include <infrastructure/CompositeLogger.hpp>
#include <infrastructure/ConsoleLogger.hpp>
#include <infrastructure/FileLogger.hpp>
#include <infrastructure/sqlite/SqliteDatabaseInitializer.hpp>

DIContainer::DIContainer(QObject *parent)
    : QObject(parent)
{
    m_consoleLogger = std::make_unique<ConsoleLogger>(LogLevel::Trace);
    m_fileLogger = std::make_unique<FileLogger>("logs/app.log", LogLevel::Trace);
    m_compositeLogger = std::make_unique<CompositeLogger>();
    m_compositeLogger->addLogger(getConsoleLogger(), LogTarget::Console);
    m_compositeLogger->addLogger(getFileLogger(), LogTarget::File);
    getCompositeLogger()->info("Application started");
    const auto databaseInitializer = std::make_unique<SqliteDatabaseInitializer>(
        getCompositeLogger(),
        "scia.db",
        QStringList{":/db/migrations/001_init.sql"},
        QStringList{":/db/fixtures/001_seed_data.sql"});

    m_databaseConnection = databaseInitializer->init();
    if (!m_databaseConnection || !m_databaseConnection->isOpen()) {
        getCompositeLogger()->fatal("Failed to open database");
    }
}

DIContainer::~DIContainer()
{
    getCompositeLogger()->info("Application closing");
}

ILogger *DIContainer::getConsoleLogger() const
{
    return m_consoleLogger.get();
}

ILogger *DIContainer::getFileLogger() const
{
    return m_fileLogger.get();
}

ILogger *DIContainer::getCompositeLogger() const
{
    return m_compositeLogger.get();
}

IDatabaseConnection *DIContainer::getDatabaseConnection() const
{
    if (!m_databaseConnection || !m_databaseConnection->isOpen()) {
        getConsoleLogger()->fatal(
            "Attempted to get DB connection, but it's not open or not initialized!");
        return nullptr;
    }
    return m_databaseConnection.get();
}

PersonContext *DIContainer::getPersonContext()
{
    if (!m_personContext) {
        m_personContext
            = std::make_unique<PersonContext>(getCompositeLogger(), getDatabaseConnection());
    }
    return m_personContext.get();
}
