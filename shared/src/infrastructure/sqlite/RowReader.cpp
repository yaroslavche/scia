#include <ctime>
#include <infrastructure/sqlite/RowReader.hpp>
#include <iomanip>
#include <stdexcept>

RowReader::RowReader(sqlite3_stmt *stmt)
    : stmt(stmt)
{
    const int count = sqlite3_column_count(stmt);
    for (int i = 0; i < count; ++i) {
        const char *name = sqlite3_column_name(stmt, i);
        indexMap[name] = i;
    }
}

int RowReader::getIndex(const std::string &column) const
{
    const auto it = indexMap.find(column);
    if (it == indexMap.end())
        throw std::runtime_error("Column not found: " + column);
    return it->second;
}

int RowReader::getInt(const std::string &column) const
{
    return sqlite3_column_int(stmt, getIndex(column));
}

long long RowReader::getInt64(const std::string &column) const
{
    return sqlite3_column_int64(stmt, getIndex(column));
}

std::string RowReader::getText(const std::string &column) const
{
    const unsigned char *text = sqlite3_column_text(stmt, getIndex(column));
    return text ? reinterpret_cast<const char *>(text) : "";
}

std::vector<uint8_t> RowReader::getBlob(const std::string &column) const
{
    const void *data = sqlite3_column_blob(stmt, getIndex(column));
    const int size = sqlite3_column_bytes(stmt, getIndex(column));
    if (!data)
        return {};
    const auto begin = static_cast<const uint8_t *>(data);
    return std::vector<uint8_t>{begin, begin + size};
}

bool RowReader::getBool(const std::string &column) const
{
    return getInt(column) != 0;
}

double RowReader::getDouble(const std::string &column) const
{
    return sqlite3_column_double(stmt, getIndex(column));
}

std::chrono::system_clock::time_point RowReader::getTimePoint(
    const std::string &column, const std::string &format) const
{
    std::tm tmStruct = {};
    std::istringstream ss(getText(column));

    ss >> std::get_time(&tmStruct, format.c_str());

    if (ss.fail()) {
        throw std::runtime_error("Failed to parse datetime string for column: " + column);
    }

    const std::time_t time = std::mktime(&tmStruct);

    return std::chrono::system_clock::from_time_t(time);
}
