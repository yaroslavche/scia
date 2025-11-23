#pragma once

#include <string>

enum class DatabaseType {
    SQLite,
};

class IDatabaseConnection
{
public:
    virtual ~IDatabaseConnection() = default;
    virtual void open(const std::string &connectionString) = 0;
    virtual void close() = 0;
    [[nodiscard]] virtual bool isOpen() const = 0;
    virtual void *getNativeHandle() = 0;
    [[nodiscard]] virtual DatabaseType getType() const = 0;
};
