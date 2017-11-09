cmake_minimum_required(VERSION 3.2)

project(test-Parser)

set(SOURCES
  test_main.cpp
  test_parser.cpp
  test_parser.h
  )


include_directories("./")
add_executable(${PROJECT_NAME} ${SOURCES})

include(../test-link.cmake)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

include(../../common.cmake)
