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
#include "Texture.h"

using namespace std;
using namespace GraphicEngine;
Texture::Texture(const string &filename): m_id(0), m_filename(filename), m_largeur(0), m_hauteur(0), m_format(0),
                                        m_formatInterne(0), m_textureVide(false)
{
    //ctor
}
Texture::Texture(const Texture &toCopy)
{
    m_filename      =   toCopy.m_filename;

    m_largeur       =   toCopy.m_largeur;
    m_hauteur       =   toCopy.m_hauteur;
    m_format        =   toCopy.m_format;
    m_formatInterne =   toCopy.m_formatInterne;
    m_textureVide   =   toCopy.m_textureVide;
    if (m_textureVide && glIsTexture(toCopy.m_id) == GL_TRUE)
        this->LoadEmptyTexture();
    else if (GL_TRUE == glIsTexture(toCopy.m_id) )
        this->load();
}
Texture::Texture(int largeur, int hauteur, GLenum format, GLenum formatInterne, bool textureVide):m_id(0),m_filename(""),
                                                    m_largeur(largeur), m_hauteur(hauteur),m_format(format),m_formatInterne(formatInterne),
                                                    m_textureVide(textureVide)
{

}
Texture& Texture::operator=(const Texture &toCopy)
{
    m_filename = toCopy.m_filename;

    m_largeur       =   toCopy.m_largeur;
    m_hauteur       =   toCopy.m_hauteur;
    m_format        =   toCopy.m_format;
    m_formatInterne =   toCopy.m_formatInterne;
    m_textureVide   =   toCopy.m_textureVide;
    if (m_textureVide && glIsTexture(toCopy.m_id) == GL_TRUE)
        this->LoadEmptyTexture();
    else if (GL_TRUE == glIsTexture(toCopy.m_id) )
        this->load();

    return *this;
}
Texture::~Texture()
{
    //dtor
    glDeleteTextures(1,&m_id);
}
void Texture::LoadEmptyTexture()
{
    if(GL_TRUE == glIsTexture(m_id))
        glDeleteTextures(1, &m_id);

    glGenTextures(1,&m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, m_formatInterne, m_largeur, m_hauteur, 0, m_format, GL_UNSIGNED_BYTE, 0);
        // filtre
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

}
bool Texture::load()
{
    SDL_Surface     *imgSDL =   IMG_Load(m_filename.c_str());
    if (0 == imgSDL)
        throw string(SDL_GetError());

    if (glIsTexture(m_id) == GL_TRUE)
        glDeleteTextures(1,&m_id);

    //img.flipVertically();

    //sf::Vector2u taillexy = img.getSize();

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    GLenum      formatInterne(0);
    GLenum      format(0);

    // détermination du format interne
    if (imgSDL->format->BytesPerPixel   ==  3)
    {
        // format interne
        formatInterne       =   GL_RGB;

        // format
        if (imgSDL->format->Rmask   ==  0xff)
            format          =   GL_RGB;
        else
            format          =   GL_BGR;
    }
    else if (imgSDL->format->BytesPerPixel  ==  4)
    {
        // format interne
        formatInterne       =   GL_RGBA;

        // format
        if (imgSDL->format->Rmask   ==  0xff)
            format          =   GL_RGBA;
        else
            format          =   GL_BGRA;

    }
    else
    {
        // on arrête le chargement
        std::cerr << "Erreur format interne non reconu" << std::endl;
        SDL_FreeSurface(imgSDL);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, imgSDL->w, imgSDL->h, 0, format, GL_UNSIGNED_BYTE, imgSDL->pixels);

    // Application des filtres

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    // on a finis
    SDL_FreeSurface(imgSDL);
    return true;
}
void Texture::setFilename(const string &filename)
{
    m_filename =  filename;
}
GLuint Texture::getID() const
{
    return m_id;
}
