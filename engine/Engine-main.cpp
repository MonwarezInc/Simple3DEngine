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
#include "Engine-main.h"
using namespace GraphicEngine;
CEngine::CEngine()
{
	m_pGraphics	=	NULL;
	m_pShader	=	NULL;
}
CEngine::~CEngine()
{
	delete m_pGraphics;
	// for the list of object, we didn't manage the object memorie , so just delete the list , and not the content
	delete m_pShader;
}
void	CEngine::CreateWindow(GLuint width, GLuint height, bool fullscreen, const std::string & title,
								GLuint bpp, GLuint aa, GLuint major, GLuint minor)
{
	// to begin , we just manage 1 window
	m_pGraphics		=	(NULL == m_pGraphics) ?  new CGraphics(width, height, fullscreen, title, bpp, aa , major, minor):
						m_pGraphics ;
	// load shader
	m_pShader	=	new	Shader("./Shader-130/texture.vert","./Shader-130/texture.frag");
	m_pShader->charger();
}
void	CEngine::DeleteWindow(GLuint indice)
{
	// not implemented  yet
}
void	CEngine::SetActive(GLuint indice)
{
	// not implemented yet
}
void	CEngine::AddObject(IObject* object,GLuint & id)
{
	m_vObject.push_back(object);
	id	=	m_vObject.size() - 1;
}
void	CEngine::DeleteObject(GLuint id)
{
	//not implemented yet
}
void 	CEngine::SetCameraLocation(glm::vec3 const & pos, glm::vec3 const & center, glm::vec3 const & vert)
{
	// not implemented yet
	m_modelview	=	glm::lookAt(pos,center,vert);
}
void	CEngine::SetCameraSettings(GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far)
{
	m_projection	=	glm::perspective(fov, ratio, near, far);
}
void	CEngine::ClearColor(float r, float g, float b, float a)
{
	m_pGraphics->ClearColor(r,g,b,a);
}
void	CEngine::Clear()
{	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);
	m_pGraphics->Clear();
}
void	CEngine::Draw(unsigned int elapsed)
{
	GLuint	 		shaderID	=	m_pShader->getProgramID();
	glm::mat4		mvp			=	m_modelview;	// load camera pos
	glEnable(GL_DEPTH_TEST);
	
	glUseProgram(shaderID);
		glClear(GL_COLOR_BUFFER_BIT	| GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,800,600);
		GLuint		mvpLocation	=	glGetUniformLocation(shaderID,"MVP");
		for (unsigned int i=0; i < m_vObject.size(); ++i)
		{
			// do transformation stuff
			// ...
			mvp	=	m_projection * mvp;
			// send to OpenGL
			glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
			// then draw it
			m_vObject[i]->Draw(elapsed,0,49);
		}
	glUseProgram(0);
	m_pGraphics->SwapWindow();
}
