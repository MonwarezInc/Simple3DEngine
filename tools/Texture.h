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
#ifndef TEXTURE_H
#define TEXTURE_H
// GLEW for all platform
#include <GL/glew.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

namespace GraphicEngine
{

	class Texture
	{
    	public:
        	Texture(const std::string &filename = "");
        	// for FBO
        	Texture(int largeur, int hauteur, GLenum format, GLenum formatInterne, bool textureVide);
        	Texture(const Texture &toCopy);
        	Texture& operator=(const Texture &toCopy);
        	bool Load();
        	void LoadEmptyTexture();

        	GLuint GetID () const;
        	void    SetFilename(const std::string &filename);
        	virtual ~Texture();
    	protected:
        	GLuint                  m_id;
        	std::string             m_filename;
        	// for FBO
        	int                     m_largeur;
        	int                     m_hauteur;
        	GLenum                  m_format;
        	GLenum                  m_formatInterne;
        	bool                    m_textureVide;
    	private:
	};
}
#endif // TEXTURE_H
