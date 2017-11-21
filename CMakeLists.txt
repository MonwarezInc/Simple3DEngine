cmake_minimum_required(VERSION 3.2)

#Configuration of project
#Here is the library
project(Simple3DEngine)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

#Include and Library filepath
include_directories("src")
#Add prefix library
link_directories(${CMAKE_PREFIX}/lib)
option(USE_SHARED "Build shared lib" ON)
option(USE_PROFILING "Enable profiling" OFF)
option(USE_DSA "Use Direct State Access" ON)
option(test "Build with test" ON)
option(asan "Build with asan enabled" ON)

# Profiling 
if (USE_PROFILING)
    message("Use of profiling")
    if(CMAKE_COMPILER_IS_GNUXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fPIC")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
    endif()
endif ()

if (USE_DSA)
    add_definitions(-DS3DE_USE_DSA)
endif()

if (win)
    set(GL opengl32)
endif ()

set(CMAKE_CXX_STANDARD 14)


if (test)

  add_subdirectory(googletest)

  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

  # here we will put coverage
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
  list(APPEND CMAKE_SHARED_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info
  COMMAND lcov --capture --no-external --initial --directory ${CMAKE_CURRENT_SOURCE_DIR} --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info
  DEPENDS Simple3DEngine test-Parser
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Pre-coverage report"
  )
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/out-coverage/index.html
  COMMAND lcov --capture --no-external --directory ${CMAKE_CURRENT_SOURCE_DIR} --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage-test.info
  COMMAND lcov --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage-test.info
  --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
  COMMAND lcov --extract ${CMAKE_CURRENT_BINARY_DIR}/coverage.info '*/src/*' -o ${CMAKE_CURRENT_BINARY_DIR}/filtered.info
  COMMAND genhtml --ignore-errors source ${CMAKE_CURRENT_BINARY_DIR}/filtered.info
  --prefix ${CMAKE_CURRENT_SOURCE_DIR} --keep-descriptions --frames --show-details --legend
  --output-directory ${CMAKE_CURRENT_BINARY_DIR}/out-coverage
  DEPENDS Simple3DEngine test-Parser
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating coverage report"
  )

  add_custom_target(pre_coverage DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info)
  add_custom_target(coverage_report DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/out-coverage/index.html)

endif()

add_subdirectory(src)

if (test)
  # add test here
  add_subdirectory(tests/Parser)
endif()

if (asan)
  add_definitions("-fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
endif()
