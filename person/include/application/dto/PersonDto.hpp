#pragma once

#include <domain/aggregate/Person.hpp>
#include <string>

struct PersonDto
{
    long long id;
    std::string name;
    bool active;

    PersonDto() = default;

    [[nodiscard]] static PersonDto from(const Person &person)
    {
        PersonDto dto;
        dto.id = person.id;
        dto.name = person.name;
        dto.active = person.active;
        return dto;
    }
};