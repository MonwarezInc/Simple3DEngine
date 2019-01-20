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
#include "MeshImpl.h"
#include "MeshImplFactory.h"
#include "Shader.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace S3DE
{
class Mesh
{
public:
    Mesh() = delete;

    Mesh( Mesh&& ) = default;

    Mesh( Mesh const& );

    explicit Mesh( MeshImplType const& meshImplType );

    /// \brief  Load a mesh from a file
    ///
    /// This function can launch exception and assertion, the loader are really strict. <br/>
    /// Loading a resource that is actually loaded may lead to unbehavior issue.
    /// \param  filename    filename of the Mesh to load
    void loadFromFile( std::string const& filename );

    /// \brief  Draw the mesh with the animation specified WIP
    ///
    /// For the moment only the first animation is supported, need some resource with multiple
    /// animation <br/>
    /// which can be loaded, and need to implement a correct search animation id by name function
    /// \param  elapsed_time    The elapsed time since the beginning of the application
    /// \param  shader A reference to set some information to the shader (like bone
    /// information
    /// etc...)
    /// \param  animation  The name of the animation to play
    void draw( std::chrono::duration<float, std::chrono::seconds::period> elapsed_time,
               Shader const& shader, std::string const& animation );

private:
    std::unique_ptr<MeshImpl> const impl_;
    std::string filename_{""};
    MeshImplType const meshType_;
};



} // namespace S3DE
