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
#include <S3DE/Graphics.h>
using namespace S3DE;
CGraphics::CGraphics(GLuint width, GLuint height, bool fullscreen,const std::string &title,
                     GLuint bpp, GLuint aa, GLuint major, GLuint minor ):m_bpp(bpp),m_aa(aa)
{
    m_pWindow       =   std::make_shared<Window>(title, width, height,fullscreen);

    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,   minor);
    // profile core
	
	if ((major >= 3 && minor >= 2) || major >= 4)
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	else if (3 == major)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_glContext     =   m_pWindow->CreateContext(0); // on a qu'une seule fenêtre
    GL_CHECK;
	
    if (m_glContext == 0)
        throw std::string(SDL_GetError());
    try
    {

        glewExperimental    =   GL_TRUE;
        GLenum  initialisationGLEW(glewInit());

        if (initialisationGLEW != GLEW_OK)
            throw std::string((char*)glewGetErrorString(initialisationGLEW));


    }
    catch (std::string error)
    {
        std::cerr << error << std::endl;
        exit(-1);
    }
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
}
void        CGraphics::SwapWindow()
{
    m_pWindow->SwapWindow(0);
}
void		CGraphics::ClearColor(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}
void 		CGraphics::Clear()
{
	// more later we will add flags option
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
CGraphics::~CGraphics()
{
    SDL_GL_DeleteContext(m_glContext);
}
