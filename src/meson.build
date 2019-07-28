subdir('engine')
subdir('misc')
subdir('tools')
subdir('window')

subdir('sdl2_utils')

simple3dengineSrc =[
  include_engine, include_misc, include_tools, include_window,
  include_window_sdl2, source_engine, source_misc, source_tools,
  source_window, source_window_sdl2
]

simple3dengine_lib = library('Simple3DEngine' ,
                             sources: [simple3dengineSrc],
                             dependencies: [simple3dengine_sdl2_utils_dep,
                                            sdl2_dep,
                                            sdl2image_dep,
                                            assimp_dep,
                                            glew_dep,
                                            glm_dep,
                                            bullet_dep,
                                            range_v3_dep,],

                             cpp_args: [simple3dengine_cpp_args],
                             install: true)

simple3dengine_inc = include_directories('.')

simple3dengine_dep = declare_dependency(link_with: simple3dengine_lib,
                                        include_directories: simple3dengine_inc,
                                        dependencies: [simple3dengine_sdl2_utils_dep,
                                                       sdl2_dep,
                                                       sdl2image_dep,
                                                       assimp_dep,
                                                       glew_dep,
                                                       glm_dep,
                                                       bullet_dep,
                                                       range_v3_dep])



if (test_option)
  subdir('tests')
endif
