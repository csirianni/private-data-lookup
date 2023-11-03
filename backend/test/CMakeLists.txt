find_package(Crow REQUIRED)
find_package(Catch2 REQUIRED)

set (TEST_SOURCE
    password.cpp
    server.cpp
)

add_executable(pdl_test ${TEST_SOURCE})

target_link_libraries(pdl_test Crow::Crow Catch2::Catch2WithMain)

target_include_directories(pdl_test PUBLIC ${CMAKE_SOURCE_DIR}/src)