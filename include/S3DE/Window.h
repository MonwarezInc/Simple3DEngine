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
#ifndef WINDOW_H_INCLUED
#define WINDOW_H_INCLUED

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <string>
#include <vector>

namespace S3DE
{

	struct SingleWindow
	{
		std::string		    title;
		SDL_Window		    *pWindow;
		unsigned	int	    width;
		unsigned	int 	height;
	};

	class Window
	{
		public:
			Window(const std::string &title, unsigned int width, unsigned int height, bool fullscreen);
			virtual SDL_GLContext   CreateContext(unsigned int numWindow);
			virtual void            SwapWindow(unsigned int numWindow);
			virtual ~Window();

		protected:
			std::vector < SingleWindow >	m_vsWindow;
	};
}
#endif
