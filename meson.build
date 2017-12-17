project('Simple3DEngine','cpp', default_options : ['cpp_std=c++1z'])

sdl2_dep = dependency('sdl2',required:true)
sdl2image_dep = dependency('SDL2_image',required:true)
assimp_dep = dependency('assimp',required:true)
glew_dep = dependency('glew',required:true)
glm_dep = dependency('glm',required:true)

gtest_dep = dependency('gtest', required:false,fallback:['gtest','gtest_mainless_dep'])

simple3dengine_inc = include_directories('src')
subdir('src')

simple3dengine_dep = declare_dependency(link_with:simple3dengine_lib,include_directories:simple3dengine_inc)

# test section

test_list = ['Parser']

foreach tst : test_list
  exe = executable('@0@Test'.format(tst),
                   ['src/tests/@0@/main.cpp'.format(tst),
                    'src/tests/@0@/@0@_test.h'.format(tst),
                    'src/tests/@0@/@0@_test.cpp'.format(tst)],
                  dependencies : [gtest_dep,simple3dengine_dep])
  test('@0@ Test'.format(tst), exe)
endforeach


# cppcheck section
cppcheck_exe = find_program('cppcheck',required:false)
cppcheck_html = find_program('cppcheck-htmlreport',required:false)

cppcheck_target = run_target('cppcheck',command : ['tools/cppcheck.sh'])
