/*
Copyright (c) 2016, Payet Thibault
Copyright (c) 2017, Payet Thibault
Copyright (c) 2018, Payet Thibault
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
#include <GL/glew.h>

#include "SDL2GLWindow.h"

using S3DE::SDL2GLWindow;

SDL2GLWindow::SDL2GLWindow( int width, int height, std::string const& title, int major, int minor,
                            bool fullscreen )
    : width_{width}
    , height_{height}
    , title_{title}
    , major_{major}
    , minor_{minor}
    , fullscreen_{fullscreen}
{
    this->Init_();
}

SDL2GLWindow::SDL2GLWindow( SDL2GLWindow const& lhs )
    : width_{lhs.width_}
    , height_{lhs.height_}
    , title_{lhs.title_}
    , major_{lhs.major_}
    , minor_{lhs.minor_}
    , fullscreen_{lhs.fullscreen_}
{
    this->Init_();
}
SDL2GLWindow::SDL2GLWindow( SDL2GLWindow const&& lhs )
    : width_{lhs.width_}
    , height_{lhs.height_}
    , title_{lhs.title_}
    , major_{lhs.major_}
    , minor_{lhs.minor_}
    , fullscreen_{lhs.fullscreen_}
{
    this->Init_();
}

void SDL2GLWindow::SwapWindow()
{
    SDL_GL_SwapWindow( window_.get() );
}

SDL2GLWindow::~SDL2GLWindow()
{
    SDL_GL_DeleteContext( context_ );
}

void SDL2GLWindow::Init_()
{
    if ( SDL_WasInit( SDL_INIT_VIDEO ) == 0 )
    {
        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            throw std::string( "error sdl init video" );
        }
    }
    if ( width_ <= 0 )
    {
        throw std::string( "error width cannot be lesser or equal to 0" );
    }
    if ( height_ <= 0 )
    {
        throw std::string( "error height cannot be lesser or equal to 0" );
    }
    if ( fullscreen_ )
    {
        window_ = {SDL_CreateWindow( title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     width_, height_, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL ),
                   sdl::Deleter()};
    }
    else
    {
        window_ = {SDL_CreateWindow( title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ),
                   sdl::Deleter()};
    }
    if ( nullptr == window_ )
    {
        throw std::string( "Error creation of window" );
    }


    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major_ );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor_ );

    if ( ( major_ >= 3 && minor_ >= 2 ) || major_ >= 4 )
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    }
    else if ( 3 == major_ )
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

    context_ = SDL_GL_CreateContext( window_.get() );

    if ( context_ == 0 )
    {
        throw std::string( SDL_GetError() );
    }

    glewExperimental = GL_TRUE;
    GLenum initialisationGLEW{glewInit()};

    if ( initialisationGLEW != GLEW_OK )
    {
        throw std::string(
            reinterpret_cast<const char*>( glewGetErrorString( initialisationGLEW ) ) );
    }
}
