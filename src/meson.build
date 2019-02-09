subdir('engine')
subdir('misc')
subdir('tools')
subdir('window')

simple3dengineSrc =[
  include_engine, include_misc, include_tools, include_window,
  include_window_sdl2, source_engine, source_misc, source_tools,
  source_window, source_window_sdl2
]

simple3dengine_lib = library('Simple3DEngine' ,
                             simple3dengineSrc,
                             dependencies: [sdl2_dep,
                                            sdl2image_dep,
                                            assimp_dep,
                                            glew_dep,
                                            glm_dep,
                                            bullet_dep,
                                            range_v3_dep,],

                             cpp_args: ['-DS3DE_USE_DSA=1'],
                             install: true)
