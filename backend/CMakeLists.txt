cmake_minimum_required(VERSION 3.27)

project(pdl LANGUAGES CXX)

add_subdirectory("src")
add_subdirectory("test")

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD_20)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined,float-divide-by-zero,float-cast-overflow, -fno-sanitize=all,null,alignment")
