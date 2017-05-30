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
#include <S3DE_CubemapTexture.h>

using namespace S3DE;

CubemapTexture::CubemapTexture(std::string const &directory, std::string const &posx,
                               std::string const &negx, std::string const &posy,
                               std::string const &negy, std::string const &posz,
                               std::string const &negz)
{
    m_textureId = 0;
    m_type.resize(6);
    m_type[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    m_type[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    m_type[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    m_type[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    m_type[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    m_type[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    m_name.resize(6);
    m_name[0] = directory + posx;
    m_name[1] = directory + negx;
    m_name[2] = directory + posy;
    m_name[3] = directory + negy;
    m_name[4] = directory + posz;
    m_name[5] = directory + negz;
}
CubemapTexture::~CubemapTexture()
{
    if (GL_TRUE == glIsTexture(m_textureId))
        glDeleteTextures(1, &m_textureId);
}
bool CubemapTexture::Load()
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

    for (size_t i = 0; i < m_type.size(); ++i)
    {
        auto imgSDL = IMG_Load(m_name[i].c_str());

        if (nullptr == imgSDL)
            throw std::string(SDL_GetError());

        GLenum internalformat(0);
        GLenum format(0);

        if (imgSDL->format->BytesPerPixel == 3)
        {
            internalformat = GL_RGB;

            if (imgSDL->format->Rmask == 0xff)
                format = GL_RGB;
            else
                format = GL_BGR;
        }
        else if (imgSDL->format->BytesPerPixel == 4)
        {
            internalformat = GL_RGBA;

            if (imgSDL->format->Rmask == 0xff)
                format = GL_RGBA;
            else
                format = GL_BGRA;
        }
        else
        {
            SDL_FreeSurface(imgSDL);
            return false;
        }

        glTexImage2D(m_type[i], 0, internalformat, imgSDL->w, imgSDL->h, 0, format,
                     GL_UNSIGNED_BYTE, imgSDL->pixels);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        SDL_FreeSurface(imgSDL);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}
void CubemapTexture::Bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
}
