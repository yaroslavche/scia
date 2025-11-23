#pragma once

#include <domain/repository/IPersonRepository.hpp>
#include <infrastructure/sqlite/SqliteRepositoryBase.hpp>

class SqlitePersonRepository final : public SqliteRepositoryBase, public IPersonRepository
{
public:
    explicit SqlitePersonRepository(ILogger *logger, IDatabaseConnection *databaseConnection);

    long long save(const Person &person) override;
    void remove(long long id) override;
    std::vector<Person> findAll() override;
    std::optional<Person> findById(long long id) override;
};
