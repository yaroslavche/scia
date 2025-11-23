#pragma once

#include <application/cqrs/IQuery.hpp>

class IQueryHandler
{
public:
    virtual ~IQueryHandler() = default;
    virtual void handle(const IQuery &query) = 0;
};
