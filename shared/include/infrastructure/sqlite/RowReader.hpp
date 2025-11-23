#pragma once

#include <chrono>
#include <cstdint>
#include <sqlite3.h>
#include <string>
#include <unordered_map>
#include <vector>

class RowReader
{
public:
    explicit RowReader(sqlite3_stmt *stmt);

    int getInt(const std::string &column) const;
    long long getInt64(const std::string &column) const;
    std::string getText(const std::string &column) const;
    std::vector<std::uint8_t> getBlob(const std::string &column) const;
    bool getBool(const std::string &column) const;
    double getDouble(const std::string &column) const;
    std::chrono::system_clock::time_point getTimePoint(
        const std::string &column, const std::string &format = "%Y-%m-%d %H:%M:%S") const;
    template<typename E>
    E getEnum(const std::string &column) const
    {
        return static_cast<E>(getInt(column));
    }

private:
    sqlite3_stmt *stmt;
    std::unordered_map<std::string, int> indexMap;

    int getIndex(const std::string &column) const;
};
