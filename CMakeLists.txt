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
	include/*
)
set(Extra_CXX_Flags "-Wall -std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
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
	message("SDL2 include dir: " ${SDL2_INCLUDE_DIRS})
	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
	PKG_SEARCH_MODULE(ASSIMP REQUIRED assimp)
	message("Assimp include dir: " ${ASSIMP_INCLUDE_DIRS})
	include_directories(${ASSIMP_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
	PKG_SEARCH_MODULE(GLEW REQUIRED glew)
	message("GLEW include dir: " ${GLEW_INCLUDE_DIRS})
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
else (UNIX)
	# Use new module to find SDL2
endif (UNIX)

