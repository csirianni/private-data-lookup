find_package(Catch2 REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(spdlog REQUIRED)

set (TEST_SOURCE
    password.cpp
    server.cpp
    cryptography.cpp
    database.cpp
)

add_executable(pdl_test ${TEST_SOURCE})

target_link_libraries(pdl_test src Catch2::Catch2WithMain nlohmann_json::nlohmann_json SQLite::SQLite3 spdlog::spdlog)

target_include_directories(pdl_test PUBLIC ${CMAKE_SOURCE_DIR}/src)