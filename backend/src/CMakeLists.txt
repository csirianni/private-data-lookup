find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(libsodium REQUIRED)

add_library(src database.cpp password.cpp server.cpp)
target_link_libraries(src Crow::Crow)

add_executable(server main.cpp)
target_link_libraries(server Crow::Crow SQLite::SQLite3 libsodium::libsodium src)
target_include_directories(server PRIVATE src)