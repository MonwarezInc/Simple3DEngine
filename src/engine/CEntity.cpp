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
#include <S3DE_CEntity.h>
using namespace S3DE;
CEntity::~CEntity()
{
    for ( auto &v : m_rcField )
        m_rcManager.Release( v );
}
void CEntity::Load( std::string const &filename, std::string const &entityName )
{
    m_rcField.push_back( m_rcManager.Load( filename ) );
    m_entityKey[ entityName ] = m_rcField.size() - 1;
}
void CEntity::Clear( std::string const &entityName )
{
    if ( m_entityKey.find( entityName ) != m_entityKey.end() )
    {
        auto id      = m_entityKey[ entityName ];
        auto rcField = m_rcField[ id ];
        m_rcManager.Release( rcField );
        m_rcField[ id ] = rcField;
    }
    else
    {
        // silently discard
    }
}
void CEntity::Draw( std::vector<std::string> const &entity,
                    std::chrono::duration<float, std::chrono::seconds::period> elapsed_time,
                    Shader const &shader, std::string const &animation )
{
    for ( auto &v : entity )
    {
        if ( m_entityKey.find( v ) != m_entityKey.end() )
            m_rcManager.Draw( m_rcField[ m_entityKey[ v ] ], elapsed_time, shader, animation );
    }
}
