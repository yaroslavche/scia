#pragma once

#include <application/port/ILogger.hpp>
#include <application/port/IPersonQueryService.hpp>
#include <domain/repository/IPersonRepository.hpp>

class PersonQueryService final : public IPersonQueryService
{
public:
    explicit PersonQueryService(ILogger *logger, IPersonRepository *personRepository)
        : m_logger(logger)
        , m_personRepository(personRepository)
    {}
    ~PersonQueryService() override = default;

    std::vector<PersonDto> findAll(const FindAllPersonQuery &query) override;
    PersonDto findById(const FindByIdPersonQuery &query) override;

private:
    ILogger *m_logger;
    IPersonRepository *m_personRepository;
};
