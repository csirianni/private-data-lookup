find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)

add_library(database database.cpp password.cpp)

add_executable(server main.cpp)

target_link_libraries(server Crow::Crow SQLite::SQLite3 database)

target_include_directories(server PRIVATE src)