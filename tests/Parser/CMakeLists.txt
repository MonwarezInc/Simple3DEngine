cmake_minimum_required(VERSION 3.2)

project(test-Parser)

set(SOURCES
  test_main.cpp
  test_parser.cpp
  test_parser.h
  )


include_directories("./")
add_executable(test-Parser ${SOURCES})
target_link_libraries(test-Parser Simple3DEngine)
target_link_libraries(test-Parser gtest gtest_main)
target_link_libraries(test-Parser gmock gmock_main)

add_test(NAME test-Parser COMMAND test-Parser)

include(../../common.cmake)
