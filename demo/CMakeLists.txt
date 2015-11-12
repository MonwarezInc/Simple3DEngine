cmake_minimum_required(VERSION 3.3)

#Configuration of project
#for the moment we will just do the exec (library will come later)
project(TestEngine)

set(EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE})

#Include and Library filepath
include_directories(${include_s3de})
link_directories(${lib_s3de})
include_directories(${lib_include})
link_directories(${lib_lib})

#Generate source file list
file(
	GLOB_RECURSE
	source_file
	src/*
)
set(Extra_CXX_Flags "-Wall -std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Extra_CXX_Flags}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(GL GL)
if (win)
	set(GL opengl32)
endif (win)
#Declaration of executable
add_executable(
	TestEngine
	${source_file}
)

#Configuration of link resolution
target_link_libraries(
	TestEngine
	Simple3DEngine
	${GL}
	SDL2
	SDL2_image
	GLEW
	assimp
)
