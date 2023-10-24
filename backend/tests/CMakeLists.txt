project(tests)

find_package(Catch2 REQUIRED)

set (TEST_SOURCE
    testPasswordGenerator.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SOURCE})

target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain backend)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
