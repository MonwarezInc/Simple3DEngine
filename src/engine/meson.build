include_engine = files([
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

source_engine  = files([
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


install_headers(include_engine, subdir: 'S3DE/engine')
