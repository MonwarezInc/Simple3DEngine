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
#include <S3DE_GBuffer.h>
#include <sstream>
#include <string>
using namespace S3DE;
void	GBuffer::Init(unsigned int width, unsigned int height)
{
	// First create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Create gbuffer textures
	glGenTextures(static_cast<int>(GBufferTextureType::NUM_TEXTURES), m_textures);
	glGenTextures(1, &m_depthTexture);

	for (size_t i= 0; i < static_cast<size_t>(GBufferTextureType::NUM_TEXTURES); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, 
						GL_FLOAT, nullptr);
		glFramebufferTexture2D(	GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
								GL_TEXTURE_2D, m_textures[i], 0);
	}
	// Depth
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, 
					GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(	GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
							m_depthTexture, 0);

	GLenum DrawBuffers[static_cast<size_t>(GBufferTextureType::NUM_TEXTURES)] =
										{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
										GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(static_cast<size_t>(GBufferTextureType::NUM_TEXTURES), DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::stringstream out;
		out << "Error at " << __FILE__ << "(" << __LINE__  << ")"<< std::endl;
		out << "Framebuffer error, status: 0x" << Status << std::endl;	
		throw out.str();
	}
	
	// Then restore default framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
