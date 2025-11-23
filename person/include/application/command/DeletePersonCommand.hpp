#pragma once

#include <application/cqrs/ICommand.hpp>

struct DeletePersonCommand final : ICommand
{
    long long id;

    DeletePersonCommand() = default;
    explicit DeletePersonCommand(const long long id)
        : id(id)
    {}
};
