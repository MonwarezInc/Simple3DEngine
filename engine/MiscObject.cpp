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
#include "MiscObject.h"
#ifndef BUFFER_OFFSET
	#define BUFFER_OFFSET(a)	((char*)NULL + (a))
#endif
using namespace GraphicEngine;
QuadObject::QuadObject()
{
	GLfloat	vertices[]	=	{ 	0,0,0,	0,1,0,	0,1,1,
								0,0,1	};
	GLfloat	coordTex[]	=	{	0,0,	1,0,	1,1,
								0,1		};

	m_verticesSizeBytes	=	12*sizeof(GLfloat);
	m_coordSizeBytes	=	8*sizeof(GLfloat);

	// generate buffer an vertex array
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// bind vertex arrays
	glBindVertexArray(m_vao);
		// bind vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			// create a buffer with enought space
			glBufferData(GL_ARRAY_BUFFER, m_verticesSizeBytes + m_coordSizeBytes, 0, GL_DYNAMIC_DRAW); // not optimal
			// populate with data
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_verticesSizeBytes, vertices);
			glBufferSubData(GL_ARRAY_BUFFER, m_verticesSizeBytes, m_coordSizeBytes, coordTex);
		
			// enable vertex attributes
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_verticesSizeBytes));

	glBindVertexArray(0);
}
QuadObject::~QuadObject()
{
	glDeleteBuffers(1,&m_vbo);
	glDeleteVertexArrays(1,&m_vao);
}
void	QuadObject::SetTextureID(GLuint id)
{
	m_textureID	=	id;
}
void	QuadObject::Draw(unsigned int elapsed_time, int start , int end)
{
	(void) 	elapsed_time;
	(void)	start;
	(void)	end;

	glBindVertexArray(m_vao);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
	
