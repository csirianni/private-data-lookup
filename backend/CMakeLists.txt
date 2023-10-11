cmake_minimum_required(VERSION 3.27)
project(backend)

find_package(Crow REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} Crow::Crow)