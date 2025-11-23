#pragma once

#include <domain/aggregate/Person.hpp>
#include <optional>
#include <vector>

class IPersonRepository
{
public:
    virtual ~IPersonRepository() = default;
    virtual long long save(const Person &person) = 0;
    virtual void remove(long long id) = 0;
    virtual std::vector<Person> findAll() = 0;
    virtual std::optional<Person> findById(long long id) = 0;
};
