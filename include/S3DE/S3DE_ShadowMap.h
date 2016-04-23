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
#ifndef S3DE_SHADOW_MAP_INCLUED
#define	S3DE_SHADOW_MAP_INCLUED

#include <GL/glew.h>
#include <S3DE_Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace S3DE
{
class	ShadowMapFBO
{
	public:
		ShadowMapFBO();
		~ShadowMapFBO();
		/** \brief initialize a shadow map with the given size
		*	\param width	width of the FBO
		*	\param height	height of the FBO
		*/
		void	Init(unsigned int width, unsigned int height); // can launch an exception
		/** \brief bind the Shadow map FBO to the current state in writing mode*/
		void	BindForWriting();
		/** \brief bind the shadow map for reading 
		*	\param	TexrureUnit	the type of the texture to be attached
		*/
		void	BindForReading(GLenum	TextureUnit);
	private:
		GLuint		m_fbo;	//< identifier for the fbo
		GLuint		m_shadowMap; //< identifier for the shadow map

};
class	ShadowMapShader : public Shader
{
	public:
		ShadowMapShader();
		/** \brief	Init the shader for uniform location*/
		void	Init();
		/** \brief Set the world view projection matrix to use
		* \param wvp the world view projection matrix
		*/
		void	SetWVP(glm::mat4 const & wvp);
		/** \brief	Set the texture unit to use
		*	\param	TextureUnit the texture unit to use
		*/
		void	SetTextureUnit(unsigned int TextureUnit);
	private:
		GLuint	m_wvpLocation;
		GLuint	m_textureLocation;
};

}  // end of S3DE namespace
#endif
