#pragma once

#include <application/dto/PersonDto.hpp>
#include <application/query/FindAllPersonQuery.hpp>
#include <application/query/FindByIdPersonQuery.hpp>
#include <vector>

struct IPersonQueryService
{
    virtual ~IPersonQueryService() = default;
    virtual std::vector<PersonDto> findAll(const FindAllPersonQuery &query) = 0;
    virtual PersonDto findById(const FindByIdPersonQuery &query) = 0;
};
