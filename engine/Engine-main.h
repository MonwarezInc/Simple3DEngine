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
#include "../window/Graphics.h"
#include "Object.hpp"
#include "../tools/Shader.h"
#include <vector>

namespace GraphicEngine
{
	class CEngine
	{
		public:
			CEngine();
			virtual	~CEngine();

			virtual	void	CreateWindow(GLuint width=320, GLuint height=240, bool fullscreen=false,
											const std::string & title="CEngine window", GLuint bpp=32,
											GLuint aa=2, GLuint major=3, GLuint minor=0);
			virtual void	DeleteWindow(GLuint indice);
			virtual	void	SetActive(GLuint indice);

			virtual void	AddObject(IObject * object, GLuint & id);
			virtual	void	DeleteObject(const GLuint id);
			
			virtual void 	SetCameraLocation(const glm::vec3 & pos, const glm::vec3 & center, const glm::vec3 & vert);
			virtual	void	SetCameraSettings(GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far);
			
			virtual void	ClearColor(float r, float g, float b, float a);
			virtual	void	Clear(); // next step is to have flags

			virtual	void	Draw(unsigned int elapsed);

		protected:
			GraphicEngine::CGraphics*				m_pGraphics;
			glm::mat4								m_modelview;
			glm::mat4								m_projection;
			std::vector< IObject* >					m_vObject;	
			Shader*									m_pShader;
	};
}

