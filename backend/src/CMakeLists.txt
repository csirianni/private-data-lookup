find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(libsodium REQUIRED)
find_package(spdlog REQUIRED)

add_library(src database.cpp password.cpp server.cpp cryptography.cpp)
target_link_libraries(src Crow::Crow libsodium::libsodium spdlog::spdlog)

add_executable(server main.cpp)
target_link_libraries(server Crow::Crow SQLite::SQLite3 libsodium::libsodium spdlog::spdlog src)
target_include_directories(server PRIVATE src)