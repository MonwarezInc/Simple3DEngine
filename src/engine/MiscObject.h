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
#ifndef	MISCOBJECT_INCLUED
#define	MISCOBJECT_INCLUED
#include "Object.hpp"
#include <GL/glew.h>

// This files will contain some traditionnal object (geometry)

namespace	GraphicEngine
{
	class	QuadObject	:	public	IObject
	{
		public:
			QuadObject();
			~QuadObject();
			
			void	SetTextureID(GLuint id);
			void	Draw(unsigned int elapsed_time, std::string const & animation="idle");
		
		private:
			GLuint		m_vao;
			GLuint		m_vbo;
			GLsizei		m_verticesSizeBytes;
			GLsizei		m_coordSizeBytes;
			GLuint		m_textureID;
	};
}	
#endif
