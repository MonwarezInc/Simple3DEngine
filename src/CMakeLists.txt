cmake_minimum_required(VERSION 3.2)

project(Simple3DEngine)

# Include files ( they will be installed )
set(include_engine
  engine/Camera.h
  engine/CEntity.h
  engine/Engine-main.h
  engine/FrameBuffer.h
  engine/GBuffer.h
  engine/IResourceManager.hpp
  engine/Light.h
  engine/Mesh.h
  engine/MeshException.h
  engine/MeshManager.h
  engine/SceneGraph.h
  engine/Shader.h
  engine/ShadowMap.h
  engine/SkyboxShader.h
  )
set(include_misc
  misc/Input.h
  )
set(include_tools
  tools/BasicMeshManager.h
  tools/CubemapTexture.h
  tools/DebugGL.h
  tools/Interpolate.hpp
  tools/Loader.h
  tools/Parser.h
  tools/SDL_Tools.h
  tools/Texture.h
  )
set(include_window
  window/Graphics.h
  window/Window.h
  )
# Source file
set(source_engine
  engine/Camera.cpp
  engine/CEntity.cpp
  engine/Engine-main.cpp
  engine/FrameBuffer.cpp
  engine/GBuffer.cpp
  engine/Light.cpp
  engine/Mesh.cpp
  engine/MeshException.cpp
  engine/MeshManager.cpp
  engine/SceneGraph.cpp
  engine/Shader.cpp
  engine/ShadowMap.cpp
  engine/SkyboxShader.cpp
  )
set(source_misc
  misc/Input.cpp
  )
set(source_tools
  tools/BasicMeshManager.cpp
  tools/CubemapTexture.cpp
  tools/DebugGL.cpp
  tools/Loader.cpp
  tools/Parser.cpp
  tools/Texture.cpp
  )
set(source_window
  window/Graphics.cpp
  window/Window.cpp
  )

#Declaration of executable
if (USE_SHARED)
    add_library(
        ${PROJECT_NAME}
        SHARED
        ${include_engine}
        ${source_engine}
        ${include_misc}
        ${source_misc}
        ${include_tools}
        ${source_tools}
        ${include_window}
        ${source_window}
    )
else ()
     add_library(
        ${PROJECT_NAME}
        STATIC
        ${include_engine}
        ${source_engine}
        ${include_misc}
        ${source_misc}
        ${include_tools}
        ${source_tools}
        ${include_window}
        ${source_window}
    )
endif ()

include(../common.cmake)

# Install stuff
install(FILES ${include_engine} DESTINATION include/S3DE/engine)
install(FILES ${include_misc} DESTINATION include/S3DE/misc)
install(FILES ${include_tools} DESTINATION include/S3DE/tools)
install(FILES ${include_window} DESTINATION include/S3DE/window)

install(TARGETS Simple3DEngine LIBRARY DESTINATION lib)
