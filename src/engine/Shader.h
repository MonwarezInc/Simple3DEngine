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
#pragma once
// GLEW for all platform
#include <GL/glew.h>


#include <fstream>
#include <iostream>
#include <string>

namespace S3DE
{
// Classe Shader

class Shader
{
public:
    Shader();
    /// \brief Copy constructor
    /// \param shaderToCopy     the shader to copy
    Shader( Shader const &shaderToCopy );
    /// \brief Common constructor used
    /// \param vertexSource the filename of the vertex shader
    /// \param fragmentSource   the filename of the fragment shader
    /// \param geometrySource   the filename of the geometry shader
    Shader( std::string const &vertexSource, std::string const &fragmentSource,
            std::string const &geometrySource );
    virtual ~Shader();
    /// \brief Assignation operator
    /// \param shaderToCopy the shader to copy
    Shader &operator=( Shader const &shaderToCopy );
    /// \brief	Set or reset the filename of the shaders
    /// \param vertexSource the filename of the vertex shader
    /// \param fragmentSource   the filename of the fragment shader
    /// \param geometrySource   the filename of the geometry shader
    virtual void SetFile( std::string const &vertexSource, std::string const &fragmentSource,
                          std::string const &geometrySource );
    /// \brief  Just call glUseProgram(shaderID)
    virtual void Enable();
    /// \brief  Just call glUseProgram(0)
    virtual void Disable();
    /// \brief Return the location of a uniform variable in the shader
    /// \param  name    the name of the uniform variable
    /// \return the location which is a GLuint
    virtual GLuint GetUniformLocation( std::string const &name ) const;

private:
    virtual void Load();
    bool BuildShader( GLuint &shader, GLenum type, std::string const &source );

    GLuint m_vertexID;
    GLuint m_fragmentID;
    GLuint m_geometryID;
    GLuint m_programID;

    std::string m_vertexSource;
    std::string m_fragmentSource;
    std::string m_geometrySource;

    bool isLoaded_{false};
};
} // namespace S3DE
