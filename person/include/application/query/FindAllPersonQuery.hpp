#pragma once

#include <application/cqrs/IQuery.hpp>

class FindAllPersonQuery final : public IQuery
{
public:
    explicit FindAllPersonQuery() = default;
};
