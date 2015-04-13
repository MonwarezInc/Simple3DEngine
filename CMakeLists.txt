cmake_minimum_required(VERSION 2.6)

#Configuration of project
#for the moment we will just do the exec (library will come later)
project(GraphicEngine)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(${lib_include})
link_directories(${lib_lib})

#Generate source file list
file(
	GLOB_RECURSE
	source_file
	src/*
)
set(GLib GL)
if (win)
	set(GLib opengl32)
endif (win)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#Declaration of executable
add_executable(
	test-engine
	${source_file}
)

#Configuration of link resolution
target_link_libraries(
	test-engine
	${GLib}
	SDL2
	SDL2_image
	GLEW
	assimp
)
