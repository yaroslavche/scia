#pragma once

#include <application/command/CreatePersonCommand.hpp>
#include <application/command/DeletePersonCommand.hpp>

struct IPersonCommandService
{
    virtual ~IPersonCommandService() = default;
    virtual void execute(const CreatePersonCommand &command) = 0;
    virtual void execute(const DeletePersonCommand &command) = 0;
};
