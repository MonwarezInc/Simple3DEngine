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
#ifndef S3DE_GBUFFER_HEADER_INCLUED_H
#define S3DE_GBUFFER_HEADER_INCLUED_H
#include <GL/glew.h>

namespace S3DE
{
class	GBuffer
{
	public:
		/// \brief set the G Buffer different texture type, plus set the number
		///	of textures used
		enum class GBufferTextureType
		{ POSITION = 0, DIFFUSE = 1, NORMAL = 2, TEXCOORD = 3, NUM_TEXTURES = 4};
	
		GBuffer();
		~GBuffer();
		///	\brief	Initialize the G Buffer
		///
		///	First it will create one framebuffer , four G Buffer textures and 
		///	one depth buffer texture.
		///	It will throw an exception if the Framebuffer is not complete
		///
		///	\param	width	The window width
		///	\param	height	The window height
		void	Init(unsigned int width, unsigned int height);
		///	\brief just a wrapper to glBindFrameBuffer with GL_DRAW_FRAMEBUFFER
		void	BindForWriting();
		///	\brief just a wrapper to glBindFramebuffer with GL_READ_FRAMEBUFFER
		void	BindForReading();

	protected:
		GLuint	m_fbo; ///< the object ID for the framebuffer 
		GLuint	m_textures[static_cast<size_t>(GBufferTextureType::NUM_TEXTURES)]; ///< an arrays of object ID for the textures
		GLuint	m_depthTexture; ///< the object ID for the depth texture

};
}
#endif
