simple3dengine_window_sdl2_headers = files([
                                            'SDL2GLWindow.h',
                                          ])

simple3dengine_window_sdl2_sources = files([
                                            'SDL2GLWindow.cpp',
                                          ])

simple3dengine_window_sdl2_include = include_directories('.')

simple3dengine_window_sdl2_lib = library('Simple3DEngine_window_sdl2',
                                         sources: [simple3dengine_window_sdl2_headers,
                                                   simple3dengine_window_sdl2_sources],
                                         dependencies: [simple3dengine_sdl2_utils_dep,
                                                        glew_dep],
                                         include_directories: simple3dengine_window_sdl2_include,)

simple3dengine_window_sdl2_dep = declare_dependency(link_with: simple3dengine_window_sdl2_lib,
                                                    dependencies: [simple3dengine_sdl2_utils_dep,
                                                                   glew_dep],
                                                    include_directories: simple3dengine_window_sdl2_include)
