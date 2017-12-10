project('Simple3DEngine','cpp', default_options : ['cpp_std=c++1z'])

sdl2_dep = dependency('sdl2',required:true)
sdl2image_dep = dependency('SDL2_image',required:true)
assimp_dep = dependency('assimp',required:true)
glew_dep = dependency('glew',required:true)
glm_dep = dependency('glm',required:true)

gtest_dep = dependency('gtest',required:false,main:false,fallback:['gtest','gtest_mainless_dep'])

simple3dengine_inc = include_directories('src')
subdir('src')

simple3dengine_dep = declare_dependency(link_with:simple3dengine_lib,include_directories:simple3dengine_inc)

subdir('tests/Parser')

# cppcheck section
cppcheck_target = run_target('cppcheck',command : ['tools/cppcheck.sh',
                                simple3dengineSrc]
                            )
