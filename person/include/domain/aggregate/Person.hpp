#pragma once

#include <string>

struct Person
{
    long long id;
    std::string name;
    bool active;

    [[nodiscard]] Person(const long long id, const std::string &name, const bool active)
        : id(id)
        , name(name)
        , active(active)
    {}
};
