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
#ifndef ENGINE_MAIN_INCLUED
#define ENGINE_MAIN_INCLUED
/**
*	\file 	S3DE_Engine-main.h
*	\brief 	Main class for the Simple 3D Engine
*
*	This class is an interface with the engine. Almost all  the command for interacting with the engine are here.
*	\author	Payet Thibault
*
*/
#include "S3DE_Graphics.h"
#include "S3DE_Mesh.h"
#include "S3DE_Shader.h"
#include "S3DE_Light.h"
#include <vector>
namespace S3DE
{
	class CEngine
	{
		public:
			CEngine();
			virtual	~CEngine();
			
			/**	\brief Create a window with the specified OpenGL version (Limited to one window for now)
			*	
			*	@param[in] width		Width of the window
			*	@param[in] height		Height of the window
			*	@param[in] fullscreen	Boolean for fullscreen mode
			*	@param[in] title		Set the title of the window
			*	@param[in] bpp			Set the bit per pixel parameter
			*	@param[in] aa			Set antialiasing multiplier
			*	@param[in] major		Set OpenGL major version
			*	@param[in] minor		Set OpenGL minor version
			*/
			virtual	void	CreateWindow(GLuint width=320, GLuint height=240, bool fullscreen=false,
											const std::string & title="CEngine window", GLuint bpp=32,
											GLuint aa=2, GLuint major=3, GLuint minor=0);
			/** \brief 	Delete the specifided window (Not Implemented)
			*	@param[in]	indice		Id of the window
			*/
			virtual void	DeleteWindow(GLuint indice);
			/** \brief 	Set the active window (Not Implemented)
			*	@param[in]	indice		Id of the window
			*/
			virtual	void	SetActive(GLuint indice);
			/**
			* 	\brief Add an object to the scene
			*
			*	@param[in] object 		A pointer to the object mesh
			*	@param[in] id 			The returned id for managing position etc...
			*
			*/
			virtual void	AddMeshNode(Mesh * object, GLuint & id);
			/**
			* 	\brief	Set/Update the light information
			*	@param[in]	pointlight	A vector of PointLight data	
			*/
			virtual	void	AttachLight(std::vector<PointLight> const & pointlight);
			/**
			*	\brief Delete an object from the scene (Not Implemented)
			*	@param[in]	id	Id of the object
			*/
			virtual	void	DeleteObject(GLuint id);
			/**	\brief	Set the position and rotation of an object of the scene
			*	@param[in] 	id		The id of the object
			*	@param[in] 	pos		The new position
			*	@param[in]	pitch	The pitch rotation X,Y,Z
			*/
			virtual	void	SetNodePosRot(GLuint id, glm::vec3 const & pos, glm::vec3 const & pitch);
			virtual	void	SetNodeScale(GLuint id, float scale);
			virtual	void	SetNodeAnimation(GLuint id, std::string const & animation);

			virtual void 	SetCameraLocation(glm::vec3 const & pos,glm::vec3 const & center,glm::vec3 const & vert);
			virtual	void	SetCameraSettings(GLdouble fov, GLdouble ratio, GLdouble near, GLdouble far);
			
			virtual void	ClearColor(float r, float g, float b, float a);
			virtual	void	Init(); // next step is to have flags

			virtual	void	Draw(unsigned int elapsed);

		protected:
			std::shared_ptr<S3DE::CGraphics>		m_pGraphics;
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
				void	DoTransformation(glm::mat4 & modelview)const;
				std::string	animation;
			};

			std::vector< ObjectNode>				m_vObjectNode;	
			// Light is a shader 
			std::shared_ptr<Light>					m_pShader;
			// Here is the different light 
			std::vector<PointLight>					m_PointLight;
			// Save some camera settings
			glm::vec3								m_CameraPosition;
			glm::vec3								m_CameraCenter;
			glm::vec3								m_CameraVertical;
	};
}
#endif
