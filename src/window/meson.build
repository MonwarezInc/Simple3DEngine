simple3dengine_window_headers = files([
                                       'WindowHandle.h',
                                       'WindowManager.h'
                                     ])

simple3dengine_window_sources = files([
                                       'WindowHandle.cpp'
                                     ])

simple3dengine_window_include = include_directories('.')

simple3dengine_window_lib = library('Simple3DEngine_window',
                                    sources: [simple3dengine_window_headers,
                                              simple3dengine_window_sources],
                                    include_directories: simple3dengine_window_include)

simple3dengine_window_dep = declare_dependency(link_with: simple3dengine_window_lib,
                                               include_directories: simple3dengine_window_include)

subdir('sdl2')



