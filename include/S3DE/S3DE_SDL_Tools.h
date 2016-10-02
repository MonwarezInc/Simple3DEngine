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
#ifndef _S3DE_SDL_TOOLS_H_INCLUED
#define _S3DE_SDL_TOOLS_H_INCLUED
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <string>
#include <memory>
namespace S3DE
{
namespace sdl
{
///	\brief	Deleter for SDL2 pointer, so that we can use unique_ptr
struct	Deleter
{
	void operator()(SDL_Window *p) const {SDL_DestroyWindow(p);}
	void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p);}
	void operator()(SDL_Surface *p) const { SDL_FreeSurface(p);}
};

} // end of namespace sdl
template <class T>
using SDL_Ptr	=	std::unique_ptr<T, sdl::Deleter>;
using WindowPtr	=	SDL_Ptr<SDL_Window>;
using SurfacePtr=	SDL_Ptr<SDL_Surface>;

} // end of namespace S3DE
#endif
