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
#ifndef ENGINE_MAIN_INCLUED
#define ENGINE_MAIN_INCLUED

#include "Graphics.h"
#include "Mesh.h"
#include "Shader.h"
#include "Light.h"
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

			virtual void	AddMeshNode(Mesh * object, GLuint & id);
			virtual	void	AttachLight(std::shared_ptr<Light> light);
			virtual	void	DeleteObject(GLuint id);
			
			virtual	void	SetNodePosRot(GLuint id, glm::vec3 const & pos, glm::vec3 const & pitch);
			virtual	void	SetNodeScale(GLuint id, float scale);
			virtual	void	SetNodeAnimation(GLuint id, std::string const & animation);

			virtual void 	SetCameraLocation(glm::vec3 const & pos,glm::vec3 const & center,glm::vec3 const & vert);
			virtual	void	SetCameraSettings(GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far);
			
			virtual void	ClearColor(float r, float g, float b, float a);
			virtual	void	Init(); // next step is to have flags

			virtual	void	Draw(unsigned int elapsed);

		protected:
			std::shared_ptr<GraphicEngine::CGraphics>	m_pGraphics;
			glm::mat4								m_modelview;
			glm::mat4								m_projection;
			// struct ObjectList
			// Maybe next time we will do SceneGraph
			struct	ObjectNode
			{
				Mesh*		object;
				glm::vec3	position;
				float		pitch[3];
				float		scale;
				void	DoTransformation(glm::mat4 & modelview);
				std::string	animation;
			};

			std::vector< ObjectNode>				m_vObjectNode;	
			std::shared_ptr<Shader>					m_pShader;
			// For the moment only one light
			std::shared_ptr<Light>					m_light;
			// Save some camera settings
			glm::vec3								m_CameraPosition;
			glm::vec3								m_CameraCenter;
			glm::vec3								m_CameraVertical;
	};
}
#endif
