project(demo)

find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)

add_library(backend database.cpp password.cpp)

set(SOURCES
    main.cpp
)

add_executable(demo ${SOURCES})

target_link_libraries(${PROJECT_NAME} Crow::Crow SQLite::SQLite3 backend)

target_include_directories(${PROJECT_NAME} PRIVATE src)