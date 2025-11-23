#include <application/PersonQueryService.hpp>
#include <ranges>

std::vector<PersonDto> PersonQueryService::findAll(const FindAllPersonQuery &query)
{
    m_logger->trace("Loading ALL Person from DB");
    const auto result = m_personRepository->findAll();
    auto persons = result | std::views::transform([](const auto &person) {
                       return PersonDto::from(person);
                   });
    return {persons.begin(), persons.end()};
}

PersonDto PersonQueryService::findById(const FindByIdPersonQuery &query)
{
    m_logger->trace("Loading Person " + std::to_string(query.getId()) + " from DB");
    if (const auto result = m_personRepository->findById(query.getId())) {
        return PersonDto::from(*result);
    }
    return {};
}
