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
#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED
#include "Window.h"
// GLEW for all platform
#include <GL/glew.h>

#include <iostream>
#include "../tools/DebugGL.h"
#include <stdlib.h>
#include <memory>

namespace GraphicEngine
{

	class CGraphics
	{
    	public:
        	CGraphics(GLuint width=320, GLuint height=240, bool fullscreen=false, const std::string &title="default",
                  GLuint bpp=32, GLuint aa=2, GLuint major=4, GLuint minor=4 );

        	virtual         ~CGraphics();
        	virtual void    SwapWindow();
			virtual void	ClearColor(float r, float g, float b, float a);
			virtual	void	Clear(); // next step is to encapsulate glClear flag

    	protected:
        	std::shared_ptr<Window>	m_pWindow;
        	SDL_GLContext   		m_glContext;
        	//OpenGL specifics
        	GLuint          		m_bpp;
        	GLuint          		m_aa;
	
	};
}
#endif // GRAPHICS_H_INCLUDED
