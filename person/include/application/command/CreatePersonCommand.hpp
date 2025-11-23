#pragma once

#include <application/cqrs/ICommand.hpp>
#include <string>

struct CreatePersonCommand final : ICommand
{
    std::string name;
    bool active;

    CreatePersonCommand() = default;
    explicit CreatePersonCommand(const std::string &name, const bool active)
        : name(name)
        , active(active)
    {}
};
