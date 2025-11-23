#pragma once

#include <domain/aggregate/Person.hpp>
#include <infrastructure/sqlite/RowReader.hpp>

inline Person mapPerson(const RowReader &rowReader)
{
    return Person{rowReader.getInt64("id"), rowReader.getText("name"), rowReader.getBool("active")};
}
