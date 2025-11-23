#include <application/PersonCommandService.hpp>
#include <application/PersonQueryService.hpp>
#include <application/composition/PersonContext.hpp>
#include <infrastructure/sqlite/SqlitePersonRepository.hpp>

IPersonCommandService *PersonContext::getPersonCommandService()
{
    return resolveDependency<IPersonCommandService, PersonCommandService>(
        m_personCommandService, m_logger, getPersonRepository());
}

IPersonQueryService *PersonContext::getPersonQueryService()
{
    return resolveDependency<IPersonQueryService, PersonQueryService>(
        m_personQueryService, m_logger, getPersonRepository());
}

IPersonRepository *PersonContext::getPersonRepository()
{
    return resolveDependency<IPersonRepository, SqlitePersonRepository>(
        m_personRepository, m_logger, m_databaseConnection);
}
