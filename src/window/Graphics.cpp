/*
Copyright (C) 2014 Payet thibault

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include <GraphicEngine/Graphics.h>
using namespace GraphicEngine;
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
