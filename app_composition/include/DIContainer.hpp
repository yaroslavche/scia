#pragma once

#include <application/composition/PersonContext.hpp>
#include <application/port/IDatabaseConnection.hpp>
#include <application/port/ILogger.hpp>
#include <infrastructure/CompositeLogger.hpp>
#include <QObject>

class DIContainer final : public QObject
{
    Q_OBJECT

public:
    explicit DIContainer(QObject *parent = nullptr);
    ~DIContainer() override;
    DIContainer(const DIContainer &) = delete;
    DIContainer &operator=(const DIContainer &) = delete;

    [[nodiscard]] ILogger *getCompositeLogger() const;

    PersonContext *getPersonContext();

private:
    [[nodiscard]] ILogger *getConsoleLogger() const;
    [[nodiscard]] ILogger *getFileLogger() const;
    [[nodiscard]] IDatabaseConnection *getDatabaseConnection() const;

    std::unique_ptr<ILogger> m_consoleLogger;
    std::unique_ptr<ILogger> m_fileLogger;
    std::unique_ptr<CompositeLogger> m_compositeLogger;
    std::unique_ptr<IDatabaseConnection> m_databaseConnection;

    std::unique_ptr<PersonContext> m_personContext;
};
