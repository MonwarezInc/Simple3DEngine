cmake_minimum_required(VERSION 3.3)

#Configuration of project
#Here is the library
project(Simple3DEngine)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(include)
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
	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
	PKG_SEARCH_MODULE(ASSIMP REQUIRED assimp)
	include_directories(${ASSIMP_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
else (UNIX)
	# Use new module to find SDL2
endif (UNIX)

find_package(GLEW)
if (GLEW_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
else (GLEW_FOUND)
	message(FATAL_ERROR "libGLEW not found")
endif (GLEW_FOUND)
