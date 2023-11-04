project(library_tests)

add_subdirectory(googletest)

add_executable(library_tests main.cpp queueTest.cpp)
target_link_libraries(library_tests gtest gtest_main )
