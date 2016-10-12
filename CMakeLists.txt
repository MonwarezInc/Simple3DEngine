# Use version 2.8 because of Travis
cmake_minimum_required(VERSION 2.8)

#Configuration of project
#Here is the library
project(Simple3DEngine)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(include/S3DE)
#Add prefix library
link_directories(${CMAKE_PREFIX}/lib)
#Generate source file list
file(
    GLOB_RECURSE
    include_files
    include/S3DE/*
)
file(
    GLOB_RECURSE
    source_file
    src/*
    include/S3DE/*
)
set(USE_GLM_BUNDLE "FALSE" CACHE BOOL "use bundle glm")
set(WITH_ASSERT "TRUE" CACHE BOOL "Enable Assert")
set(USE_SHARED "TRUE" CACHE BOOL "Build shared lib")
set(OLD_CXX "FALSE" CACHE BOOL "use c++1y")
set(USE_PROFILING "FALSE" CACHE BOOL "Enable profiling with gprof May not work")

if (OLD_CXX)
    set(Extra_CXX_Flags "-Wall -std=c++1y")
else (OLD_CXX)
    set(Extra_CXX_Flags "-Wall -std=c++1z")
endif (OLD_CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
# Profiling 
if (USE_PROFILING)
    message("Use of profiling, warning linking can fail")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif ()
if (win)
    set(GL opengl32)
endif ()

#Declaration of executable
if (USE_SHARED)
    add_library(
        Simple3DEngine
        SHARED
        ${source_file}
    )
else ()
target_link_libraries(
    Simple3DEngine
    ${GL}
    SDL2
    SDL2_image
    GLEW
    assimp
)
endif ()

if(UNIX)
    include(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image >=2.0.0)
    PKG_SEARCH_MODULE(ASSIMP REQUIRED assimp >= 3.0)
    PKG_SEARCH_MODULE(GLEW REQUIRED glew)
else ()
    message("Please Define the followings path")
    message("SDL2_INCLUDE_DIRS, SDL2IMAGE_INCLUDE_DIRS")
    message("SDL2_LIBRARIES, SDL2IMAGE_LIBRARIES")
    message("ASSIMP_INCLUDE_DIRS, ASSIMP_LIBRARIES")
    message("GLEW_INCLUDE_DIRS, GLEW_LIBRARIES")
endif ()
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
if (USE_GLM_BUNDLE)
	include_directories(include/bundle)
endif()
if (WITH_ASSERT)
    message("Enable Assert")
else ()
    message("Disable Assert")
    add_definitions(-DNDEBUG)
endif()
# Install stuff
install(FILES ${include_files} DESTINATION include/S3DE)
install(TARGETS Simple3DEngine LIBRARY DESTINATION lib)
