
simple3dengine_texture_headers = files([
                                      ])

simple3dengine_texture_sources = files([
                                      ])

simple3dengine_texture_include = include_directories('.')

simple3dengine_texture_lib = library('Simple3DEngine_texture',
                                     sources: [simple3dengine_texture_headers,
                                               simple3dengine_texture_sources,],
                                     dependencies: [simple3dengine_tools_dep],
                                     include_directories: simple3dengine_texture_include,)

simple3dengine_texture_dep = declare_dependency(link_with: simple3dengine_texture_lib,
                                                dependencies: [simple3dengine_tools_dep],
                                                include_directories: simple3dengine_texture_include,)


subdir('sdl2')
