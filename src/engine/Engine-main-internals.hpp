/*
Copyright (c) 2016, Payet Thibault
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Monwarez Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "Engine-main.h"

namespace S3DE
{
template <typename GLWindow, typename SceneManager>
Engine<GLWindow, SceneManager>::Engine( SceneManager &sceneManagerView )
    : sceneManager_{sceneManagerView}
{
}


template <typename GLWindow, typename SceneManager>
WindowHandle Engine<GLWindow, SceneManager>::CreateWindow( EngineWindow const &engine )
{
    // if ( !m_pGraphics )
    //     m_pGraphics = std::make_unique<CGraphics>( engine.width, engine.height,
    //     engine.fullscreen,
    //                                                engine.title, engine.bpp, engine.aa,
    //                                                engine.major, engine.mior );


    WindowHandle window = window_.NewWindow( engine.width, engine.height, engine.title,
                                             engine.major, engine.minor, engine.fullscreen );
    // load shader
    m_pShader.SetFile( engine.shader.lightV, engine.shader.lightF, engine.shader.lightG );
    return window;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::DeleteWindow( GLuint indice )
{
    // not implemented  yet
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::SetActive( GLuint indice )
{
    // not implemented yet
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::AddMesh( std::vector<MeshPair> const &meshPair )
{
    m_meshManager.Load( meshPair );
    for ( auto &v : meshPair )
        this->AddMeshNode( v.entity );
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::AddMeshNode( std::string const &entity, bool isVisible )
{
    ObjectNode objectNode;
    objectNode.entity   = entity;
    objectNode.position = glm::vec3( 0.0, 0.0, 0.0 );
    objectNode.scale    = 1.0;
    for ( unsigned int i = 0; i < 3; ++i )
        objectNode.pitch[ i ] = 0.0;
    objectNode.animation = "idle"; // idle by default
    objectNode.id        = m_vObjectNode.size();
    objectNode.isVisible = isVisible;
    m_vObjectNode.push_back( objectNode );
    m_entToID[ entity ] = objectNode.id;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::DelMeshNode( std::string const &entity )
{
    // If succeed does not launch an exception
    // it's not manage the memory of the mesh itself
    auto it = m_entToID.find( entity );
    if ( it == m_entToID.end() )
        throw std::string( "Error the id is to high for DelMeshNode" );
    m_vObjectNode[ it->second ].isVisible = false; // Set to false so that we didn't show it
    m_vObjectNode[ it->second ].entity    = "";
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::AttachLight( std::vector<PointLight> const &pointlight )
{
    m_PointLight = pointlight;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::AttachLight( std::vector<SpotLight> const &spotlight )
{
    m_SpotLight = spotlight;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::AttachLight( DirectionalLight const &dlight )
{
    m_Directional = dlight;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::DeleteObject( GLuint id )
{
    // not implemented yet
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::SetNodePosRot( std::string const &entity, glm::vec3 const &pos,
                                                    glm::vec3 const &pitch )
{
    auto it = m_entToID.find( entity );
    if ( it != m_entToID.end() )
    {
        m_vObjectNode[ it->second ].position = pos;
        for ( unsigned int i = 0; i < 3; ++i )
        {
            m_vObjectNode[ it->second ].pitch[ i ] = pitch[ i ];
        }
    }
    // else we do nothing improve performance xD
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::SetNodeScale( std::string const &entity, float scale )
{
    auto it = m_entToID.find( entity );
    if ( it != m_entToID.end() )
        m_vObjectNode[ it->second ].scale = scale;
    // same things like SetObjectPosRot
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::SetNodeAnimation( std::string const &entity,
                                                       std::string const &animation )
{
    auto it = m_entToID.find( entity );
    if ( it != m_entToID.end() )
        m_vObjectNode[ it->second ].animation = animation;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::SetCameraLocation( glm::vec3 const &pos,
                                                        glm::vec3 const &center,
                                                        glm::vec3 const &vert )
{
    m_modelview      = glm::lookAt( pos, center, vert );
    m_CameraPosition = pos;
    m_CameraCenter   = center;
    m_CameraVertical = vert;
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::SetCameraSettings( GLdouble fov, GLdouble ratio, GLdouble near,
                                                        GLdouble far )
{
    m_projection = glm::perspective( fov, ratio, near, far );
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::ClearColor( float r, float g, float b, float a )
{
    glClearColor( r, g, b, a );
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::Init()
{
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::Draw(
    std::chrono::duration<float, std::chrono::seconds::period> elapsed )
{
    m_pShader.Enable();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    GLuint mvpLocation  = m_pShader.GetUniformLocation( "MVP" );
    GLuint modelviewloc = m_pShader.GetUniformLocation( "modelview" );
    GLuint projectionl  = m_pShader.GetUniformLocation( "projection" );

    // Light
    m_pShader.Init();
    m_pShader.SetLights( m_Directional );
    m_pShader.SetLights( m_PointLight );
    m_pShader.SetLights( m_SpotLight );

    m_pShader.SetEyeWorldPos( m_CameraPosition );

    glUniformMatrix4fv( projectionl, 1, GL_FALSE, glm::value_ptr( m_projection ) );
    // end light
    for ( auto &objectNode : m_vObjectNode )
    {
        // do transformation stuff
        // ...
        // glm::mat4		mvp			=	m_modelview;	// load camera pos
        if ( objectNode.isVisible )
        {
            glm::mat4 mvp;
            objectNode.DoTransformation( mvp );
            // Send to OpenGL the modelview before apply camera transformation and after object
            // transformation
            glUniformMatrix4fv( modelviewloc, 1, GL_FALSE, glm::value_ptr( mvp ) );
            mvp = m_projection * m_modelview * mvp;
            // send to OpenGL
            glUniformMatrix4fv( mvpLocation, 1, GL_FALSE, glm::value_ptr( mvp ) );
            // send material
            m_pShader.SetMatSpecularIntensity( 1.0 );
            m_pShader.SetMatSpecularPower( 2 );
            // then draw it
            try
            {
                m_meshManager.Draw( objectNode.entity, elapsed, m_pShader, objectNode.animation );
            }
            catch ( std::string &a )
            {
                ResourceExcept re = {objectNode.id, MeshExceptFlag::RELEASE};
                MeshException me;
                me.SetResource( re );
                me.SetMsg( a );
                throw me;
            }
        }
    }
    m_pShader.Disable();
    for ( auto &window : window_ )
    {
        window.second.SwapWindow();
    }
}
template <typename GLWindow, typename SceneManager>
void Engine<GLWindow, SceneManager>::ObjectNode::DoTransformation( glm::mat4 &mdv ) const
{
    // Translate to position
    mdv = glm::translate( mdv, position );
    // axe  X Y Z orientation
    mdv = glm::rotate( mdv, pitch[ 0 ], glm::vec3( 1, 0, 0 ) );
    mdv = glm::rotate( mdv, pitch[ 1 ], glm::vec3( 0, 1, 0 ) );
    mdv = glm::rotate( mdv, pitch[ 2 ], glm::vec3( 0, 0, 1 ) );
    // resize
    mdv = glm::scale( mdv, glm::vec3( scale, scale, scale ) );
}
} // namespace S3DE
