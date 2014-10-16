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
#ifndef DEF_SHADER
#define DEF_SHADER

// GLEW for all platform
#include <GL/glew.h>


#include <iostream>
#include <string>
#include <fstream>


namespace GraphicEngine
{

	// Classe Shader
	class Shader
	{
    	public:

    	Shader();
    	Shader(Shader const &shaderACopier);
    	Shader(std::string vertexSource, std::string fragmentSource);
    	~Shader();

    	Shader& operator=(Shader const &shaderACopier);

    	bool Load();
    	bool BuildShader(GLuint &shader, GLenum type, std::string const &fichierSource);
    	GLuint GetProgramID() const;


    	private:

    	GLuint m_vertexID;
    	GLuint m_fragmentID;
    	GLuint m_programID;

    	std::string m_vertexSource;
    	std::string m_fragmentSource;
	};
}
#endif
