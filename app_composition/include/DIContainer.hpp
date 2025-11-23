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
    explicit DIContainer(const QString &projectName, QObject *parent = nullptr);
    ~DIContainer() override;
    DIContainer(const DIContainer &) = delete;
    DIContainer &operator=(const DIContainer &) = delete;

    [[nodiscard]] ILogger *getCompositeLogger() const { return m_compositeLogger.get(); }
    [[nodiscard]] QString getProjectName() const { return m_projectName; }

    PersonContext *getPersonContext();

private:
    const QString m_projectName;
    [[nodiscard]] ILogger *getConsoleLogger() const { return m_consoleLogger.get(); }
    [[nodiscard]] ILogger *getFileLogger() const { return m_fileLogger.get(); }
    [[nodiscard]] IDatabaseConnection *getDatabaseConnection() const;

    std::unique_ptr<ILogger> m_consoleLogger;
    std::unique_ptr<ILogger> m_fileLogger;
    std::unique_ptr<CompositeLogger> m_compositeLogger;
    std::unique_ptr<IDatabaseConnection> m_databaseConnection;

    std::unique_ptr<PersonContext> m_personContext;
};
