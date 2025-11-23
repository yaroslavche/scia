#pragma once

#include <application/cqrs/ICommand.hpp>

class ICommandHandler
{
public:
    virtual ~ICommandHandler() = default;
    virtual void handle(const ICommand &command) = 0;
};
