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
#include <S3DE_FrameBuffer.h>
using namespace S3DE;
FrameBuffer::FrameBuffer():m_id(0), m_largeur(0), m_hauteur(0), m_colorBuffers(0), m_depthBufferID(0)
{
    //ctor
}
FrameBuffer::FrameBuffer(int largeur, int hauteur):m_id(0), m_largeur(largeur), m_hauteur(hauteur),
                        m_colorBuffers(0), m_depthBufferID(0)
{

}
void FrameBuffer::CreerRenderBuffer(GLuint &id, GLenum formatInterne)
{
    if(GL_TRUE == glIsRenderbuffer(id))
        glDeleteRenderbuffers(1,&id);

    glGenRenderbuffers(1,&id);

    glBindRenderbuffer(GL_RENDERBUFFER,id);

        glRenderbufferStorage(GL_RENDERBUFFER, formatInterne, m_largeur, m_hauteur);

    glBindRenderbuffer(GL_RENDERBUFFER,0);

}
void    FrameBuffer::Load()
{
    if (GL_TRUE == glIsFramebuffer(m_id))
    {
        glDeleteFramebuffers(1,&m_id);

        m_colorBuffers.clear();
    }

    // generate framebuffer id
    glGenFramebuffers(1,&m_id);

    // lock the  fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        Texture colorBuffer(m_largeur, m_hauteur, GL_RGBA, GL_RGBA, true);
        colorBuffer.LoadEmptyTexture();

        // add it on the array
        m_colorBuffers.push_back(colorBuffer);

        this->CreerRenderBuffer(m_depthBufferID, GL_DEPTH24_STENCIL8);

        // linking Color Buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffers[0].GetID(),0);
        // linking depth and stencil buffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);

        // integrity check 
        if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
        {
            glDeleteFramebuffers(1,&m_id);
            glDeleteRenderbuffers(1,&m_depthBufferID);

            m_colorBuffers.clear();
            throw std::string("Error , the fbo are not correctly builded");
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
GLuint  FrameBuffer::GetID() const
{
    return m_id;
}
GLuint FrameBuffer::GetColorBufferID(unsigned int index) const
{
    if (index >= m_colorBuffers.size())
        throw std::string("index >= m_colorBuffers.size()");
    return m_colorBuffers[index].GetID();
}
int     FrameBuffer::GetLargeur() const
{
    return m_largeur;
}
int     FrameBuffer::GetHauteur() const
{
    return m_hauteur;
}
FrameBuffer::~FrameBuffer()
{
    //dtor
    glDeleteFramebuffers(1,&m_id);
    glDeleteRenderbuffers(1,&m_depthBufferID);
    m_colorBuffers.clear();
}
