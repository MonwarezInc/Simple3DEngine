simple3dengine_texture_sdl2_headers = files([
                                             'CubemapTexture.h',
                                             'Texture.h',
                                           ])

simple3dengine_texture_sdl2_sources = files([
                                             'CubemapTexture.cpp',
                                             'Texture.cpp',
                                           ])

simple3dengine_texture_sdl2_include = include_directories('.')

simple3dengine_texture_sdl2_lib = library('Simple3DEngine_texture_sdl2',
                                          sources: [simple3dengine_texture_sdl2_headers,
                                                    simple3dengine_texture_sdl2_sources,],
                                          dependencies: [simple3dengine_sdl2_utils_dep,
                                                         simple3dengine_texture_dep,
                                                         sdl2image_dep,],
                                          include_directories: simple3dengine_texture_sdl2_include,
                                         )

simple3dengine_texture_sdl2_dep = declare_dependency(link_with: simple3dengine_texture_sdl2_lib,
                                                     dependencies: [simple3dengine_sdl2_utils_dep,
                                                                    simple3dengine_texture_dep,
                                                                    sdl2image_dep,],
                                                     include_directories: simple3dengine_texture_sdl2_include)
