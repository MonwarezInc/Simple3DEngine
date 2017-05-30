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
#include "S3DE_Mesh.h"
#include "S3DE_MeshManager.h"
#include <chrono>
#include <map>
#include <memory>
#include <vector>
///
///    \brief    namespace of the engine
namespace S3DE
{
class CEntity
{
public:
    /// \brief    Construct a new entity manager
    CEntity() = default;
    /// \brief    Destructor of a entity, it will decrease the counter associated to the resource
    virtual ~CEntity();
    /// \brief    Request loading a ressource
    /// \param    filename the filename of the ressource to load
    /// \param    entityName the name of the entity (has to be unique)
    virtual void Load(std::string const &filename, std::string const &entityName);
    /// \brief    Clear the resource associated with entityName
    /// \param    entityName	the name of the entity to release
    virtual void Clear(std::string const &entityName);
    /// \brief    Draw the Entity, just a wrapper to MeshManager->Draw(...)
    /// \param    entity        Array of the name of the entity to display
    /// \param    elapsed_time  The elapsed time since the beginning, so that animation works
    /// \param    shader        A reference to the shader to use
    /// \param    animation     The name of the animation to play
    virtual void Draw(std::vector<std::string> const &entity,
                      std::chrono::duration<float, std::chrono::seconds::period> elapsed_time,
                      Shader const &shader, std::string const &animation);

protected:
    MeshManager m_rcManager;                   ///<    the resource manager
    std::map<std::string, size_t> m_entityKey; ///<    associate a entityname to a rcField
    std::vector<RcField> m_rcField; ///<    keep informative data of all the resource \see RcField
};
} // end of S3DE namespace
