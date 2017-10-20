cmake_minimum_required(VERSION 3.2)

#Configuration of project
#Here is the library
project(Simple3DEngine)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(include/S3DE)
#Add prefix library
link_directories(${CMAKE_PREFIX}/lib)
option(USE_SHARED "Build shared lib" ON)
option(USE_PROFILING "Enable profiling" OFF)
option(USE_DSA "Use Direct State Access" ON)
option(test "Build with test" ON)
option(asan "Build with asan enabled" OFF)

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

add_subdirectory(src)


if (test)

  add_subdirectory(googletest)

  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

  # here we will put coverage

endif()

if (asan)
  add_definitions("-fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()
