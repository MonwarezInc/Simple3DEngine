project('Simple3DEngine','cpp', default_options : ['cpp_std=c++1z'])

sdl2dep = dependency('sdl2',required:true)
sdl2imagedep = dependency('SDL2_image',required:true)
assimpdep = dependency('assimp',required:true)
glewdep = dependency('glew',required:true)
glmdep = dependency('glm',required:true)

gtestdep = dependency('gtest',required:false,main:false,fallback:['gtest','gtest_mainless_dep'])

simple3dengine_inc = include_directories('src')
subdir('src')

simple3dengine_dep = declare_dependency(link_with:simple3dengine_lib,include_directories:simple3dengine_inc)

subdir('tests/Parser')
