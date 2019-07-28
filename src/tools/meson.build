simple3dengine_tools_headers = files([
                                      'DebugGL.h',
                                      'Interpolate.hpp',
                                      'Loader.h',
                                      'Parser.h',
                                    ])

simple3dengine_tools_sources = files([
                                      'DebugGL.cpp',
                                      'Loader.cpp',
                                      'Parser.cpp',
                                    ])

simple3dengine_tools_include = include_directories('.')

simple3dengine_tools_lib = library('Simple3DEngine_tools',
                                   sources: [simple3dengine_tools_headers,
                                             simple3dengine_tools_sources,],
                                   dependencies: [glew_dep,
                                                  range_v3_dep,],
                                   include_directories: simple3dengine_tools_include,)

simple3dengine_tools_dep = declare_dependency(link_with: simple3dengine_tools_lib,
                                              dependencies: [glew_dep,
                                                             range_v3_dep,],
                                              include_directories: simple3dengine_tools_include,)

