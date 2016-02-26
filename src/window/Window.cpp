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
#include <S3DE_Window.h>
using namespace S3DE;
Window::Window(const std::string &title, unsigned int width, unsigned int height, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::string ("error sdl init video");
	m_vsWindow.push_back(SingleWindow());
	m_vsWindow.back().title	=	title;
	m_vsWindow.back().width	=	width;
	m_vsWindow.back().height	=	height;
	m_vsWindow.back().pWindow =	fullscreen?
                            SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
							SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL) :
                            SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
							SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
}
SDL_GLContext   Window::CreateContext(unsigned int numWindow)
{
    if (numWindow >= m_vsWindow.size())
        throw std::string("error unknow indice");
    return SDL_GL_CreateContext(m_vsWindow[numWindow].pWindow);
}
void            Window::SwapWindow(unsigned int numWindow)
{
    if (numWindow >= m_vsWindow.size())
        throw std::string("error unknow indice");
    SDL_GL_SwapWindow(m_vsWindow[numWindow].pWindow);
}
Window::~Window()
{
    for (auto &vsWindow:  m_vsWindow)
        SDL_DestroyWindow(vsWindow.pWindow);
    m_vsWindow.clear();

}
