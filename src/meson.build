include_engine = files([
  'engine/Camera.h',
  'engine/CEntity.h',
  'engine/Engine-main.h',
  'engine/FrameBuffer.h',
  'engine/GBuffer.h',
  'engine/IResourceManager.hpp',
  'engine/Light.h',
  'engine/Mesh.h',
  'engine/MeshException.h',
  'engine/MeshManager.h',
  'engine/SceneGraph.h',
  'engine/Shader.h',
  'engine/ShadowMap.h',
  'engine/SkyboxShader.h'
  ])
include_misc = files([
  'misc/Input.h'
])

include_tools = files([
  'tools/BasicMeshManager.h',
  'tools/CubemapTexture.h',
  'tools/DebugGL.h',
  'tools/Interpolate.hpp',
  'tools/Loader.h',
  'tools/Parser.h',
  'tools/SDL_Tools.h',
  'tools/Texture.h'
])
include_window = files([
  'window/Graphics.h',
  'window/Window.h'
])
source_engine  = files([
  'engine/Camera.cpp',
  'engine/CEntity.cpp',
  'engine/Engine-main.cpp',
  'engine/FrameBuffer.cpp',
  'engine/GBuffer.cpp',
  'engine/Light.cpp',
  'engine/Mesh.cpp',
  'engine/MeshException.cpp',
  'engine/MeshManager.cpp',
  'engine/SceneGraph.cpp',
  'engine/Shader.cpp',
  'engine/ShadowMap.cpp',
  'engine/SkyboxShader.cpp'
])
source_misc = files([
  'misc/Input.cpp'
])
source_tools = files([
  'tools/BasicMeshManager.cpp',
  'tools/CubemapTexture.cpp',
  'tools/DebugGL.cpp',
  'tools/Loader.cpp',
  'tools/Parser.cpp',
  'tools/Texture.cpp'
])
source_window = files([
  'window/Graphics.cpp',
  'window/Window.cpp'
])

simple3dengineSrc =[
  include_engine,include_misc,include_tools,include_window,
  source_engine,source_misc,source_tools,source_window
]

simple3dengine_lib = library('Simple3DEngine' , simple3dengineSrc, dependencies: [sdl2_dep,sdl2image_dep,assimp_dep,glew_dep,glm_dep])
