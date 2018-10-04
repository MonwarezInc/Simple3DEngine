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
#include "../tools/BasicMeshManager.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshException.h"
#include "Shader.h"
#include "window/WindowManager.h"
#include "window/sdl2/SDL2GLWindow.h"

#include <chrono>
#include <functional>
#include <vector>

namespace S3DE
{
struct EngineShader
{
    std::string lightV; ///< light vertex shader filename*/
    std::string lightF; ///< light fragment shader filename*/
    std::string lightG; ///< basic geometry shader filename*/
    std::string guiV;   ///< gui vertex shader filename*/
    std::string guiF;   ///< gui fragment shader filename*/
};
struct EngineWindow
{
    int width;           ///< width of the window */
    int height;          ///< height of the window */
    bool fullscreen;     ///< boolean for fullscreen state*/
    std::string title;   ///< title of the window*/
    int major;           ///< OpenGL major version*/
    int minor;           ///< OpenGL minor version*/
    EngineShader shader; ///< @see EngineShader*/
};
/// \brief  Main class for the Simple 3D Engine
///
/// This class is an interface with the engine. Almost all  the command for interacting with the
/// engine are here.
/// \author Payet Thibault
template <typename GLWindow>
class CEngine
{
public:
    CEngine();
    virtual ~CEngine();

    /** \brief Create a window with the specified OpenGL version (Limited to one window for now)
     *   @param  engineWindow    Structure that contain all information needed to create the window
     *   @return an handle to the window
     */
    WindowHandle CreateWindow( const EngineWindow &engineWindow );
    ///	\brief 	Delete the specifided window (Not Implemented)
    ///	@param[in]  indice  Id of the window
    void DeleteWindow( GLuint indice );
    ///	\brief 	Set the active window (Not Implemented)
    ///	@param[in]  indice Id of the window
    void SetActive( GLuint indice );
    ///	\brief  Add a bunch of mesh
    ///	\param  meshPair    A vector of entity,filename pair
    void AddMesh( std::vector<MeshPair> const &meshPair );
    void DelMeshNode( std::string const &entity );
    ///	\brief  Set/Update the light information
    ///	\param[in]  pointlight  A vector of PointLight data
    void AttachLight( std::vector<PointLight> const &pointlight );
    ///	\brief Set/update the spot light information
    ///	\param	spotlight   a vector of SpotLight data
    void AttachLight( std::vector<SpotLight> const &spotlight );
    ///	\brief Set/update the Directional light information
    ///	\param  dlight  a DirectionalLight
    void AttachLight( DirectionalLight const &dlight );
    ///	\brief Delete an object from the scene (Not Implemented)
    ///	\param[in]  id jId of the object
    void DeleteObject( GLuint id );
    ///	\brief  Set the position and rotation of an object of the scene
    ///	\param[in]  entity  The name of the entity
    ///	\param[in]  pos The new position
    ///	\param[in]  pitch   The pitch rotation X,Y,Z
    void SetNodePosRot( std::string const &entity, glm::vec3 const &pos, glm::vec3 const &pitch );
    void SetNodeScale( std::string const &entity, float scale );
    void SetNodeAnimation( std::string const &entity, std::string const &animation );

    void SetCameraLocation( glm::vec3 const &pos, glm::vec3 const &center, glm::vec3 const &vert );
    void SetCameraSettings( GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far );

    void ClearColor( float r, float g, float b, float a );
    void Init(); // next step is to have flags

    void Draw( std::chrono::duration<float, std::chrono::seconds::period> elapsed );

protected:
    WindowManager<std::map<WindowHandle, GLWindow>, GLWindow> window_;
    BasicMeshManager m_meshManager;
    std::map<std::string, size_t> m_entToID;
    glm::mat4 m_modelview;
    glm::mat4 m_projection;
    // struct ObjectList
    // Maybe next time we will do SceneGraph
    struct ObjectNode
    {
        std::string entity;
        glm::vec3 position;
        float pitch[ 3 ];
        float scale;
        void DoTransformation( glm::mat4 &modelview ) const;
        std::string animation;
        size_t id;      ///< the id of the Mesh
        bool isVisible; ///< Do the node is visible
    };
    void AddMeshNode( std::string const &entity, bool isVisible = true );
    std::vector<ObjectNode> m_vObjectNode;
    // Light is a shader
    Light m_pShader;
    // Here is the different light
    DirectionalLight m_Directional;
    std::vector<PointLight> m_PointLight;
    std::vector<SpotLight> m_SpotLight;
    // Save some camera settings
    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraCenter;
    glm::vec3 m_CameraVertical;
};


} // namespace S3DE

#include "Engine-main-internals.hpp"
