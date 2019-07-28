simple3dengine_sdl2_utils_headers = files([
                                         ])

simple3dengine_sdl2_utils_sources = files([
                                         ])

simple3dengine_sdl2_utils_include = include_directories('.')

simple3dengine_sdl2_utils_lib = library('Simple3DEngine_sdl2_utils',
                                        sources: [simple3dengine_sdl2_utils_headers,
                                                  simple3dengine_sdl2_utils_sources],

                                        dependencies: [sdl2_dep],
                                        include_directories: simple3dengine_sdl2_utils_include,
                                        )

simple3dengine_sdl2_utils_dep = declare_dependency(link_with: simple3dengine_sdl2_utils_lib,
                                                   dependencies: [sdl2_dep],
                                                   include_directories: simple3dengine_sdl2_utils_include
                                                   )
