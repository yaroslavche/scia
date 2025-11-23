#pragma once

#include <application/cqrs/IQuery.hpp>

class FindByIdPersonQuery final : public IQuery
{
public:
    [[nodiscard]] explicit FindByIdPersonQuery(const long long id)
        : m_id(id)
    {}

    [[nodiscard]] long long getId() const { return m_id; }

private:
    long long m_id = 0;
};
