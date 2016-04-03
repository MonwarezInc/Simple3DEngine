cmake_minimum_required(VERSION 2.8)

#Configuration of project
#Here is the library
project(Simple3DEngine)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(include/S3DE)

#Generate source file list
file(
	GLOB_RECURSE
	source_file
	src/*
	include/S3DE/*
)
set(Extra_CXX_Flags "-Wall -std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(USE_GLM_BUNDLE "FALSE" CACHE BOOL "use bundle glm")
#Declaration of executable
add_library(
	Simple3DEngine
	STATIC
	${source_file}
)
if(UNIX)
	include(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image >=2.0.0)
	PKG_SEARCH_MODULE(ASSIMP REQUIRED assimp >= 3.0)
	PKG_SEARCH_MODULE(GLEW REQUIRED glew)
else (UNIX)
	message("Please Define the followings path")
	message("SDL2_INCLUDE_DIRS, SDL2IMAGE_INCLUDE_DIRS")
	message("SDL2_LIBRARIES, SDL2IMAGE_LIBRARIES")
	message("ASSIMP_INCLUDE_DIRS, ASSIMP_LIBRARIES")
	message("GLEW_INCLUDE_DIRS, GLEW_LIBRARIES")
endif (UNIX)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
if (USE_GLM_BUNDLE)
	include_directories(include/bundle)
endif(USE_GLM_BUNDLE)
