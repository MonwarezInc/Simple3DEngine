cmake_minimum_required(VERSION 2.6)

#Configuration of project
#Here is the library
project(GraphicEngine)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(include)
include_directories(${lib_include})
link_directories(${lib_lib})

#Generate source file list
file(
	GLOB_RECURSE
	source_file
	src/*
	include/*
)
set(GLib GL)
set(Extra_CXX_Flags "-Wall -std=c++1y")
if (win)
	set(GLib opengl32)
endif (win)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#Declaration of executable
add_library(
	GraphicEngine
	STATIC
	${source_file}
)

