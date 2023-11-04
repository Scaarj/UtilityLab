cmake_minimum_required(VERSION 3.14)

project(library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(library SHARED
        TQueue_global.h
        tqueue.h
)

target_compile_definitions(library PRIVATE TQUEUE_LIBRARY)
set_target_properties(library PROPERTIES LINKER_LANGUAGE CXX)
