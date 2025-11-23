#pragma once

#include <application/composition/BaseContext.hpp>
#include <application/port/IPersonCommandService.hpp>
#include <application/port/IPersonQueryService.hpp>
#include <domain/repository/IPersonRepository.hpp>
#include <memory>

class PersonContext final : public BaseContext
{
public:
    using BaseContext::BaseContext;
    ~PersonContext() override = default;

    IPersonCommandService *getPersonCommandService();
    IPersonQueryService *getPersonQueryService();

private:
    IPersonRepository *getPersonRepository();

    std::unique_ptr<IPersonRepository> m_personRepository;
    std::unique_ptr<IPersonCommandService> m_personCommandService;
    std::unique_ptr<IPersonQueryService> m_personQueryService;
};
