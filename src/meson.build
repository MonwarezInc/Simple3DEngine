include_engine = [
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
]
include_misc =[
  'misc/Input.h'
]
include_tools =[
  'tools/BasicMeshManager.h',
  'tools/CubemapTexture.h',
  'tools/DebugGL.h',
  'tools/Interpolate.hpp',
  'tools/Loader.h',
  'tools/Parser.h',
  'tools/SDL_Tools.h',
  'tools/Texture.h'
]
include_window =[
  'window/Graphics.h',
  'window/Window.h'
]
source_engine  = [
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
]
source_misc =[
  'misc/Input.cpp'
]
source_tools =[
  'tools/BasicMeshManager.cpp',
  'tools/CubemapTexture.cpp',
  'tools/DebugGL.cpp',
  'tools/Loader.cpp',
  'tools/Parser.cpp',
  'tools/Texture.cpp'
]
source_window = [
  'window/Graphics.cpp',
  'window/Window.cpp'
]

simple3dengineSrc =[
  include_engine,include_misc,include_tools,include_window,
  source_engine,source_misc,source_tools,source_window
]

simple3dengine = library('Simple3DEngine' , simple3dengineSrc, dependencies: [sdl2dep,sdl2imagedep,assimpdep,glewdep])
