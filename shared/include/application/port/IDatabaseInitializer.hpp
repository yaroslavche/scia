#pragma once

#include <application/port/IDatabaseConnection.hpp>
#include <memory>

class ILogger;

class IDatabaseInitializer
{
public:
    virtual ~IDatabaseInitializer() = default;

    virtual std::unique_ptr<IDatabaseConnection> init() = 0;
};
