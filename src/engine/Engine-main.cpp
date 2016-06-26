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
#include <S3DE_Engine-main.h>
using namespace S3DE;
CEngine::CEngine()
{
}
CEngine::~CEngine()
{
	// for the list of object, we didn't manage the object memory , so just delete the list , and not the content
}
void	CEngine::CreateWindow(EngineWindow const & engine)
{
	if (!m_pGraphics)
		m_pGraphics	=	std::make_shared<CGraphics>(engine.width,engine.height,
													engine.fullscreen,engine.title,engine.bpp,
													engine.aa,engine.major,engine.minor);
	// load shader
	m_pShader.SetFile(engine.shader.lightV,engine.shader.lightF, engine.shader.lightG);
}
void	CEngine::DeleteWindow(GLuint indice)
{
	// not implemented  yet
}
void	CEngine::SetActive(GLuint indice)
{
	// not implemented yet
}
void	CEngine::AddMeshNode(Mesh* object,GLuint & id, bool isVisible)
{
	ObjectNode	objectNode;
	objectNode.object	=	object;
	objectNode.position	=	glm::vec3(0.0,0.0,0.0);
	objectNode.scale	=	1.0;
	for (unsigned int i=0; i < 3; ++i)
		objectNode.pitch[i]	=	0.0;
	objectNode.animation	=	"idle"; // idle by default
	objectNode.id			=	m_vObjectNode.size();
	objectNode.isVisible	=	isVisible;
	m_vObjectNode.push_back(objectNode);
	id	=	objectNode.id;
}
void	CEngine::DelMeshNode(size_t id)
{
	// If succeed does not launch an exception
	// it's not manage the memory of the mesh itself
	if (id >= m_vObjectNode.size())
		throw std::string ("Error the id is to high for DelMeshNode");
	m_vObjectNode[id].isVisible	=	false;	// Set to false so that we didn't show it
	m_vObjectNode[id].object	=	nullptr;	
}
void	CEngine::AttachLight(std::vector<PointLight> const & pointlight)
{
	m_PointLight	=	pointlight;
}
void	CEngine::AttachLight(std::vector<SpotLight> const & spotlight)
{
	m_SpotLight		=	spotlight;
}
void	CEngine::AttachLight(DirectionalLight const & dlight)
{
	m_Directional	=	dlight;
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
			m_vObjectNode[id].pitch[i]	=	pitch[i];
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
}
void	CEngine::Draw(unsigned int elapsed)
{
	m_pShader.Enable();
		glClear(GL_COLOR_BUFFER_BIT	| GL_DEPTH_BUFFER_BIT);
		GLuint		mvpLocation	=	m_pShader.GetUniformLocation("MVP");
		GLuint		modelviewloc=	m_pShader.GetUniformLocation("modelview");
		GLuint		projectionl	=	m_pShader.GetUniformLocation("projection");
		
		//Light
		m_pShader.Init();
		m_pShader.SetLights(m_Directional);
		m_pShader.SetLights(m_PointLight);
		m_pShader.SetLights(m_SpotLight);

		m_pShader.SetEyeWorldPos(m_CameraPosition);

		glUniformMatrix4fv(projectionl, 1, GL_FALSE, glm::value_ptr(m_projection));
		// end light
		for (auto &objectNode : m_vObjectNode)
		{
			// do transformation stuff
			// ...
			//glm::mat4		mvp			=	m_modelview;	// load camera pos
			if (objectNode.isVisible)
			{
				glm::mat4	mvp;
				objectNode.DoTransformation(mvp);
				// Send to OpenGL the modelview before apply camera transformation and after object transformation
				glUniformMatrix4fv(modelviewloc, 1, GL_FALSE, glm::value_ptr(mvp));
				mvp	=	m_projection * m_modelview * mvp;
				// send to OpenGL
				glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
				// send material
				m_pShader.SetMatSpecularIntensity(1.0);
				m_pShader.SetMatSpecularPower(2);
				// then draw it
				try
				{
					objectNode.object->Draw(elapsed,m_pShader,objectNode.animation);
				}
				catch (std::string &a)
				{
					ResourceExcept	re = { objectNode.id , MeshExceptFlag::RELEASE};
					MeshException	me;
					me.SetResource(re);
					me.SetMsg(a);
					throw me;	
				}
			}	
		}
	m_pShader.Disable();
	m_pGraphics->SwapWindow();
}
void	CEngine::ObjectNode::DoTransformation(glm::mat4 & mdv) const
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

