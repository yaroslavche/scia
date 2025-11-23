#include <application/PersonCommandService.hpp>

void PersonCommandService::execute(const CreatePersonCommand &command)
{
    auto person = Person{-1, command.name, command.active};
    const auto id = m_personRepository->save(person);
    person.id = id;
    m_logger->info("Person created with id: " + std::to_string(id));
}

void PersonCommandService::execute(const DeletePersonCommand &command)
{
    m_personRepository->remove(command.id);
    m_logger->info("Person deleted with id: " + std::to_string(command.id));
}
