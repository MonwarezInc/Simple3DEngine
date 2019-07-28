simple3dengine_headers = files([
                                'BasicMeshManager.h',
                                'Camera.h',
                                'Camera-internals.hpp',
                                'CEntity.h',
                                'Engine-main.h',
                                'Engine-main-internals.hpp',
                                'FrameBuffer.h',
                                'GBuffer.h',
                                'IResourceManager.hpp',
                                'Light.h',
                                'Mesh.h',
                                'MeshAssimpImpl.h',
                                'MeshImpl.h',
                                'MeshImplFactory.h',
                                'MeshException.h',
                                'MeshManager.h',
                                'SceneGraph.h',
                                'Shader.h',
                                'ShadowMap.h',
                                'SkyboxShader.h',
                                'Vertex.h'
                              ])

simple3dengine_sources = files([
                                'BasicMeshManager.cpp',
                                'CEntity.cpp',
                                'FrameBuffer.cpp',
                                'GBuffer.cpp',
                                'Light.cpp',
                                'Mesh.cpp',
                                'MeshAssimpImpl.cpp',
                                'MeshImplFactory.cpp',
                                'MeshException.cpp',
                                'MeshManager.cpp',
                                'SceneGraph.cpp',
                                'Shader.cpp',
                                'ShadowMap.cpp',
                                'SkyboxShader.cpp'
                              ])


simple3dengine_include = include_directories('.')

simple3dengine_lib = library('Simple3DEngine' ,
                             sources: [simple3dengine_headers,
                                       simple3dengine_sources,],
                             dependencies: [simple3dengine_sdl2_utils_dep,
                                            simple3dengine_window_dep,
                                            simple3dengine_texture_sdl2_dep,
                                            assimp_dep,
                                            glew_dep,
                                            glm_dep,
                                            bullet_dep,
                                            range_v3_dep,],

                             include_directories: simple3dengine_include,

                             cpp_args: [simple3dengine_cpp_args],
                             install: true)


simple3dengine_dep = declare_dependency(link_with: simple3dengine_lib,
                                        dependencies: [simple3dengine_sdl2_utils_dep,
                                                       simple3dengine_window_dep,
                                                       simple3dengine_texture_sdl2_dep,
                                                       assimp_dep,
                                                       glew_dep,
                                                       glm_dep,
                                                       bullet_dep,
                                                       range_v3_dep],
                                        include_directories: simple3dengine_include,
                                       )

