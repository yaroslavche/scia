#include <infrastructure/sqlite/PersonMapper.hpp>
#include <infrastructure/sqlite/ScopedTransaction.hpp>
#include <infrastructure/sqlite/SqlitePersonRepository.hpp>

SqlitePersonRepository::SqlitePersonRepository(
    ILogger *logger, IDatabaseConnection *databaseConnection)
    : SqliteRepositoryBase(logger, databaseConnection)
{}

long long SqlitePersonRepository::save(const Person &person)
{
    ScopedTransaction tx(getDatabaseConnection());
    const auto id = executeInsert("INSERT INTO Person (name) VALUES (?);", person.name);
    tx.commit();
    return id.value();
}

void SqlitePersonRepository::remove(const long long id)
{
    ScopedTransaction tx(getDatabaseConnection());
    executeDelete("DELETE FROM Person WHERE id = ?;", id);
    tx.commit();
}

std::vector<Person> SqlitePersonRepository::findAll()
{
    return find<Person>("SELECT * FROM Person;", mapPerson);
}

std::optional<Person> SqlitePersonRepository::findById(const long long id)
{
    return findOne<Person>("SELECT * FROM Person WHERE id = ?;", mapPerson, id);
}
