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
#include "SceneGraph.h"

using namespace S3DE;
SceneGraph::SceneGraph()
{
    // not implemented yet
}
SceneGraph::~SceneGraph()
{
    // not implemented yet
}
unsigned int SceneGraph::AddMesh( Mesh* obj, glm::mat4 const& transf, unsigned int parent,
                                  bool hide, bool trans )
{
    // not implemented yet

    return 0;
}
bool SceneGraph::DeleteObject( unsigned int id )
{
    // not implemented yet
    return false;
}
Mesh* SceneGraph::ChainTransformation( unsigned int id, glm::mat4& transf )
{
    // not implemented yet
    return nullptr;
}
// Tree algorithm
SceneGraph::Tree::Tree()
{
    // ok we create the root node
    m_root.nodeInfo.id  = 0;
    m_root.nodeInfo.obj = nullptr;
    // it is root id = 0 so don't care about more initialisation
    m_root.previous = nullptr;
    m_root.next.push_back( nullptr );
}
SceneGraph::Tree::~Tree()
{
    // not implemented yet
}
unsigned int SceneGraph::Tree::AddNodeInfo( NodeInfo const& nodeInfo, unsigned int parent )
{
    throw std::runtime_error( "Not implemented yet" );
}
SceneGraph::Node* SceneGraph::Tree::Detach( unsigned int id )
{
    throw std::runtime_error( "Not implemented yet" );
}
bool SceneGraph::Tree::Attach( Node const* node, unsigned int id )
{
    throw std::runtime_error( "Not implemented yet" );
}
