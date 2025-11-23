cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME AppComposition)

file(GLOB_RECURSE SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${TARGET_NAME} STATIC ${SRC_FILES})

target_include_directories(${TARGET_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

target_link_libraries(${TARGET_NAME}
        PRIVATE
        scia::Person
        scia::Shared
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
)

add_library(scia::AppComposition ALIAS ${TARGET_NAME})
