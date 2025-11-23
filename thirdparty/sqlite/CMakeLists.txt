cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME ${PROJECT_NAME}_third_party_sqlite3)

add_library(${TARGET_NAME}
        STATIC
        sqlite3.c
)
set_source_files_properties(sqlite3.c PROPERTIES LANGUAGE C)

target_include_directories(${TARGET_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${TARGET_NAME}
        PRIVATE
        SQLITE_THREADSAFE=1
        SQLITE_ENABLE_FTS5
)

add_library(${PROJECT_NAME}::Sqlite ALIAS ${TARGET_NAME})
