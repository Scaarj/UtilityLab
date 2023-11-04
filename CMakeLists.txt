cmake_minimum_required(VERSION 3.14)

project(UtilityLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(library library)

include_directories(lib)

add_subdirectory(lib)
add_subdirectory(tests)