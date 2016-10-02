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
#pragma once
#include "S3DE_SDL_Tools.h"
// GLEW for all platform
#include <GL/glew.h>


#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

namespace S3DE
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
		SurfacePtr	Load_IMG(std::string const &file);
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
}  // end of S3DE namespace
