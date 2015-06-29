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
#include <GraphicEngine/Engine-main.h>
using namespace GraphicEngine;
CEngine::CEngine()
{
}
CEngine::~CEngine()
{
	// for the list of object, we didn't manage the object memory , so just delete the list , and not the content
}
void	CEngine::CreateWindow(GLuint width, GLuint height, bool fullscreen, const std::string & title,
								GLuint bpp, GLuint aa, GLuint major, GLuint minor)
{
	if (!m_pGraphics)
		m_pGraphics	=	std::make_shared<CGraphics>(width,height,fullscreen,title,bpp,aa,major,minor);
	// load shader
	if (!m_pShader)
		m_pShader	=	std::make_shared<Shader>("./Shader/texture.vert","./Shader/texture.frag");
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
void	CEngine::AddMeshNode(Mesh* object,GLuint & id)
{
	ObjectNode	objectNode;
	objectNode.object	=	object;
	objectNode.position	=	glm::vec3(0.0,0.0,0.0);
	objectNode.scale	=	1.0;
	for (unsigned int i=0; i < 3; ++i)
		objectNode.pitch[i]	=	0.0;
	objectNode.animation	=	"idle"; // idle by default
	m_vObjectNode.push_back(objectNode);
	id	=	m_vObjectNode.size() - 1;
}
void	CEngine::AttachLight(std::shared_ptr<Light> light, std::vector<PointLight> const & pointlight)
{
	m_light			=	light;
	m_PointLight	=	pointlight;
}
void	CEngine::DeleteObject(GLuint id)
{
	//not implemented yet
}
void	CEngine::SetNodePosRot(GLuint id, glm::vec3 const & pos, glm::vec3 const & pitch)
{
	if (id < m_vObjectNode.size())
	{
		m_vObjectNode[id].position	=	pos;
		for (unsigned int i=0; i < 3; ++i)
			m_vObjectNode[id].pitch[i]	=	(float)pitch[i];
	}
	// else we do nothing improve performance xD
}
void	CEngine::SetNodeScale(GLuint id, float scale)
{
	if (id < m_vObjectNode.size())
		m_vObjectNode[id].scale	=	scale;
	// same things like SetObjectPosRot
}
void	CEngine::SetNodeAnimation(GLuint id, std::string const & animation)
{
	if (id < m_vObjectNode.size())
		m_vObjectNode[id].animation	=	animation;
}
void 	CEngine::SetCameraLocation(glm::vec3 const & pos, glm::vec3 const & center, glm::vec3 const & vert)
{
	m_modelview	=	glm::lookAt(pos,center,vert);
	m_CameraPosition	=	pos;
	m_CameraCenter		=	center;
	m_CameraVertical	=	vert;
}
void	CEngine::SetCameraSettings(GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far)
{
	m_projection	=	glm::perspective(fov, ratio, near, far);
}
void	CEngine::ClearColor(float r, float g, float b, float a)
{
	m_pGraphics->ClearColor(r,g,b,a);
}
void	CEngine::Init()
{	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);
}
void	CEngine::Draw(unsigned int elapsed)
{
	GLuint	 		shaderID	=	m_pShader->getProgramID();
	
	glUseProgram(shaderID);
		glClear(GL_COLOR_BUFFER_BIT	| GL_DEPTH_BUFFER_BIT);
		GLuint		mvpLocation	=	glGetUniformLocation(shaderID,"MVP");
		GLuint		modelviewloc=	glGetUniformLocation(shaderID,"modelview");
		GLuint		projectionl	=	glGetUniformLocation(shaderID,"projection");
		
		//Light
		// for_each light
		if (m_light)
		{
			m_light->SetShaderID(shaderID);
			//m_light->Show();
			m_light->SetPointLights(m_PointLight);
			m_light->SetEyeWorldPos(m_CameraPosition);
		}
		glUniformMatrix4fv(projectionl, 1, GL_FALSE, glm::value_ptr(m_projection));
		// end light
		for (unsigned int i=0; i < m_vObjectNode.size(); ++i)
		{
			// do transformation stuff
			// ...
			//glm::mat4		mvp			=	m_modelview;	// load camera pos
			glm::mat4	mvp;
			m_vObjectNode[i].DoTransformation(mvp);
			// Send to OpenGL the modelview before apply camera transformation and after object transformation
			glUniformMatrix4fv(modelviewloc, 1, GL_FALSE, glm::value_ptr(mvp));
			mvp	=	m_projection * m_modelview * mvp;
			// send to OpenGL
			glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
			// send material
			m_light->SetMatSpecularIntensity(1.0);
			m_light->SetMatSpecularPower(2);
			// then draw it
			m_vObjectNode[i].object->Draw(elapsed, shaderID,m_vObjectNode[i].animation);
		}
	glUseProgram(0);
	m_pGraphics->SwapWindow();
}
void	CEngine::ObjectNode::DoTransformation(glm::mat4 & mdv)
{
	// Translate to position
	mdv	=	glm::translate(mdv, position);
	// axe  X Y Z orientation
	mdv	=	glm::rotate(mdv, pitch[0], glm::vec3(1,0,0));
	mdv	=	glm::rotate(mdv, pitch[1], glm::vec3(0,1,0));
	mdv	=	glm::rotate(mdv, pitch[2], glm::vec3(0,0,1));
	// resize
	mdv	=	glm::scale(mdv, glm::vec3(scale,scale,scale));
}

