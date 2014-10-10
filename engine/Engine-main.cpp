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
}
CEngine::~CEngine()
{
	delete m_pGraphics;
	// for the list, we didn't manage the object memorie , so just delete the list , and not the content
}
void	CEngine::CreateWindow(GLuint width, GLuint height, bool fullscreen, const std::string & title,
								GLuint bpp, GLuint aa, GLuint major, GLuint minor)
{
	// to begin , we just manage 1 window
	m_pGraphics		=	(NULL == m_pGraphics) ?  new CGraphics(width, height, fullscreen, title, bpp, aa , major, minor):
						m_pGraphics ;
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
	// not implemented yet
}
void	CEngine::DeleteObject(GLuint id)
{
	//not implemented yet
}
void 	CEngine::SetCameraLocation(glm::vec3 const & pos, glm::vec3 const & center, glm::vec3 const & vert)
{
	// not implemented yet
}
void	CEngine::SetCameraSettings(GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far)
{
	// not implemented yet
}
void	CEngine::ClearColor(float r, float g, float b, float a)
{
	m_pGraphics->ClearColor(r,g,b,a);
}
void	CEngine::Clear()
{
	m_pGraphics->Clear();
}
void	CEngine::Draw()
{
	// do drawing stuff
	// ...
	// then SwapWindow
	m_pGraphics->SwapWindow();
}
