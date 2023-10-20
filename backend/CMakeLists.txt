cmake_minimum_required(VERSION 3.27)
project(backend)

find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)

set(SOURCES
    src/main.cpp
    src/database.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Crow::Crow SQLite::SQLite3)

target_include_directories(${PROJECT_NAME} PRIVATE src)
