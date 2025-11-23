#pragma once

#include <application/port/IDatabaseConnection.hpp>
#include <application/port/ILogger.hpp>
#include <memory>
#include <utility>

class BaseContext
{
public:
    BaseContext(ILogger *logger, IDatabaseConnection *databaseConnection)
        : m_logger(logger)
        , m_databaseConnection(databaseConnection)
    {}
    virtual ~BaseContext() = default;

protected:
    ILogger *m_logger;
    IDatabaseConnection *m_databaseConnection;

    template<typename Interface, typename Implementation, typename... Args>
    Interface *resolveDependency(std::unique_ptr<Interface> &memberPtr, Args &&...args)
    {
        if (!memberPtr)
            memberPtr = std::make_unique<Implementation>(std::forward<Args>(args)...);
        return memberPtr.get();
    }
};
