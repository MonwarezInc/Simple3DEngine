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
#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED
#include "Window.h"
// GLEW for all platform
#include <GL/glew.h>

#include <iostream>
#include "DebugGL.h"
#include <stdlib.h>
#include <memory>

namespace S3DE
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
