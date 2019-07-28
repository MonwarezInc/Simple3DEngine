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
#include "Texture.h"
#include "DebugGL.h"
#include "SDL_Tools.h"

using namespace std;
using namespace S3DE;
Texture::Texture( const string &filename )
    : m_id( 0 )
    , m_filename( filename )
    , m_largeur( 0 )
    , m_hauteur( 0 )
    , m_format( 0 )
    , m_formatInterne( 0 )
    , m_textureVide( false )
{
#ifdef S3DE_USE_DSA
    if ( !GLEW_ARB_direct_state_access )
        throw std::string( "Error nee ARB_direct_state_access for using DSA" );
#endif
}
Texture::Texture( const Texture &toCopy )
    : m_filename{toCopy.m_filename}
    , m_largeur{toCopy.m_largeur}
    , m_format{toCopy.m_format}
    , m_formatInterne{toCopy.m_format}
    , m_textureVide{toCopy.m_textureVide}
{
    if ( m_textureVide && glIsTexture( toCopy.m_id ) == GL_TRUE )
        this->LoadEmptyTexture();
    else if ( GL_TRUE == glIsTexture( toCopy.m_id ) )
        this->Load();
}
Texture::Texture( int largeur, int hauteur, GLenum format, GLenum formatInterne, bool textureVide )
    : m_id( 0 )
    , m_filename( "" )
    , m_largeur( largeur )
    , m_hauteur( hauteur )
    , m_format( format )
    , m_formatInterne( formatInterne )
    , m_textureVide( textureVide )
{
}
Texture &Texture::operator=( const Texture &toCopy )
{
    m_filename = toCopy.m_filename;

    m_largeur       = toCopy.m_largeur;
    m_hauteur       = toCopy.m_hauteur;
    m_format        = toCopy.m_format;
    m_formatInterne = toCopy.m_formatInterne;
    m_textureVide   = toCopy.m_textureVide;
    if ( m_textureVide && glIsTexture( toCopy.m_id ) == GL_TRUE )
        this->LoadEmptyTexture();
    else if ( GL_TRUE == glIsTexture( toCopy.m_id ) )
        this->Load();

    return *this;
}
Texture::~Texture()
{
    glDeleteTextures( 1, &m_id );
}
void Texture::LoadEmptyTexture()
{
    if ( GL_TRUE == glIsTexture( m_id ) )
        glDeleteTextures( 1, &m_id );

#ifdef S3DE_USE_DSA
    glCreateTextures( GL_TEXTURE_2D, 1, &m_id );
    glTextureStorage2D( m_id, 1, m_formatInterne, m_largeur, m_hauteur );
    glTextureSubImage2D( m_id, 0, 0, 0, m_largeur, m_hauteur, m_format, GL_UNSIGNED_BYTE, nullptr );
    glTextureParameteri( m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTextureParameteri( m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
#else
    glGenTextures( 1, &m_id );
    GLuint boundTexture = 0;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint *>( &boundTexture ) );
    glBindTexture( GL_TEXTURE_2D, m_id );

    glTexImage2D( GL_TEXTURE_2D, 0, m_formatInterne, m_largeur, m_hauteur, 0, m_format,
                  GL_UNSIGNED_BYTE, nullptr );
    // filtre
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glBindTexture( GL_TEXTURE_2D, boundTexture );
#endif
}
bool Texture::Load()
{
    auto imgSDL = this->Load_IMG( m_filename );
    if ( nullptr == imgSDL.get() )
        throw string( SDL_GetError() );

    if ( glIsTexture( m_id ) == GL_TRUE )
        glDeleteTextures( 1, &m_id );



    GLenum formatInterne( 0 );
    GLenum format( 0 );

    // d?termination du format interne
    if ( imgSDL->format->BytesPerPixel == 3 )
    {
        // format interne
        formatInterne = GL_RGB4;
        // format
        if ( imgSDL->format->Rmask == 0xff )
            format = GL_RGB;
        else
            format = GL_BGR;
    }
    else if ( imgSDL->format->BytesPerPixel == 4 )
    {
        // format interne
        formatInterne = GL_RGBA4;
        // format
        if ( imgSDL->format->Rmask == 0xff )
            format = GL_RGBA;
        else
            format = GL_BGRA;
    }
    else
    {
        //	We stop loading
        std::cerr << "Erreur format interne non reconu\n";
        return false;
    }


#ifdef S3DE_USE_DSA
    glCreateTextures( GL_TEXTURE_2D, 1, &m_id );
    glTextureStorage2D( m_id, 1, formatInterne, imgSDL->w, imgSDL->h );
    glTextureSubImage2D( m_id, 0, 0, 0, imgSDL->w, imgSDL->h, format, GL_UNSIGNED_BYTE,
                         imgSDL->pixels );

    glTextureParameteri( m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTextureParameteri( m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
#else
    glGenTextures( 1, &m_id );
    GLuint boundTexture = 0;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint *>( &boundTexture ) );
    glBindTexture( GL_TEXTURE_2D, m_id );

    glTexImage2D( GL_TEXTURE_2D, 0, formatInterne, imgSDL->w, imgSDL->h, 0, format,
                  GL_UNSIGNED_BYTE, imgSDL->pixels );

    // Application des filtres

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glBindTexture( GL_TEXTURE_2D, boundTexture );
#endif
    // on a finis
    return true;
}
void Texture::SetFilename( const string &filename )
{
    m_filename = filename;
}
GLuint Texture::GetID() const
{
    return m_id;
}
SurfacePtr Texture::Load_IMG( string const &file )
{
    return SurfacePtr( IMG_Load( file.c_str() ), sdl::Deleter() );
}
