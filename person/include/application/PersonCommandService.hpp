#pragma once

#include <application/port/ILogger.hpp>
#include <application/port/IPersonCommandService.hpp>
#include <domain/repository/IPersonRepository.hpp>

class PersonCommandService final : public IPersonCommandService
{
public:
    explicit PersonCommandService(ILogger *logger, IPersonRepository *personRepository)
        : m_logger(logger)
        , m_personRepository(personRepository)
    {}

    ~PersonCommandService() override = default;
    void execute(const CreatePersonCommand &command) override;
    void execute(const DeletePersonCommand &command) override;

private:
    ILogger *m_logger;
    IPersonRepository *m_personRepository;
};
