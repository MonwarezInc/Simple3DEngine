cmake_minimum_required(VERSION 3.2)

project(Simple3DEngine)

file(
    GLOB_RECURSE
    include_files
    ../include/S3DE/*
)
file(
    GLOB_RECURSE
    source_file
    ../src/*
    ../include/S3DE/*
)

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

include(../common.cmake)

# Install stuff
install(FILES ${include_files} DESTINATION include/S3DE)
install(TARGETS Simple3DEngine LIBRARY DESTINATION lib)
